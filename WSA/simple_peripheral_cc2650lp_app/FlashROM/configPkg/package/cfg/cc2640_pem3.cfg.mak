# invoke SourceDir generated makefile for cc2640.pem3
cc2640.pem3: .libraries,cc2640.pem3
.libraries,cc2640.pem3: package/cfg/cc2640_pem3.xdl
	$(MAKE) -f D:\Dropbox\Phyn\BLE\WSA\simple_peripheral_cc2650lp_app/src/makefile.libs

clean::
	$(MAKE) -f D:\Dropbox\Phyn\BLE\WSA\simple_peripheral_cc2650lp_app/src/makefile.libs clean

