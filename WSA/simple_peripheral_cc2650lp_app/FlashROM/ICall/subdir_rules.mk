################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ICall/icall.obj: C:/BLE/ti/ble_sdk_2_02_01_18/src/components/icall/src/icall.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/src/config/build_components.opt" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/app/../../iar/stack/build_config.opt" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/app/../../iar/stack/../../ccs/config/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O1 --opt_for_speed=0 --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc" --include_path="C:/BLE/WSA/simple_peripheral_cc2650lp_app/PhynSrc" --include_path="C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/icall/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/dev_info" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/heapmgr" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/target" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/sdata" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/rom" --include_path="C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc" --include_path="C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages/ti/drivers/timer" --define=xNO_BOOTLOADER --define=xWAIT_FOR_ID_CHANGE --define=CC26XX --define=CC2650DK_4XS --define=PHYN_PWD --define=USE_RCOSC --define=xTERMINAL_INTERFACE --define=xDEBUG_MODE --define=xBLINKY_TEST --define=WD_ENABLE --define=xTRACK_STATES --define=EXT_HAL_ASSERT --define=BOARD_DISPLAY_EXCLUDE_LCD --define=xBOARD_DISPLAY_EXCLUDE_UART --define=Display_DISABLE_ALL --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=POWER_MEASURE --define=USE_ICALL --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_suppress=48 --diag_suppress=169 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="ICall/icall.d_raw" --obj_directory="ICall" --verbose $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ICall/icall_cc2650.obj: C:/BLE/ti/ble_sdk_2_02_01_18/src/components/icall/src/icall_cc2650.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/src/config/build_components.opt" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/app/../../iar/stack/build_config.opt" --cmd_file="C:/BLE/WSA/../ti/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/app/../../iar/stack/../../ccs/config/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O1 --opt_for_speed=0 --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc" --include_path="C:/BLE/WSA/simple_peripheral_cc2650lp_app/PhynSrc" --include_path="C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/icall/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/dev_info" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/heapmgr" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/target" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/sdata" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/inc" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/rom" --include_path="C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --include_path="C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc" --include_path="C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages/ti/drivers/timer" --define=xNO_BOOTLOADER --define=xWAIT_FOR_ID_CHANGE --define=CC26XX --define=CC2650DK_4XS --define=PHYN_PWD --define=USE_RCOSC --define=xTERMINAL_INTERFACE --define=xDEBUG_MODE --define=xBLINKY_TEST --define=WD_ENABLE --define=xTRACK_STATES --define=EXT_HAL_ASSERT --define=BOARD_DISPLAY_EXCLUDE_LCD --define=xBOARD_DISPLAY_EXCLUDE_UART --define=Display_DISABLE_ALL --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=POWER_MEASURE --define=USE_ICALL --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_suppress=48 --diag_suppress=169 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="ICall/icall_cc2650.d_raw" --obj_directory="ICall" --verbose $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


