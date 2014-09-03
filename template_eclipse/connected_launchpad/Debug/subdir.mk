################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cgifuncs.c \
../io.c \
../io_fs.c \
../main.c \
../startup_gcc.c 

OBJS += \
./cgifuncs.o \
./io.o \
./io_fs.o \
./main.o \
./startup_gcc.o 

C_DEPS += \
./cgifuncs.d \
./io.d \
./io_fs.d \
./main.d \
./startup_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DPART_TM4C1294NCPDT -DTARGET_IS_TM4C129_RA0 -I/home/aignacio/tools/tivaware -I/home/aignacio/tools/tivaware/examples/boards/ek-tm4c1294xl/drivers -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1/apps/httpserver_raw -I/home/aignacio/tools/tivaware/utils -I/home/aignacio/tools/tivaware/examples/boards/ek-tm4c1294xl/drivers -I/home/aignacio/tools/tivaware/examples/boards -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1 -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1/apps -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1/ports/tiva-tm4c129/include -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1/src/include -I/home/aignacio/tools/tivaware/third_party/lwip-1.4.1/src/include/ipv4 -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


