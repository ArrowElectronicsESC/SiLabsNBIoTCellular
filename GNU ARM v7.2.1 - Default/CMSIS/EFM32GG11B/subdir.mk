################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/EFM32GG11B/system_efm32gg11b.c 

S_UPPER_SRCS += \
C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5/platform/Device/SiliconLabs/EFM32GG11B/Source/GCC/startup_efm32gg11b.S 

OBJS += \
./CMSIS/EFM32GG11B/startup_efm32gg11b.o \
./CMSIS/EFM32GG11B/system_efm32gg11b.o 

C_DEPS += \
./CMSIS/EFM32GG11B/system_efm32gg11b.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFM32GG11B/startup_efm32gg11b.o: C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5/platform/Device/SiliconLabs/EFM32GG11B/Source/GCC/startup_efm32gg11b.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -c -x assembler-with-cpp -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" '-DEFM32_MICRIUM=1' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DUSER="user"' '-DXBEE_CHANGE_APN=1' '-DPWD="password"' '-DXBEE_TARGET_APN="hologram"' '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_DEMO_CONFIG=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DEFM32GG11B820F2048GL192=1' -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFM32GG11B/system_efm32gg11b.o: ../CMSIS/EFM32GG11B/system_efm32gg11b.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFM32_MICRIUM=1' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DUSER="user"' '-DXBEE_CHANGE_APN=1' '-DPWD="password"' '-DXBEE_TARGET_APN="hologram"' '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_DEMO_CONFIG=1' '-DDEBUG_EFM=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DEFM32GG11B820F2048GL192=1' -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4_2/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"CMSIS/EFM32GG11B/system_efm32gg11b.d" -MT"CMSIS/EFM32GG11B/system_efm32gg11b.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


