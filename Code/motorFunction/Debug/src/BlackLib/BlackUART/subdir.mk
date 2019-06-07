################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackUART/BlackUART.cpp 

OBJS += \
./src/BlackLib/BlackUART/BlackUART.o 

CPP_DEPS += \
./src/BlackLib/BlackUART/BlackUART.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/BlackUART/%.o: ../src/BlackLib/BlackUART/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


