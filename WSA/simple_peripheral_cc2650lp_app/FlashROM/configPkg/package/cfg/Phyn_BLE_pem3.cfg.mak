# invoke SourceDir generated makefile for Phyn_BLE.pem3
Phyn_BLE.pem3: .libraries,Phyn_BLE.pem3
.libraries,Phyn_BLE.pem3: package/cfg/Phyn_BLE_pem3.xdl
	$(MAKE) -f C:\BLE\WSA\simple_peripheral_cc2650lp_app/src/makefile.libs

clean::
	$(MAKE) -f C:\BLE\WSA\simple_peripheral_cc2650lp_app/src/makefile.libs clean

