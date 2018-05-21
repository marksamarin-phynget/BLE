#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.M3{1,0,5.2,5
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3.dep
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3: package/package_ti.uia.events.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events/package -fr=./package/lib/lib/release/ti.uia.events/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events/package -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events/package -fr=./package/lib/lib/release/ti.uia.events/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.sem3: package/package_ti.uia.events.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events/package -fr=./package/lib/lib/release/ti.uia.events/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events/package -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events/package -fr=./package/lib/lib/release/ti.uia.events/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3.dep
package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3: UIAAppCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/UIAAppCtx.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAAppCtx.sem3: UIAAppCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAAppCtx.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAAppCtx.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3.dep
package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3: UIAChanCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/UIAChanCtx.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAChanCtx.sem3: UIAChanCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAChanCtx.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAChanCtx.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3.dep
package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3: UIAFrameCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/UIAFrameCtx.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAFrameCtx.sem3: UIAFrameCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAFrameCtx.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAFrameCtx.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3.dep
package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3: UIAThreadCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/UIAThreadCtx.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAThreadCtx.sem3: UIAThreadCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAThreadCtx.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAThreadCtx.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3.dep
package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3.dep: ;
endif

package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3: UIAUserCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c  -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C   -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

package/lib/lib/release/ti.uia.events/UIAUserCtx.sem3: | .interfaces
package/lib/lib/release/ti.uia.events/UIAUserCtx.sem3: UIAUserCtx.c lib/release/ti.uia.events.aem3.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clem3 -n $< ...
	$(ti.targets.arm.elf.M3.rootDir)/bin/armcl -c -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/ti.uia.events -s oem3 $< -C  -n -s --symdebug:none -qq -pdsw225 --endian=little -mv7M3 --abi=eabi -eo.oem3 -ea.sem3  -ms -g  -Dxdc_target_name__=M3 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_5_2_5 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.M3.rootDir)/include  -fs=./package/lib/lib/release/ti.uia.events -fr=./package/lib/lib/release/ti.uia.events
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/ti.uia.events/UIAUserCtx.sem3: export C_DIR=
package/lib/lib/release/ti.uia.events/UIAUserCtx.sem3: PATH:=$(ti.targets.arm.elf.M3.rootDir)/bin/:$(PATH)

clean,em3 ::
	-$(RM) package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3
	-$(RM) package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.sem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAAppCtx.sem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAChanCtx.sem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAFrameCtx.sem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAThreadCtx.sem3
	-$(RM) package/lib/lib/release/ti.uia.events/UIAUserCtx.sem3

lib/release/ti.uia.events.aem3: package/lib/lib/release/ti.uia.events/package/package_ti.uia.events.oem3 package/lib/lib/release/ti.uia.events/UIAAppCtx.oem3 package/lib/lib/release/ti.uia.events/UIAChanCtx.oem3 package/lib/lib/release/ti.uia.events/UIAFrameCtx.oem3 package/lib/lib/release/ti.uia.events/UIAThreadCtx.oem3 package/lib/lib/release/ti.uia.events/UIAUserCtx.oem3 lib/release/ti.uia.events.aem3.mak

clean::
	-$(RM) lib/release/ti.uia.events.aem3.mak
