################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../server/backup/backup.cc \
../server/backup/backupFrontEnd.cc 

CC_DEPS += \
./server/backup/backup.d \
./server/backup/backupFrontEnd.d 

OBJS += \
./server/backup/backup.o \
./server/backup/backupFrontEnd.o 


# Each subdirectory must supply rules for building sources it contributes
server/backup/%.o: ../server/backup/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


