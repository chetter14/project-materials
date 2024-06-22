#include "init.h"
#include "adc-dac-api.h"
#include <cmath>
#include <iostream>
#include <chrono>


void ADC(double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	if (*PLUGIN_COMMON.NEWINT == 1)
	{
		ADC_DAC_API::INIT(PAR);
		ADC_DAC_API::SET_ADC_PARS(PAR);
		WRK[0] = 0.0;
	}
	else if (*PLUGIN_COMMON.NITER == 1)
	{
		int id = getId();
		WRK[0] = ADC_DAC_API::READ_FROM_ADC(PAR, id);

		double currentTime = *PLUGIN_COMMON.TIME + *PLUGIN_COMMON.STEP;
		if (fabs(currentTime - *PLUGIN_COMMON.TIMEND) <= *PLUGIN_COMMON.SMIN)
			ADC_DAC_API::TERMINATE_ADC(id);
	}
}

void DAC(double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK,
	double value
)
{
	if (*PLUGIN_COMMON.NEWINT == 1)
	{
		ADC_DAC_API::INIT(PAR);
		ADC_DAC_API::SET_DAC_PARS(PAR);
	}
	else if (*PLUGIN_COMMON.NITER == 1)
	{
		int id = getId();
		ADC_DAC_API::WRITE_TO_DAC(PAR, id, value);

		double currentTime = *PLUGIN_COMMON.TIME + *PLUGIN_COMMON.STEP;
		if (fabs(currentTime - *PLUGIN_COMMON.TIMEND) <= *PLUGIN_COMMON.SMIN)
			ADC_DAC_API::TERMINATE_DAC(id);
	}

	NEW[0] = V[0];

	Y[0] = 0.0;
	I[0] = 0.0;
}


extern "C" void PLUGIN_INIT_CC ADC1A(
	double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	ADC(I, Y, X, V, A, PAR, NEW, OLD, WRK);

	Y[0] = 1.0;
	I[0] = V[0] - WRK[0];
}

extern "C" void PLUGIN_INIT_CC ADC2A(
	double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	ADC(I, Y, X, V, A, PAR, NEW, OLD, WRK);

	double booleanHighValue = PAR[PARS_INDEX::BOOLEAN_HIGH_VALUE];
	double result = 0.0;
	if (ADC_DAC_API::isEqual(WRK[0], booleanHighValue) || WRK[0] > booleanHighValue)
		result = 1.0;

	//std::cout << "Adc value - " << WRK[0] << "\n";

	Y[0] = 1.0;
	I[0] = V[0] - result;
}

extern "C" void PLUGIN_INIT_CC DAC1A(
	double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	if (*PLUGIN_COMMON.NEWINT == 1)
	{
		NEW[0] = 0.0;
		OLD[0] = 0.0;
	}

	DAC(I, Y, X, V, A, PAR, NEW, OLD, WRK, OLD[0]);
}

extern "C" void PLUGIN_INIT_CC DAC2A(
	double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	if (*PLUGIN_COMMON.NEWINT == 1)
	{
		NEW[0] = 0.0;
		OLD[0] = 0.0;
	}

	constexpr double logicHigh = 1.0;
	double value = 0.0;
	if (ADC_DAC_API::isEqual(OLD[0], logicHigh) || OLD[0] > logicHigh)
		value = PAR[PARS_INDEX::BOOLEAN_HIGH_VALUE];

	DAC(I, Y, X, V, A, PAR, NEW, OLD, WRK, value);
}

namespace
{
	std::chrono::time_point<std::chrono::system_clock> startTime, curTime;
}

extern "C" void PLUGIN_INIT_CC RT(
	double* I,
	double* Y,
	double* X,
	double* V,
	double*	A,
	double* PAR,
	double* NEW,
	double* OLD,
	double* WRK
)
{
	if (*PLUGIN_COMMON.NEWINT == 1)
	{
		startTime = std::chrono::system_clock::now();
	}
	else if (*PLUGIN_COMMON.NITER == 1)
	{
		curTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsedSeconds = curTime - startTime;
		WRK[0] = elapsedSeconds.count();
	}

	Y[0] = 1.0;
	I[0] = V[0] - WRK[0];
}