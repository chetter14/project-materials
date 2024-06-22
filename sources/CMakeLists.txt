add_library(AdcDacDll SHARED main.cpp main.def adc-dac-api.cpp "LCard/e154.cpp" "LCard/e502.cpp" "LCard/e140m.cpp" "LCard/LoadDll.cpp")

set_target_properties(AdcDacDll PROPERTIES OUTPUT_NAME "ADCDAC")
