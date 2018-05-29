#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/bios_6_46_01_38/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/uia_2_00_06_52/packages;C:/ti/ccs740/ccsv7/ccs_base;D:/GitHub/BLE_Reload/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08
override XDCROOT = C:/BLE/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/bios_6_46_01_38/packages;C:/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/uia_2_00_06_52/packages;C:/ti/ccs740/ccsv7/ccs_base;D:/GitHub/BLE_Reload/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08;C:/BLE/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
