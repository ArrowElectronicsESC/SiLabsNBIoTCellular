################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/source/platform_mgr/platform_mgr.c 

OBJS += \
./common/source/platform_mgr/platform_mgr.o 

C_DEPS += \
./common/source/platform_mgr/platform_mgr.d 


# Each subdirectory must supply rules for building sources it contributes
common/source/platform_mgr/platform_mgr.o: ../common/source/platform_mgr/platform_mgr.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFM32_MICRIUM=1' '-DXBEE_CELLULAR_ENABLED=1' '-DXBEE_DEMO_HOLOGRAM_SIM=1' '-DUSER="user"' '-DXBEE_CHANGE_APN=1' '-DPWD="password"' '-DXBEE_TARGET_APN="hologram"' '-DXBEE_PLATFORM_HEADER="xbee/platform_efm32.h"' '-DXBEE_DEMO_CONFIG=1' '-DDEBUG_EFM=1' '-DXBEE_DEVICE_ENABLE_ATMODE=1' '-DEFM32GG11B820F2048GL192=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/SEGGER" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/segger/systemview/Sample/MicriumOSKernel" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//app/mcu_example/SLSTK3701A_EFM32GG11/lte_xbee_device_cloud/cfg" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/src/efm32" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/config/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//util/third_party/digi_lte/xbee/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/SLSTK3701A_EFM32GG11/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/micrium_os/bsp/siliconlabs/generic/include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/glib" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/middleware/glib/dmd/ssd2119" -I"C:\cygwin64\usr\include" -I"C:\cygnus\cygwin-b20\include" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"common/source/platform_mgr/platform_mgr.d" -MT"common/source/platform_mgr/platform_mgr.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


