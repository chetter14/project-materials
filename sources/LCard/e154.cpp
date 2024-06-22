#include "Lusbapi.h"
#include "LoadDll.h"
#include "../common.h"
#include "lcard-api.h"
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <unordered_map>


PLUGIN_DECLARE_COMMON

namespace
{
	enum ADC_PARS
	{
		ClkSource = 0,
		EnableClkOutput,
		InputMode,
		SynchroAdType,
		SynchroAdMode,
		SynchroAdChannel,
		SynchroAdPorog,
		AdcRate,
		InterKadrDelay
	};

	// указатель на класс динамической загрузки DLL
	TLoadDll *pLoadDll;

	// максимально возможное кол-во опрашиваемых 
	// виртуальных слотов (при поиске модуля)
	const WORD MaxVirtualSoltsQuantity = 511;

	constexpr int NumberOfWordsToPass = 32;
	constexpr int MAX_CHANNELS_QUANTITY = 16;

	// карта с парами: id модели - индекс логического канала (этой модели) в управляющей таблице (общая для контроллера)
	std::unordered_map<int, unsigned int> logicalChannelsIndices;

	// версия библиотеки
	DWORD DllVersion;
	// указатель на интерфейс модуля
	ILE154 *pModule;
	// дескриптор устройства
	HANDLE ModuleHandle;
	// название модуля
	char ModuleName[7];
	// скорость работы шины USB
	BYTE UsbSpeed;
	// структура с полной информацией о модуле
	MODULE_DESCRIPTION_E154 ModuleDescription;
	// структура параметров работы АЦП модуля
	ADC_PARS_E154 ap;

	// буфер данных
	SHORT ReadBuffer[NumberOfWordsToPass];

	// OVERLAPPED структура
	OVERLAPPED ReadOv;
	// структура с параметрами запроса на ввод/вывод данных ; параметры и требуемый режим получения данных с модуля E-154
	IO_REQUEST_LUSBAPI IoReq;
}

void LCardE154::cleanUp(bool forceCleanUpFlag)
{
	if (!forceCleanUpFlag && --ADC_DAC_MODELS_COUNTER != 0)	// для очистки на последней модели
		return;

	// освободим идентификатор событий
	if (ReadOv.hEvent) CloseHandle(ReadOv.hEvent);
	// подчищаем интерфейс модуля
	if (pModule)
	{
		// освободим интерфейс модуля
		if (!pModule->ReleaseLInstance()) printf(" ReleaseLInstance() --> Bad\n");
		else printf(" ReleaseLInstance() --> OK\n");
		// обнулим указатель на интерфейс модуля
		pModule = NULL;
	}

	// освободим библиотеку
	if (pLoadDll) { delete pLoadDll; pLoadDll = NULL; }

	// прочистим очередь клавиатуры
	if (kbhit()) { while (kbhit()) getch(); }

	// или спокойно выходим из функции   
	else return;
}

