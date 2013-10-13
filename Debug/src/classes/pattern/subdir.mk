################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/classes/pattern/CDelay.cpp \
../src/classes/pattern/CTextPattern.cpp \
../src/classes/pattern/CTokenPattern.cpp \
../src/classes/pattern/libmathing.cpp 

OBJS += \
./src/classes/pattern/CDelay.o \
./src/classes/pattern/CTextPattern.o \
./src/classes/pattern/CTokenPattern.o \
./src/classes/pattern/libmathing.o 

CPP_DEPS += \
./src/classes/pattern/CDelay.d \
./src/classes/pattern/CTextPattern.d \
./src/classes/pattern/CTokenPattern.d \
./src/classes/pattern/libmathing.d 


# Each subdirectory must supply rules for building sources it contributes
src/classes/pattern/%.o: ../src/classes/pattern/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


