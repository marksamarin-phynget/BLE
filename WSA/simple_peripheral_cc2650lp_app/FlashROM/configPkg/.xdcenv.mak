#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/bios_6_46_01_38/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/uia_2_00_06_52/packages;C:/BLE/App/ccsv7/ccs_base;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08
override XDCROOT = D:/Dropbox/Phyn/BLE/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/tidrivers_cc13xx_cc26xx_2_20_01_10/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/bios_6_46_01_38/packages;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/uia_2_00_06_52/packages;C:/BLE/App/ccsv7/ccs_base;D:/Dropbox/Phyn/BLE/ti/tirtos_cc13xx_cc26xx_2_20_01_08;D:/Dropbox/Phyn/BLE/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
