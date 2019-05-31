################################################################################
# Automatically-generated file. Do not edit!
################################################################################

export WINEDEBUG=-all

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include ports/source/iar/subdir.mk
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
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(MSA_UPPER_DEPS)),)
-include $(MSA_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S79_DEPS)),)
-include $(S79_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(MSA_DEPS)),)
-include $(MSA_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S79_UPPER_DEPS)),)
-include $(S79_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SLSTK3701A_lte_xbee_EBC.out

# Tool invocations
SLSTK3701A_lte_xbee_EBC.out: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: IAR Linker for ARM'
	ilinkarm $(OBJS) $(USER_OBJS) -o "SLSTK3701A_lte_xbee_EBC.out" --config "SLSTK3701A_lte_xbee_EBC.icf" --no_wrap_diagnostics  
	ielftool --ihex --verbose "SLSTK3701A_lte_xbee_EBC.out" "SLSTK3701A_lte_xbee_EBC.hex"  
	ielftool --bin --verbose "SLSTK3701A_lte_xbee_EBC.out" "SLSTK3701A_lte_xbee_EBC.bin"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(MSA_UPPER_DEPS)$(EXECUTABLES)$(OBJS)$(ASM_DEPS)$(S79_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(MSA_DEPS)$(C_DEPS)$(S79_UPPER_DEPS) SLSTK3701A_lte_xbee_EBC.out
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
