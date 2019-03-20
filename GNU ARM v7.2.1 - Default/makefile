################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include ports/source/gnu/subdir.mk
-include ports/source/generic/subdir.mk
-include kernel/source/subdir.mk
-include emlib/subdir.mk
-include cpu/source/subdir.mk
-include common/source/rtos/subdir.mk
-include common/source/platform_mgr/subdir.mk
-include common/source/lib/subdir.mk
-include common/source/kal/subdir.mk
-include common/source/common/subdir.mk
-include common/source/collections/subdir.mk
-include common/source/clk/subdir.mk
-include common/source/auth/subdir.mk
-include XBee/subdir.mk
-include SystemView/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32GG11B/subdir.mk
-include BSP/siliconlabs/generic/source/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SLSTK3701A_lte_xbee_EBC.axf

# Tool invocations
SLSTK3701A_lte_xbee_EBC.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "SLSTK3701A_lte_xbee_EBC.ld" -Xlinker --gc-sections -Xlinker -Map="SLSTK3701A_lte_xbee_EBC.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o SLSTK3701A_lte_xbee_EBC.axf "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" "./BSP/siliconlabs/generic/source/bsp_cpu.o" "./BSP/siliconlabs/generic/source/bsp_os.o" "./CMSIS/EFM32GG11B/startup_efm32gg11b.o" "./CMSIS/EFM32GG11B/system_efm32gg11b.o" "./Drivers/display.o" "./Drivers/displayls013b7dh06.o" "./Drivers/displaypalemlib.o" "./Drivers/i2cspm.o" "./Drivers/retargetio.o" "./Drivers/retargettextdisplay.o" "./Drivers/si7013.o" "./Drivers/textdisplay.o" "./Drivers/udelay.o" "./SystemView/SEGGER_RTT.o" "./SystemView/SEGGER_SYSVIEW.o" "./SystemView/SEGGER_SYSVIEW_Config_MicriumOSKernel.o" "./SystemView/SEGGER_SYSVIEW_MicriumOSKernel.o" "./XBee/hexdump.o" "./XBee/hexstrtobyte.o" "./XBee/jslong.o" "./XBee/memcheck.o" "./XBee/swapbytes.o" "./XBee/swapcpy.o" "./XBee/wpan_types.o" "./XBee/xbee_atcmd.o" "./XBee/xbee_atmode.o" "./XBee/xbee_cbuf.o" "./XBee/xbee_config.o" "./XBee/xbee_device.o" "./XBee/xbee_ipv4.o" "./XBee/xbee_platform_efm32.o" "./XBee/xbee_serial_efm32.o" "./XBee/xmodem_crc16.o" "./common/source/auth/auth.o" "./common/source/clk/clk.o" "./common/source/clk/clk_cmd.o" "./common/source/collections/bitmap.o" "./common/source/collections/map.o" "./common/source/collections/slist.o" "./common/source/common/common.o" "./common/source/kal/kal_kernel.o" "./common/source/lib/lib_ascii.o" "./common/source/lib/lib_math.o" "./common/source/lib/lib_mem.o" "./common/source/lib/lib_str.o" "./common/source/platform_mgr/platform_mgr.o" "./common/source/rtos/rtos_err_str.o" "./cpu/source/cpu_core.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_prs.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./kernel/source/os_cfg_app.o" "./kernel/source/os_core.o" "./kernel/source/os_dbg.o" "./kernel/source/os_flag.o" "./kernel/source/os_mem.o" "./kernel/source/os_mon.o" "./kernel/source/os_msg.o" "./kernel/source/os_mutex.o" "./kernel/source/os_prio.o" "./kernel/source/os_q.o" "./kernel/source/os_sem.o" "./kernel/source/os_stat.o" "./kernel/source/os_task.o" "./kernel/source/os_tick.o" "./kernel/source/os_time.o" "./kernel/source/os_tmr.o" "./kernel/source/os_var.o" "./ports/source/generic/armv6m_v7m_cpu_int.o" "./ports/source/gnu/armv7m_cpu_a.o" "./ports/source/gnu/armv7m_os_cpu_a.o" "./ports/source/gnu/armv7m_os_cpu_c.o" "./src/gnss.o" "./src/http_client.o" "./src/leds.o" "./src/main.o" "./src/temp_sensor.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SLSTK3701A_lte_xbee_EBC.hex'
	arm-none-eabi-objcopy -O ihex "SLSTK3701A_lte_xbee_EBC.axf" "SLSTK3701A_lte_xbee_EBC.hex"
	@echo ' '

	@echo 'Building bin file: SLSTK3701A_lte_xbee_EBC.bin'
	arm-none-eabi-objcopy -O binary "SLSTK3701A_lte_xbee_EBC.axf" "SLSTK3701A_lte_xbee_EBC.bin"
	@echo ' '

	@echo 'Building s37 file: SLSTK3701A_lte_xbee_EBC.s37'
	arm-none-eabi-objcopy -O srec "SLSTK3701A_lte_xbee_EBC.axf" "SLSTK3701A_lte_xbee_EBC.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SLSTK3701A_lte_xbee_EBC.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SLSTK3701A_lte_xbee_EBC.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
