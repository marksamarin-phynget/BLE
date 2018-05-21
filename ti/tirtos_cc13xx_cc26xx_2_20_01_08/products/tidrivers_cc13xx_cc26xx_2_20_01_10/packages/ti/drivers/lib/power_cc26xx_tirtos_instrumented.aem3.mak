#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.M3{1,0,5.2,5
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3.dep
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3.dep: ;
endif

package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3: | .interfaces
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3: package/package_ti.drivers.c lib/power_cc26xx_tirtos_instrumented.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/power_cc26xx_tirtos_instrumented/package -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3: export C_DIR=
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.sem3: | .interfaces
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.sem3: package/package_ti.drivers.c lib/power_cc26xx_tirtos_instrumented.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/power_cc26xx_tirtos_instrumented/package -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/package -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.sem3: export C_DIR=
package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3.dep
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3.dep: ;
endif

package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3: | .interfaces
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3: ./power/PowerCC26XX_tirtos.c lib/power_cc26xx_tirtos_instrumented.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/power_cc26xx_tirtos_instrumented/./power -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3: export C_DIR=
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.sem3: | .interfaces
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.sem3: ./power/PowerCC26XX_tirtos.c lib/power_cc26xx_tirtos_instrumented.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/power_cc26xx_tirtos_instrumented/./power -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms --gcc -g -Dccs  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  -I/db/vtree/library/trees/zumaprod/zumaprod-n08/exports/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393 -Dxdc_runtime_Assert_DISABLE_CONDITIONAL_ASSERT -DDebugP_ASSERT_ENABLED -DDebugP_LOG_ENABLED  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power -fr=./package/lib/lib/power_cc26xx_tirtos_instrumented/./power
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.sem3: export C_DIR=
package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

clean,em3 ::
	-$(RM) package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3
	-$(RM) package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3
	-$(RM) package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.sem3
	-$(RM) package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.sem3

lib/power_cc26xx_tirtos_instrumented.aem3: package/lib/lib/power_cc26xx_tirtos_instrumented/package/package_ti.drivers.oem3 package/lib/lib/power_cc26xx_tirtos_instrumented/./power/PowerCC26XX_tirtos.oem3 lib/power_cc26xx_tirtos_instrumented.aem3.mak

clean::
	-$(RM) lib/power_cc26xx_tirtos_instrumented.aem3.mak
