################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1171871985:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1171871985-inproc

build-1171871985-inproc: ../Phyn_BLE.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/BLE/ti/xdctools_3_32_00_06_core/xs" --xdcpath="C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/bios_6_46_01_38/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/uia_2_00_06_52/packages;C:/ti/ccs740/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640F128 -r release -c "C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS" --compileOptions "-mv7M3 --code_state=16 -me -O1 --opt_for_speed=0 --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc\" --include_path=\"C:/BLE/WSA/simple_peripheral_cc2650lp_app/PhynSrc\" --include_path=\"C:/ti/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/icall/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/roles\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/dev_info\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/profiles/simple_profile\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/heapmgr\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/controller/cc26xx/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/target\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/hal/src/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/osal/src/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/sdata\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/services/src/saddr\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/components/icall/src/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/inc\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/rom\" --include_path=\"C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393\" --include_path=\"C:/BLE/ti/ble_sdk_2_02_01_18/src/common/cc26xx/rcosc\" --include_path=\"C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages/ti/drivers/timer\" --define=xNO_BOOTLOADER --define=xWAIT_FOR_ID_CHANGE --define=CC26XX --define=CC2650DK_4XS --define=PHYN_PWD --define=USE_RCOSC --define=xTERMINAL_INTERFACE --define=xDEBUG_MODE --define=xBLINKY_TEST --define=WD_ENABLE --define=xTRACK_STATES --define=EXT_HAL_ASSERT --define=BOARD_DISPLAY_EXCLUDE_LCD --define=xBOARD_DISPLAY_EXCLUDE_UART --define=Display_DISABLE_ALL --define=HEAPMGR_SIZE=0 --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=POWER_MEASURE --define=USE_ICALL --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_suppress=48 --diag_suppress=169 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1171871985 ../Phyn_BLE.cfg
configPkg/compiler.opt: build-1171871985
configPkg/: build-1171871985


