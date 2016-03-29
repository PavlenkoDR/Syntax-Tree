################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dif.cpp \
../global.cpp \
../main.cpp \
../numpars.cpp \
../poltreecreate.cpp \
../tree.cpp 

OBJS += \
./dif.o \
./global.o \
./main.o \
./numpars.o \
./poltreecreate.o \
./tree.o 

CPP_DEPS += \
./dif.d \
./global.d \
./main.d \
./numpars.d \
./poltreecreate.d \
./tree.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


