################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../server/witness/witness.cc \
../server/witness/witnessFrontEnd.cc 

CC_DEPS += \
./server/witness/witness.d \
./server/witness/witnessFrontEnd.d 

OBJS += \
./server/witness/witness.o \
./server/witness/witnessFrontEnd.o 


# Each subdirectory must supply rules for building sources it contributes
server/witness/%.o: ../server/witness/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


