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
	arm-none-eabi-gcc -DARM_MATH_CM4 -DTARGET_IS_TM4C123_RB1 -DTARGET_IS_BLIZZARD_RB1 -DPART_TM4C123GH6PM -I/home/aignacio/tools/tivaware -I/home/aignacio/tools/tivaware/inc -I/home/aignacio/tools/tiva_arm_template/lib -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


