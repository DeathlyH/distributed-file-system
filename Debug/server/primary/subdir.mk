################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../server/primary/primary.cc \
../server/primary/primaryFrontEnd.cc 

CC_DEPS += \
./server/primary/primary.d \
./server/primary/primaryFrontEnd.d 

OBJS += \
./server/primary/primary.o \
./server/primary/primaryFrontEnd.o 


# Each subdirectory must supply rules for building sources it contributes
server/primary/%.o: ../server/primary/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


