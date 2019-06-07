################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackCore.cpp 

OBJS += \
./src/BlackLib/BlackCore.o 

CPP_DEPS += \
./src/BlackLib/BlackCore.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/%.o: ../src/BlackLib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


