################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackADC/BlackADC.cpp 

OBJS += \
./src/BlackLib/BlackADC/BlackADC.o 

CPP_DEPS += \
./src/BlackLib/BlackADC/BlackADC.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/BlackADC/%.o: ../src/BlackLib/BlackADC/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


