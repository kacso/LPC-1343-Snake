################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: My\ Project\ (6.2)\ -\ Snake.axf

dependents:
	-cd C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/CMSISv2p00_LPC13xx/Release && $(MAKE) all
	-cd C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_EaBaseBoard/Release && $(MAKE) all
	-cd C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_MCU/Release && $(MAKE) all

# Tool invocations
My\ Project\ (6.2)\ -\ Snake.axf: $(OBJS) $(USER_OBJS) C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/CMSISv2p00_LPC13xx/Release/libCMSISv2p00_LPC13xx.a C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_EaBaseBoard/Release/libLib_EaBaseBoard.a C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_MCU/Release/libLib_MCU.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\CMSISv2p00_LPC13xx\Release" -L"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\Lib_EaBaseBoard\Release" -L"C:\Users\Danijel\Documents\LPCXpresso_4.2.3_292\workspace\Lib_MCU\Release" -Xlinker -Map="My Project (6.2) - Snake.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "My_Project_6_-_Snake_Release.ld" -o"My Project (6.2) - Snake.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) "My Project (6.2) - Snake.axf"
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "My Project (6.2) - Snake.axf"; # arm-none-eabi-objcopy -O binary "My Project (6.2) - Snake.axf" "My Project (6.2) - Snake.bin" ; checksum -p LPC1343 -d "My Project (6.2) - Snake.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/CMSISv2p00_LPC13xx/Release/libCMSISv2p00_LPC13xx.a:
C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_EaBaseBoard/Release/libLib_EaBaseBoard.a:
C:/Users/Danijel/Documents/LPCXpresso_4.2.3_292/workspace/Lib_MCU/Release/libLib_MCU.a:

-include ../makefile.targets
