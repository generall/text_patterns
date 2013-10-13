################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/classes/text_representation/CDict.cpp \
../src/classes/text_representation/CPunctuation.cpp \
../src/classes/text_representation/CText.cpp \
../src/classes/text_representation/CToken.cpp \
../src/classes/text_representation/CWord.cpp 

OBJS += \
./src/classes/text_representation/CDict.o \
./src/classes/text_representation/CPunctuation.o \
./src/classes/text_representation/CText.o \
./src/classes/text_representation/CToken.o \
./src/classes/text_representation/CWord.o 

CPP_DEPS += \
./src/classes/text_representation/CDict.d \
./src/classes/text_representation/CPunctuation.d \
./src/classes/text_representation/CText.d \
./src/classes/text_representation/CToken.d \
./src/classes/text_representation/CWord.d 


# Each subdirectory must supply rules for building sources it contributes
src/classes/text_representation/%.o: ../src/classes/text_representation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


