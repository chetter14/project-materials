#include "adc-dac-api.h"
#include "LCard/lcard-api.h"
#include "common.h"
#include <memory>
#include <unordered_map>

PLUGIN_DECLARE_COMMON

namespace
{
	// карта id модели к её контроллеру (несколько разных id могут ссылаться на один и тот же контроллер)
	std::unordered_map<int, std::shared_ptr<ControllerAPI>> controllers;

	CONTROLLER toController(double parControllerType)
	{
		return static_cast<CONTROLLER>(toInt(parControllerType));
	}
}

bool ADC_DAC_API::isEqual(double lhs, double rhs)
{
	if (abs(lhs - rhs) < 0.001)
		return true;
	return false;
}

// Может принимать разные типы модулей
void ADC_DAC_API::INIT(double* PAR)
{
	CONTROLLER controllerType = toController(PAR[PARS_INDEX::CONTROLLER_TYPE]);
	int id = getId();

	// проходимся по всем уже созданным объектам контроллеров
	bool isOurControllerInitialized = false;
	for (auto& pair : controllers)
	{
		if (controllerType == pair.second->getControllerType())	// если такой объект этого контроллера уже создан
		{
			controllers.emplace(id, pair.second);
			isOurControllerInitialized = true;
			break;
		}
	}
	
	if (!isOurControllerInitialized)
	{
		switch (controllerType)
		{
			case CONTROLLER::LCARD_E154:
			{
				controllers[id] = std::make_shared<LCardE154>();
				break;
			}
			case CONTROLLER::LCARD_E140M:
			{
				//controllers[id] = std::make_shared<LCardE140M>();
				break;
			}
			case CONTROLLER::LCARD_E502:
			{
				//controllers[id] = std::make_shared<LCardE502>();
				break;
			}
			default:
			{
				*PLUGIN_COMMON.CODE = 100;
				printf("Получен некорректный тип контроллера\n");
				return;
			}
		}
	}
	
	controllers[id]->INIT();
}

void ADC_DAC_API::SET_ADC_PARS(double* PAR)
{
	int id = getId();
	controllers[id]->SET_ADC_PARS(PAR);
}

void ADC_DAC_API::SET_DAC_PARS(double* PAR)
{
	int id = getId();
	controllers[id]->SET_DAC_PARS(PAR);
}

double ADC_DAC_API::READ_FROM_ADC(double* PAR, int id)
{
	return controllers[id]->READ_FROM_ADC(PAR);
}

void ADC_DAC_API::WRITE_TO_DAC(double* PAR, int id, double value)
{
	controllers[id]->WRITE_TO_DAC(PAR, value);
}

void ADC_DAC_API::TERMINATE_ADC(int id)
{
	controllers[id]->TERMINATE_ADC();
}

void ADC_DAC_API::TERMINATE_DAC(int id)
{
	controllers[id]->TERMINATE_DAC();
}

int toInt(double x)
{
	return (int)(x + 0.5);
}

int getId()
{
	return *PLUGIN_COMMON.NUMPP;
}