# -*- coding: utf-8 -*-
from include import *
from misc import *
from LCardParameters import *

adc_counter = 1

class ADC:

    def __init__ (self, nl, pl, desc="misc.default"):

        adc_sensor_type = pl[0]
        boolean_high_value = pl[1]
        channel = pl[2]
        controller_params_obj = pl[3]

        params = [boolean_high_value, channel]

        if isinstance(controller_params_obj, LCardParameters):    # как вариант, работу с контроллером можно убрать в отдельный python файл, так как в DAC.py функционал дублируется
            ModuleTypeNumberMap = { "E154": 0, "E140M": 1, "E502": 2 }
            controller_type = ModuleTypeNumberMap[controller_params_obj.module_type]
            params.append(controller_type)
        else:
            misc.SetError(-1, misc.ErrorType.Error, "Объект параметров контроллера не подходит!")

        params.extend(controller_params_obj.pl)

        # вызываем соотвествующую ADC модель
        
        global adc_counter
        ppl_scheme_desc = ''

        if (adc_sensor_type == "Analog"):
            ADCM = Model("ADC1A", nl, params, desc = misc.ppl_scheme_desc(ppl_scheme_desc, "ADC1A" + str(adc_counter)))
        else:                   # Цифровой
            ADCM = Model("ADC2A", nl, params, desc = misc.ppl_scheme_desc(ppl_scheme_desc, "ADC2A" + str(adc_counter)))

        adc_counter = adc_counter + 1
            


