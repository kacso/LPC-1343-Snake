################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc13.c \
../src/main.c 

OBJS += \
./src/cr_startup_lpc13.o \
./src/main.o 

C_DEPS += \
./src/cr_startup_lpc13.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC13xx -I"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\CMSISv2p00_LPC13xx" -I"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\CMSISv2p00_LPC13xx\inc" -I"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\Lib_EaBaseBoard\inc" -I"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\Lib_MCU\inc" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


