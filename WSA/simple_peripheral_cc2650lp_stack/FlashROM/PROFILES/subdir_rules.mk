################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
PROFILES/gap.obj: D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles/gap.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/build_config.opt"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/rom" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/aes/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/npi/src" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx/npi/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --define=BLE_NO_SECURITY --define=CC26XX --define=CC26XXWARE --define=DATA= --define=EXT_HAL_ASSERT --define=FLASH_ROM_BUILD --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=NEAR_FUNC= --define=OSAL_CBTIMER_NUM_TASKS=1 --define=OSAL_SNV=1 --define=xPOWER_SAVING --define=USE_ICALL --define=xDEBUG --define=xDEBUG_ENC --define=xDEBUG_GPIO --define=xDEBUG_SW_TRACE --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --c99 --diag_suppress=48 --diag_suppress=16004 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="PROFILES/gap.d_raw" --obj_directory="PROFILES" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

PROFILES/gapbondmgr.obj: D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles/gapbondmgr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/build_config.opt"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/rom" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/aes/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/npi/src" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx/npi/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --define=BLE_NO_SECURITY --define=CC26XX --define=CC26XXWARE --define=DATA= --define=EXT_HAL_ASSERT --define=FLASH_ROM_BUILD --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=NEAR_FUNC= --define=OSAL_CBTIMER_NUM_TASKS=1 --define=OSAL_SNV=1 --define=xPOWER_SAVING --define=USE_ICALL --define=xDEBUG --define=xDEBUG_ENC --define=xDEBUG_GPIO --define=xDEBUG_SW_TRACE --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --c99 --diag_suppress=48 --diag_suppress=16004 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="PROFILES/gapbondmgr.d_raw" --obj_directory="PROFILES" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

PROFILES/gattservapp_util.obj: D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/host/gattservapp_util.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/build_config.opt"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/rom" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/aes/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/npi/src" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx/npi/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --define=BLE_NO_SECURITY --define=CC26XX --define=CC26XXWARE --define=DATA= --define=EXT_HAL_ASSERT --define=FLASH_ROM_BUILD --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=NEAR_FUNC= --define=OSAL_CBTIMER_NUM_TASKS=1 --define=OSAL_SNV=1 --define=xPOWER_SAVING --define=USE_ICALL --define=xDEBUG --define=xDEBUG_ENC --define=xDEBUG_GPIO --define=xDEBUG_SW_TRACE --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --c99 --diag_suppress=48 --diag_suppress=16004 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="PROFILES/gattservapp_util.d_raw" --obj_directory="PROFILES" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

PROFILES/sm_ecc.obj: D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/host/sm_ecc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/../../../../../src/config/build_components.opt" --cmd_file="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/examples/cc2650lp/simple_peripheral/ccs/stack/../../iar/stack/build_config.opt"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="D:/Dropbox/Phyn/BLE/App/ccsv7/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/nv" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/rom" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/aes/cc26xx" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/components/npi/src" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx/npi/stack" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="D:/Dropbox/Phyn/BLE/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --define=BLE_NO_SECURITY --define=CC26XX --define=CC26XXWARE --define=DATA= --define=EXT_HAL_ASSERT --define=FLASH_ROM_BUILD --define=GATT_NO_CLIENT --define=INCLUDE_AES_DECRYPT --define=NEAR_FUNC= --define=OSAL_CBTIMER_NUM_TASKS=1 --define=OSAL_SNV=1 --define=xPOWER_SAVING --define=USE_ICALL --define=xDEBUG --define=xDEBUG_ENC --define=xDEBUG_GPIO --define=xDEBUG_SW_TRACE --define=xPM_DISABLE_PWRDOWN --define=xTESTMODES --define=xTEST_BLEBOARD --c99 --diag_suppress=48 --diag_suppress=16004 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="PROFILES/sm_ecc.d_raw" --obj_directory="PROFILES" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


