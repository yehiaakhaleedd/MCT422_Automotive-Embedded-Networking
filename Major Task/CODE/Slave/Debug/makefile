################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./tm4c123gh6pm_startup_ccs.obj" \
"./APP/main.obj" \
"./HAL/button.obj" \
"./HAL/led.obj" \
"./MCAL/CAN/can.obj" \
"./MCAL/SPI/spi.obj" \
"./MCAL/Timers/SYSTICK_TIMER/systickTimer.obj" \
"./MCAL/Timers/TIMER0/timer0.obj" \
"./MCAL/Timers/TIMER1/timer1.obj" \
"./MCAL/UART/uart.obj" \
"./OS/scheduler.obj" \
"C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/ccs/Debug/driverlib.lib" \
"../tm4c123gh6pm.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include APP/subdir_vars.mk
-include HAL/subdir_vars.mk
-include MCAL/CAN/subdir_vars.mk
-include MCAL/SPI/subdir_vars.mk
-include MCAL/Timers/SYSTICK_TIMER/subdir_vars.mk
-include MCAL/Timers/TIMER0/subdir_vars.mk
-include MCAL/Timers/TIMER1/subdir_vars.mk
-include MCAL/UART/subdir_vars.mk
-include OS/subdir_vars.mk
-include subdir_rules.mk
-include APP/subdir_rules.mk
-include HAL/subdir_rules.mk
-include MCAL/CAN/subdir_rules.mk
-include MCAL/SPI/subdir_rules.mk
-include MCAL/Timers/SYSTICK_TIMER/subdir_rules.mk
-include MCAL/Timers/TIMER0/subdir_rules.mk
-include MCAL/Timers/TIMER1/subdir_rules.mk
-include MCAL/UART/subdir_rules.mk
-include OS/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Slave.out \

EXE_OUTPUTS__QUOTED += \
"Slave.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Slave.out"

# Tool invocations
Slave.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --check_misra="required,-2.2,-19.3,-19.4,-19.5,-19.6" --misra_advisory=warning --misra_required=warning -z -m"Slave.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/lib" -i"C:/ti/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Slave_linkInfo.xml" --rom_model -o "Slave.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "tm4c123gh6pm_startup_ccs.obj" "APP\main.obj" "HAL\button.obj" "HAL\led.obj" "MCAL\CAN\can.obj" "MCAL\SPI\spi.obj" "MCAL\Timers\SYSTICK_TIMER\systickTimer.obj" "MCAL\Timers\TIMER0\timer0.obj" "MCAL\Timers\TIMER1\timer1.obj" "MCAL\UART\uart.obj" "OS\scheduler.obj" 
	-$(RM) "tm4c123gh6pm_startup_ccs.d" "APP\main.d" "HAL\button.d" "HAL\led.d" "MCAL\CAN\can.d" "MCAL\SPI\spi.d" "MCAL\Timers\SYSTICK_TIMER\systickTimer.d" "MCAL\Timers\TIMER0\timer0.d" "MCAL\Timers\TIMER1\timer1.d" "MCAL\UART\uart.d" "OS\scheduler.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

