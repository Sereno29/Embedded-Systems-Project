################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlackLib/BlackDirectory/BlackDirectory.cpp 

OBJS += \
./src/BlackLib/BlackDirectory/BlackDirectory.o 

CPP_DEPS += \
./src/BlackLib/BlackDirectory/BlackDirectory.d 


# Each subdirectory must supply rules for building sources it contributes
src/BlackLib/BlackDirectory/%.o: ../src/BlackLib/BlackDirectory/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


