#ifndef PRADIS_ADC_DAC_API_H
#define PRADIS_ADC_DAC_API_H

#include "common.h"
#include <string>
#include <cmath>

PLUGIN_DECLARE_COMMON

namespace ADC_DAC_API
{
	void INIT(double* PAR);
	void SET_ADC_PARS(double* PAR);
	void SET_DAC_PARS(double* PAR);
	double READ_FROM_ADC(double* PAR, int id);
	void WRITE_TO_DAC(double* PAR, int id, double value);
	void TERMINATE_ADC(int id);
	void TERMINATE_DAC(int id);

	bool isEqual(double lhs, double rhs);
}

enum class CONTROLLER
{
	LCARD_E154 = 0,
	LCARD_E140M,
	LCARD_E502
};

enum PARS_INDEX
{
	BOOLEAN_HIGH_VALUE = 0,
	CHANNEL,
	CONTROLLER_TYPE,
	CONTROLLER_PARAMS
};

class ControllerAPI
{
public:
	virtual void INIT() = 0;
	virtual void SET_ADC_PARS(double* PAR) = 0;
	virtual void SET_DAC_PARS(double* PAR) = 0;
	virtual double READ_FROM_ADC(double* PAR) = 0;
	virtual void WRITE_TO_DAC(double* PAR, double value) = 0;
	virtual void TERMINATE_ADC() = 0;
	virtual void TERMINATE_DAC() = 0;

	virtual CONTROLLER getControllerType() = 0;

	virtual ~ControllerAPI() = default;

protected:
	ControllerAPI() = default;
	ControllerAPI(const ControllerAPI&) = default;
	ControllerAPI(ControllerAPI&&) = default;

	ControllerAPI& operator=(const ControllerAPI&) = default;
	ControllerAPI& operator=(ControllerAPI&&) = default;

	// функция очистки всех выделенных ресурсов для контроллера
	virtual void cleanUp(bool forceCleanUpFlag = false) = 0;

	// функция экстренного завершения программы из-за ошибки
	virtual void AbortProgram(const char* ErrorString) final
	{
		printf(ErrorString);
		*PLUGIN_COMMON.CODE = 100;
		cleanUp(true);
	}

	// счётчик АЦП/ЦАП моделей (для инициализации и завершения)
	int ADC_DAC_MODELS_COUNTER = 0;
	// флаг - установка АЦП/ЦАП параметров уже происходила или нет
	bool setAdcPars = false, setDacPars = false;
	// флаг - выключили АЦП/ЦАП на данном контроллере
	bool didTerminateAdc = false, didTerminateDac = false;
};

extern int toInt(double x);
extern int getId();

#endif // PRADIS_ADC_DAC_API_H