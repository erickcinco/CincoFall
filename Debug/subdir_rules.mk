################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Game.obj: ../Game.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/CincoFall/image_src" --include_path="C:/Users/erickcinco/git/CincoFall/G8RTOS_Empty_Lab3" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/CincoFall" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Game.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/CincoFall/image_src" --include_path="C:/Users/erickcinco/git/CincoFall/G8RTOS_Empty_Lab3" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/CincoFall" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/CincoFall/image_src" --include_path="C:/Users/erickcinco/git/CincoFall/G8RTOS_Empty_Lab3" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/CincoFall" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/CincoFall/image_src" --include_path="C:/Users/erickcinco/git/CincoFall/G8RTOS_Empty_Lab3" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/CincoFall" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

threads.obj: ../threads.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/CincoFall/image_src" --include_path="C:/Users/erickcinco/git/CincoFall/G8RTOS_Empty_Lab3" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/CincoFall/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/CincoFall" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/CincoFall/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="threads.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


