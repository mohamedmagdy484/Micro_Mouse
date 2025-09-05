################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micro_Mouse_Services/micro_mouse.c 

OBJS += \
./Micro_Mouse_Services/micro_mouse.o 

C_DEPS += \
./Micro_Mouse_Services/micro_mouse.d 


# Each subdirectory must supply rules for building sources it contributes
Micro_Mouse_Services/%.o: ../Micro_Mouse_Services/%.c Micro_Mouse_Services/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


