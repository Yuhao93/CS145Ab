################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../src/ \
../src/ASF/ \
../src/ASF/common/ \
../src/ASF/common/boards/ \
../src/ASF/common/utils/ \
../src/ASF/common/utils/interrupt/ \
../src/ASF/common/utils/make/ \
../src/ASF/mega/ \
../src/ASF/mega/boards/ \
../src/ASF/mega/boards/stk600/ \
../src/ASF/mega/boards/stk600/rcx_x/ \
../src/ASF/mega/utils/ \
../src/ASF/mega/utils/assembler/ \
../src/ASF/mega/utils/preprocessor/ \
../src/config/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../src/adxl.c \
../src/avr.c \
../src/ASF/mega/boards/stk600/rcx_x/init.c \
../src/main.c


PREPROCESSING_SRCS +=  \
../src/i2cmaster.S


ASM_SRCS += 


OBJS +=  \
src/adxl.o \
src/avr.o \
src/i2cmaster.o \
src/ASF/mega/boards/stk600/rcx_x/init.o \
src/main.o

OBJS_AS_ARGS +=  \
src/adxl.o \
src/avr.o \
src/i2cmaster.o \
src/ASF/mega/boards/stk600/rcx_x/init.o \
src/main.o

C_DEPS +=  \
src/adxl.d \
src/avr.d \
src/i2cmaster.d \
src/ASF/mega/boards/stk600/rcx_x/init.d \
src/main.d

C_DEPS_AS_ARGS +=  \
src/adxl.d \
src/avr.d \
src/i2cmaster.d \
src/ASF/mega/boards/stk600/rcx_x/init.d \
src/main.d

OUTPUT_FILE_PATH +=Project5Try2.elf

OUTPUT_FILE_PATH_AS_ARGS +=Project5Try2.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler









src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=STK600_MEGA  -I"../common/applications/user_application/atmega32_stk600-mega/config" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/mega/utils" -I"../src/ASF/common/utils" -I"../src/ASF/mega/utils/preprocessor" -I"../src"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g3 -Wall -mmcu=atmega32 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/ASF/mega/boards/stk600/rcx_x/%.o: ../src/ASF/mega/boards/stk600/rcx_x/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -DDEBUG -DBOARD=STK600_MEGA  -I"../common/applications/user_application/atmega32_stk600-mega/config" -I"../src/config" -I"../src/ASF/common/boards" -I"../src/ASF/mega/utils" -I"../src/ASF/common/utils" -I"../src/ASF/mega/utils/preprocessor" -I"../src"  -O1 -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g3 -Wall -mmcu=atmega32 -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
src/i2cmaster.o: ../src/i2cmaster.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega32 -mrelax -DBOARD=STK600_MEGA -I "../common/applications/user_application/atmega32_stk600-mega/config" -I "../src/config" -I "../src/ASF/common/boards" -I "../src/ASF/mega/utils" -I "../src/ASF/common/utils" -I "../src/ASF/mega/utils/preprocessor" -I "../src"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	


src/%.o: ../src/%.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega32 -mrelax -DBOARD=STK600_MEGA -I "../common/applications/user_application/atmega32_stk600-mega/config" -I "../src/config" -I "../src/ASF/common/boards" -I "../src/ASF/mega/utils" -I "../src/ASF/common/utils" -I "../src/ASF/mega/utils/preprocessor" -I "../src"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Project5Try2.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32 -Wl,--relax  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Project5Try2.elf" "Project5Try2.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Project5Try2.elf" "Project5Try2.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Project5Try2.elf" > "Project5Try2.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Project5Try2.elf" "Project5Try2.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1051\avr8-gnu-toolchain\bin\avr-size.exe" "Project5Try2.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Project5Try2.elf" "Project5Try2.a" "Project5Try2.hex" "Project5Try2.lss" "Project5Try2.eep" "Project5Try2.map" "Project5Try2.srec" "Project5Try2.usersignatures"
	