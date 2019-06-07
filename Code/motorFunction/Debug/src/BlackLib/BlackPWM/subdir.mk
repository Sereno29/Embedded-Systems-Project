################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackPWM/BlackPWM.cpp 

OBJS += \
./src/BlackLib/BlackPWM/BlackPWM.o 

CPP_DEPS += \
./src/BlackLib/BlackPWM/BlackPWM.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/BlackPWM/%.o: ../src/BlackLib/BlackPWM/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


