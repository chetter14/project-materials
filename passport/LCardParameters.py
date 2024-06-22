# -*- coding: utf-8 -*-
from include import *

class LCardParameters:

    def __init__ (self, nl, pl, desc="misc.default"):

        module_type = pl[0]
        input_range = pl[1]
        tech_params = pl[2]

        InputRangeNumberMap = { "5": 0, "1_6": 1, "0_5": 2, "0_16": 3 }

        self.module_type = module_type 
        self.input_range = InputRangeNumberMap[input_range]

        self.pl = [self.input_range]
        self.pl.extend(tech_params)