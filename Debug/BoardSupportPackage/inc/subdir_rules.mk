################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BoardSupportPackage/inc/RGBLeds.obj: ../BoardSupportPackage/inc/RGBLeds.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/Users/erickcinco/git/MicroP2/CC3100SupportPackage/SL_Common" --include_path="C:/Users/erickcinco/git/MicroP2/CC3100SupportPackage/spi_cc3100" --include_path="C:/Users/erickcinco/git/MicroP2/CC3100SupportPackage/board" --include_path="C:/Users/erickcinco/git/MicroP2/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/erickcinco/git/MicroP2/CC3100SupportPackage/simplelink/include" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/erickcinco/git/MicroP2" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/Users/erickcinco/git/MicroP2/BoardSupportPackage/inc" --include_path="C:/Users/erickcinco/git/MicroP2/BoardSupportPackage/src" --include_path="C:/Users/erickcinco/git/MicroP2/BoardSupportPackage/DriverLib" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="BoardSupportPackage/inc/RGBLeds.d_raw" --obj_directory="BoardSupportPackage/inc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


