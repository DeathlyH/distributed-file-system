################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../server/primary_server_down_main.cc \
../server/server_main.cc \
../server/server_view_change_main.cc 

CC_DEPS += \
./server/primary_server_down_main.d \
./server/server_main.d \
./server/server_view_change_main.d 

OBJS += \
./server/primary_server_down_main.o \
./server/server_main.o \
./server/server_view_change_main.o 


# Each subdirectory must supply rules for building sources it contributes
server/%.o: ../server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


