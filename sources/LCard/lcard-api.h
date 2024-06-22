#ifndef PRADIS_LCARD_API_H
#define PRADIS_LCARD_API_H

#include "../adc-dac-api.h"

enum LCARD_CONTROLLER_PARAMS
{
	INPUT_RANGE = 0,
	TECH_PARAMS
};

class LCardE154 : public ControllerAPI
{
public:
	void INIT() override;
	void SET_ADC_PARS(double* PAR) override;
	void SET_DAC_PARS(double* PAR) override;
	double READ_FROM_ADC(double* PAR) override;
	void WRITE_TO_DAC(double* PAR, double value) override;
	void TERMINATE_ADC() override;
	void TERMINATE_DAC() override;

	CONTROLLER getControllerType() override { return CONTROLLER::LCARD_E154; }

private:
	void cleanUp(bool forceCleanUpFlag = false) override;
};

#endif	// PRADIS_LCARD_API_H
