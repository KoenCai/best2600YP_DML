
cmd_platform/cmsis/DSP_Lib/cmsis_dsp_lib.o := arm-none-eabi-gcc -Wl,-r,--whole-archive -nostdlib -nostartfiles  -mthumb -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o platform/cmsis/DSP_Lib/cmsis_dsp_lib.o platform/cmsis/DSP_Lib/BasicMathFunctions/built-in.a platform/cmsis/DSP_Lib/CommonTables/built-in.a platform/cmsis/DSP_Lib/FilteringFunctions/built-in.a platform/cmsis/DSP_Lib/MatrixFunctions/built-in.a platform/cmsis/DSP_Lib/SupportFunctions/built-in.a platform/cmsis/DSP_Lib/StatisticsFunctions/built-in.a platform/cmsis/DSP_Lib/TransformFunctions/built-in.a platform/cmsis/DSP_Lib/FastMathFunctions/built-in.a