void LCardE154::INIT()
{
	printf("ADC DAC MODELS COUNTER - %d\n", ADC_DAC_MODELS_COUNTER);
	if (ADC_DAC_MODELS_COUNTER++ != 0)	// чтобы не инициализировать LCard E-154 несколько раз
		return;

	printf(" *******************************\n");
	printf(" Module E-154                   \n");
	printf(" Console example for ADC Stream \n");
	printf(" *******************************\n\n");

	// динамическая загрузка библиотеки "Lusbapi.dll"
	const char* envVar = "DINSYS";
	std::string dinsysPath = std::getenv(envVar);
	std::string lusbapiPath = dinsysPath + "\\DINAMA\\plugin\\Lusbapi.dll";

	pLoadDll = new TLoadDll((char*)lusbapiPath.c_str());
	if (!pLoadDll) AbortProgram(" Can't alloc 'TLoadDll' object!!!\n");

	// проверим смогли ли мы загрузить библиотеку?
	if (!pLoadDll->GetDllHinstance()) AbortProgram(" 'Lusbapi.dll' Dynamic Loading --> Bad\n");
	else printf(" 'Lusbapi.dll' Dynamic Loading --> OK\n");

	// адрес функции получения версии библиотеки
	pGetDllVersion GetDllVersion = (pGetDllVersion)pLoadDll->CallGetDllVersion();
	if (!GetDllVersion) AbortProgram(" Address of GetDllVersion() --> Bad\n");
	else printf(" Address of GetDllVersion() --> OK\n");

	// проверим версию используемой библиотеки Lusbapi.dll
	if ((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
		char String[128];
		sprintf(String, " Lusbapi.dll Version Error!!!\n   Current: %1u.%1u. Required: %1u.%1u",
			DllVersion >> 0x10, DllVersion & 0xFFFF,
			CURRENT_VERSION_LUSBAPI >> 0x10, CURRENT_VERSION_LUSBAPI & 0xFFFF);

		AbortProgram(String);
	}
	else printf(" Lusbapi.dll Version --> OK\n");

	// адрес функции получения интерфейса модуля
	pCreateInstance CreateLInstance = (pCreateInstance)pLoadDll->CallCreateLInstance();
	if (!CreateLInstance) AbortProgram(" Address of CreateLInstance() --> Bad!!!\n");
	else printf(" Address of CreateLInstance() --> OK\n");

	// попробуем получить указатель на интерфейс
	pModule = static_cast<ILE154 *>(CreateLInstance("e154"));
	if (!pModule) AbortProgram(" Module Interface --> Bad\n");
	else printf(" Module Interface --> OK\n");

	// попробуем обнаружить модуль E-154 в первых 256 виртуальных слотах
	DWORD i;
	for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenLDevice(i)) break;
	// что-нибудь обнаружили?
	if (i == MaxVirtualSoltsQuantity) AbortProgram(" Can't find any module E-154 in virtual slots!\n");
	else printf(" OpenLDevice(%u) --> OK\n", i);

	// попробуем прочитать дескриптор устройства
	ModuleHandle = pModule->GetModuleHandle();
	if (ModuleHandle == INVALID_HANDLE_VALUE) AbortProgram(" GetModuleHandle() --> Bad\n");
	else printf(" GetModuleHandle() --> OK\n");

	// прочитаем название модуля в обнаруженном виртуальном слоте
	if (!pModule->GetModuleName(ModuleName)) AbortProgram(" GetModuleName() --> Bad\n");
	else printf(" GetModuleName() --> OK\n");
	// проверим, что это 'E-154'
	if (strcmp(ModuleName, "E154")) AbortProgram(" The module is not 'E-154'\n");
	else printf(" The module is 'E-154'\n");

	// попробуем получить скорость работы шины USB
	if (!pModule->GetUsbSpeed(&UsbSpeed)) AbortProgram(" GetUsbSpeed() --> Bad\n");
	else printf(" GetUsbSpeed() --> OK\n");
	// теперь отобразим скорость работы шины USB
	printf("   USB is in %s\n", UsbSpeed ? "High-Speed Mode (480 Mbit/s)" : "Full-Speed Mode (12 Mbit/s)");

	// получим информацию из ППЗУ модуля
	if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription)) AbortProgram(" GET_MODULE_DESCRIPTION() --> Bad\n");
	else printf(" GET_MODULE_DESCRIPTION() --> OK\n");

	ap.ChannelsQuantity = 0;
}

void LCardE154::SET_ADC_PARS(double* PAR)
{
	// запоминаем канал и inputRange

	int channel = toInt(PAR[PARS_INDEX::CHANNEL]);
	int inputRange = toInt(PAR[PARS_INDEX::CONTROLLER_PARAMS + LCARD_CONTROLLER_PARAMS::INPUT_RANGE]);

	++ap.ChannelsQuantity;
	if (ap.ChannelsQuantity > MAX_CHANNELS_QUANTITY) AbortProgram(" Too many channels! Unable to continue!\n");

	int id = getId();
	logicalChannelsIndices[id] = ap.ChannelsQuantity - 1;

	// формируем управляющую таблицу
	ap.ControlTable[ap.ChannelsQuantity - 1] = (WORD)(channel | (inputRange << 0x6));	
}

void LCardE154::SET_DAC_PARS(double* PAR)
{
	;	// ничего не устанавливается, так как в E154 модуле отсутствует такой функционал для ЦАП
}

