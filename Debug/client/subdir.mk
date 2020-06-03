################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../client/client-main.cc \
../client/client.cc \
../client/test_dfs.cc 

O_SRCS += \
../client/backupFrontEnd.o \
../client/common.o \
../client/primaryFrontEnd.o \
../client/test_dfs.o \
../client/witnessFrontEnd.o 

CC_DEPS += \
./client/client-main.d \
./client/client.d \
./client/test_dfs.d 

OBJS += \
./client/client-main.o \
./client/client.o \
./client/test_dfs.o 


# Each subdirectory must supply rules for building sources it contributes
client/%.o: ../client/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


