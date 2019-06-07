################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackMutex/BlackMutex.cpp 

OBJS += \
./src/BlackLib/BlackMutex/BlackMutex.o 

CPP_DEPS += \
./src/BlackLib/BlackMutex/BlackMutex.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/BlackMutex/%.o: ../src/BlackLib/BlackMutex/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