double LCardE154::READ_FROM_ADC(double* PAR)
{
	if (!setAdcPars)
	{
		int adcParsIndex = PARS_INDEX::CONTROLLER_PARAMS + LCARD_CONTROLLER_PARAMS::TECH_PARAMS;

		ap.ClkSource = toInt(PAR[adcParsIndex + ADC_PARS::ClkSource]);						// внутренний запуск АЦП
		ap.EnableClkOutput = toInt(PAR[adcParsIndex + ADC_PARS::EnableClkOutput]); 			// без трансляции тактовых импульсов АЦП
		ap.InputMode = toInt(PAR[adcParsIndex + ADC_PARS::InputMode]);						// без синхронизации ввода данных
		ap.SynchroAdType = toInt(PAR[adcParsIndex + ADC_PARS::SynchroAdType]);
		ap.SynchroAdMode = toInt(PAR[adcParsIndex + ADC_PARS::SynchroAdMode]);
		ap.SynchroAdChannel = toInt(PAR[adcParsIndex + ADC_PARS::SynchroAdChannel]);
		ap.SynchroAdPorog = toInt(PAR[adcParsIndex + ADC_PARS::SynchroAdPorog]);
		ap.AdcRate = PAR[adcParsIndex + ADC_PARS::AdcRate];							// частота работы АЦП в кГц
		ap.InterKadrDelay = PAR[adcParsIndex + ADC_PARS::InterKadrDelay];			// межкадровая задержка в мс

		// передадим требуемые параметры работы АЦП в модуль
		if (!pModule->SET_ADC_PARS(&ap)) AbortProgram(" SET_ADC_PARS() --> Bad\n");
		else printf(" SET_ADC_PARS() --> OK\n");

		// отобразим параметры сбора данных модуля на экране монитора
		printf(" \n");
		printf(" Module E-154 (S/N %s) is ready ... \n", ModuleDescription.Module.SerialNumber);
		printf("   Module Info:\n");
		printf("     Module  Revision   is '%c'\n", ModuleDescription.Module.Revision);
		printf("     MCU Driver Version is %s (%s)\n", ModuleDescription.Mcu.Version.Version, ModuleDescription.Mcu.Version.Date);
		printf("   Adc parameters:\n");
		printf("     ChannelsQuantity = %2d\n", ap.ChannelsQuantity);
		printf("     AdcRate = %8.3f kHz\n", ap.AdcRate);
		printf("     InterKadrDelay = %2.4f ms\n", ap.InterKadrDelay);
		printf("     KadrRate = %8.3f kHz\n", ap.KadrRate);

		// остановим работу АЦП и одновременно сбросим USB-канал чтения данных
		if (!pModule->STOP_ADC()) { AbortProgram(" STOP_ADC() --> Bad\n"); }

		// формируем структуру IoReq
		IoReq.Buffer = &ReadBuffer[0];								// массив данных
		IoReq.NumberOfWordsToPass = NumberOfWordsToPass;			// читаем n-ое кол-во данных
		IoReq.NumberOfWordsPassed = 0x0;
		IoReq.Overlapped = NULL;
		// (общее число отсчётов) / (частота между соседними отсчётами) + 1с (на случай задержек ?)
		IoReq.TimeOut = (DWORD)(1 / ap.AdcRate + 1000);
		
		// запустим АЦП
		if (!pModule->START_ADC()) AbortProgram(" START_ADC() --> Bad\n");

		setAdcPars = true;
	}

	if (didTerminateAdc)
		return 0.0;

	if (!pModule->ReadData(&IoReq)) AbortProgram(" Couldn't read data from ADC\n");

	// получаем индекс логического канала, чтобы прочитать нужный нам АЦП код из ReadBuffer
	unsigned int logicalChannelIndex = logicalChannelsIndices[getId()];

	double result;
	if (!pModule->ProcessOnePoint(ReadBuffer[logicalChannelIndex], &result, ap.ControlTable[logicalChannelIndex], true, true)) AbortProgram(" Couldn't process ADC code into real format\n");

	//printf("Value from ADC - %f\n", result);

	return result;
}

void LCardE154::WRITE_TO_DAC(double* PAR, double value)
{
	if (didTerminateDac)
		return;

	if (!pModule->DAC_SAMPLE_VOLT(value, true)) AbortProgram(" Couldn't write data to DAC\n");
}

void LCardE154::TERMINATE_ADC()
{
	if (didTerminateAdc)
		return;

	// прервём возможно незавершённый асинхронный запрос на приём данных
	if (!CancelIo(ModuleHandle)) AbortProgram(" Couldn't close IO stream of module\n");

	if (!pModule->STOP_ADC()) AbortProgram(" STOP_ADC() --> Bad\n");

	cleanUp();
	didTerminateAdc = true;
}

void LCardE154::TERMINATE_DAC()
{
	if (didTerminateDac)
		return;

	if (!pModule->DAC_SAMPLE_VOLT(0, true)) AbortProgram(" Couldn't write data to DAC\n");

	cleanUp();
	didTerminateDac = true;
}

