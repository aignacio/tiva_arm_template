################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
all: stellaris_lm3s6965.elf

# Tool invocations
stellaris_lm3s6965.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -L/home/aignacio/tools/stellarisware_lm3s6965/driverlib/gcc-cm3 -Wl,--static,--gc-sections,-T../ldStart.ld -mthumb -mcpu=cortex-m3 -o "stellaris_lm3s6965.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) stellaris_lm3s6965.elf
	-@echo ' '

post-build:
	-@echo 'generate .bin'
	-arm-none-eabi-objcopy -O binary stellaris_lm3s6965.elf stellaris_lm3s6965.bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
