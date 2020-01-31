################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../External_Interrupts_PBcfg.c \
../External_interrupts.c \
../game.c \
../glcd.c \
../shapes.c \
../shapes_PBcfg.c 

OBJS += \
./External_Interrupts_PBcfg.o \
./External_interrupts.o \
./game.o \
./glcd.o \
./shapes.o \
./shapes_PBcfg.o 

C_DEPS += \
./External_Interrupts_PBcfg.d \
./External_interrupts.d \
./game.d \
./glcd.d \
./shapes.d \
./shapes_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


