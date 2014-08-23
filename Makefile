#*******************************************************************************
# ASTRI - Astrofisica con Specchi a Teccnologia Italiana
# (c) INAF - Instituto Nazionale di Astrofisica, 2012
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
#
#*******************************************************************************
# This Makefile follows ACS Standards (see Makefile(5) for more).
#*******************************************************************************
# REMARKS
#    None
#------------------------------------------------------------------------
# COMMENT
#
# Modules in the various ACS sub-packages.
#
# The following example is for a subsystem called Voyager One
# which is composed of two directories, Alpha and Beta.

# Alpha contains the modules alpha1, alpha2 and alpha3
# Beta continas the moduels beta1, beta2 and beta3
#
# Modify as appropriate to your subsystem

SUBSYSTEM = SORT (UOS v4.0)

MODULES =  ICD EXTERNAL/SimNexstar EXTERNAL/SimLpi \
           CONSOLE DATABASE INSTRUMENT ITS SCHEDULER TELESCOPE

MODULES_TEST = CONSOLE DATABASE INSTRUMENT SCHEDULER TELESCOPE

MAKE_FLAGS = "-k"
PLATFORM := $(shell uname)

SHELL=/bin/ksh
ECHO=echo

ifdef MAKE_VERBOSE
    AT = 
    OUTPUT =
else
    AT = @
    OUTPUT = > /dev/null
endif
#
os     = $(shell uname)
osrev  = $(shell uname -r)

ifeq ($(os),SunOS)
	realtime=YES
endif

#
# This target just forward any make target to all modules
#


define canned
	@$(ECHO) "############ Executing '$@' on all $(SUBSYSTEM) modules #################"
	@for member in  $(foreach name, $(MODULES), $(name) ) ; do \
		    $(ECHO) "############ $${member}" ;\
		    if [ ! -d $${member} ]; then \
                         echo "### ==> $${member} MODULE NOT FOUND! FAILED! " | tee -a build.log;\
                    fi;\
		    if [ -f $${member}/src/Makefile ]; then \
			$(MAKE) $(MAKE_FLAGS) -C $${member}/src/ $@ || break ;\
		    elif [ -f $${member}/ws/src/Makefile ]; then \
			$(MAKE) $(MAKE_FLAGS) -C $${member}/ws/src/ $@ || break ;\
		    fi;\
		    if [ "$(realtime)" == "YES" ]; then \
			if [ -f $${member}/lcu/src/Makefile ]; then \
			$(MAKE) $(MAKE_FLAGS) -C $${member}/lcu/src/ $@ || break ;\
			fi;\
		    fi;\
		done
endef

clean_log:
	@$(ECHO) "############ Clean Build Log File: build.log #################"
	@rm -f build.log
	@touch build.log

#
# building all modules
#
build:	
	@$(ECHO) "############ build $(SUBSYSTEM) Software         #################"| tee -a build.log
	@for member in  $(foreach name, $(MODULES), $(name) ) ; do \
		    if [ ! -d $${member} ]; then \
                         echo "### ==> $${member} MODULE NOT FOUND! FAILED! " | tee -a build.log;\
                    fi;\
		    if [ -f $${member}/src/Makefile ]; then \
		         $(ECHO) "############ $${member} MAIN" | tee -a build.log;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/src/ clean >> build.log 2>& 1;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/src/ all >> build.log 2>& 1 || echo "### ==> FAILED all ! " | tee -a build.log; \
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/src/ install >> build.log 2>& 1 || echo "### ==> FAILED install ! " | tee -a build.log; \
                         continue ;\
		    fi;\
		    if [ -f $${member}/ws/src/Makefile ]; then \
		    $(ECHO) "############ $${member} WS" | tee -a build.log;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/ws/src/ clean >> build.log 2>& 1;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/ws/src/ all >> build.log 2>& 1 || echo "### ==> FAILED all ! " | tee -a build.log; \
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/ws/src/ install >> build.log 2>& 1 || echo "### ==> FAILED install ! " | tee -a build.log; \
		    fi;\
		    if [ "$(realtime)" == "YES" ]; then \
			if [ -f $${member}/lcu/src/Makefile ]; then \
			    $(ECHO) "############ $${member} LCU" | tee -a build.log;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/lcu/src/ clean >> build.log 2>& 1;\
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/lcu/src/ all >> build.log 2>& 1 || echo "### ==> FAILED all ! " | tee -a build.log; \
                         $(MAKE) $(MAKE_FLAGS) -C $${member}/lcu/src/ install >> build.log 2>& 1 || echo "### ==> FAILED install ! " | tee -a build.log; \
			fi;\
		    fi;\
		done
#
# Test target
#

.PHONY: test

Test = test
$(Test):
	@$(ECHO) "############ Clean Test Log File: test.log #################"
	@rm -f test.log
	@touch test.log
	@$(ECHO) "############ TEST $(SUBSYSTEM) Software #################"| tee -a test.log
	@for member in $(foreach name,$(MODULES_TEST),$(name)); do\
		if [ -d $${member}/ws/test ]; then\
			$(ECHO) "############ $${member}/ws/test WS TEST ############" | tee -a test.log ;\
			$(MAKE) -k -C $${member}/ws/test/ $@ | tee -a test.log | grep -E '(Nothing to|FAILED.|PASSED.|Error:)';\
			if [ -d $${member}/lcu/test ]; then\
				$(ECHO) "############ $${member}/lcu/test LCU TEST ############" | tee -a test.log;\
				$(MAKE) -k -C $${member}/lcu/test/ $@ | tee -a test.log | grep -E '(Nothing to|FAILED.|PASSED.|Error:)';\
			fi;\
		elif [ -d $${member}/test ]; then\
			$(ECHO) "############ $${member}/test MAIN TEST ############" | tee -a test.log ;\
			$(MAKE) -k -C $${member}/test/ $@ | tee -a test.log | grep -E '(Nothing to|FAILED.|PASSED.|Error:)';\
		else\
			$(ECHO) "### ==> $${member} TEST DIRECTORY STRUCTURE NOT FOUND! FAILED!" | tee -a test.log ;\
		fi;\
	done
	@$(ECHO) "... done"
#
# Standard canned targets
#
clean:	
	$(canned)
all:	
	$(canned)
install:
	$(canned)
man:
	$(canned)

buildClean: build clean

buildMan: build man

#
# ___oOo___
