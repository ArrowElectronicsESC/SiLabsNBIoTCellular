################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../CMSIS/EFM32GG11B/startup_iar_efm32gg11b.s 

C_SRCS += \
../CMSIS/EFM32GG11B/system_efm32gg11b.c 

S_UPPER_SRCS += \
C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5/platform/Device/SiliconLabs/EFM32GG11B/Source/GCC/startup_efm32gg11b.S 

OBJS += \
./CMSIS/EFM32GG11B/startup_efm32gg11b.o \
./CMSIS/EFM32GG11B/startup_iar_efm32gg11b.o \
./CMSIS/EFM32GG11B/system_efm32gg11b.o 

S_DEPS += \
./CMSIS/EFM32GG11B/startup_iar_efm32gg11b.d 

S_UPPER_DEPS += \
./CMSIS/EFM32GG11B/startup_gcc_efm32gg11b.d 

C_DEPS += \
./CMSIS/EFM32GG11B/system_efm32gg11b.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFM32GG11B/startup_efm32gg11b.o: C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5/platform/Device/SiliconLabs/EFM32GG11B/Source/GCC/startup_efm32gg11b.S
	@echo 'Building file: $<'
	@echo 'Invoking: IAR Assembler for ARM'
	iasmarm "$<" -o  "$@" --cpu Cortex-M4 --fpu VFPv4_sp -s+ -r -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DXBEE_DEMO_CONFIG=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DXBEE_CHANGE_APN=1' '-DXBEE_TARGET_APN="hologram"' '-DUSER="user"' '-DPWD="password"' '-DEFM32_MICRIUM=1' '-DEFM32GG11B820F2048GL192=1'
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFM32GG11B/startup_iar_efm32gg11b.o: ../CMSIS/EFM32GG11B/startup_iar_efm32gg11b.s
	@echo 'Building file: $<'
	@echo 'Invoking: IAR Assembler for ARM'
	iasmarm "$<" -o  "$@" --cpu Cortex-M4 --fpu VFPv4_sp -s+ -r -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DXBEE_DEMO_CONFIG=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DXBEE_CHANGE_APN=1' '-DXBEE_TARGET_APN="hologram"' '-DUSER="user"' '-DPWD="password"' '-DEFM32_MICRIUM=1' '-DEFM32GG11B820F2048GL192=1'
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFM32GG11B/system_efm32gg11b.o: ../CMSIS/EFM32GG11B/system_efm32gg11b.c
	@echo 'Building file: $<'
	@echo 'Invoking: IAR C/C++ Compiler for ARM'
	iccarm "$<" -o "$@" --no_wrap_diagnostics -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.5\/lib/gcc/arm-none-eabi/4.8.3/include" -I"C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.5\/arm-none-eabi/include" -I"C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.5\/lib/gcc/arm-none-eabi/4.8.3/include-fixed" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" -e --cpu Cortex-M4 --fpu VFPv4_sp --debug --dlib_config "C:/Program Files (x86)/IAR Systems/Embedded Workbench 7.5/arm/inc/c/DLib_Config_Normal.h" --endian little --cpu_mode thumb -On --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling '-DDEBUG_EFM=1' '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DXBEE_DEMO_CONFIG=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DXBEE_CHANGE_APN=1' '-DXBEE_TARGET_APN="hologram"' '-DUSER="user"' '-DPWD="password"' '-DEFM32_MICRIUM=1' '-DEFM32GG11B820F2048GL192=1' --dependencies=m CMSIS/EFM32GG11B/system_efm32gg11b.d
	@echo 'Finished building: $<'
	@echo ' '


