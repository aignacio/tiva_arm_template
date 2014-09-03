################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c \
../startup_gcc.c 

OBJS += \
./main.o \
./startup_gcc.o 

C_DEPS += \
./main.d \
./startup_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DPART_LM3S6965 -I/home/aignacio/tools/stellarisware_lm3s6965/boards/ek-lm3s6965/drivers -I/home/aignacio/tools/stellarisware_lm3s6965/boards/ek-lm3s6965 -I/home/aignacio/tools/stellarisware_lm3s6965/boards -I/home/aignacio/tools/stellarisware_lm3s6965 -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


