# --------------------------------------------------------------
# MAKE FILE FOR PROJECT CHAOS-NGP
# --------------------------------------------------------------
#
include Makefile.conf
#
DIRS = src/utils \
       src/math \
       src/datetime \
       src/geodesy \
       src/external \
       src/gnss \
       src/math \
       src/rinex
#
TESTD = debug
#
NGPD = ngp
#
MKDIR_P = mkdir -p
#
OUT_DIR = lib obj bin
#
# the sets of directories to do various things in
INSTALLDIRS = $(DIRS:%=install-%)
REMOVEDIRS  = $(DIRS:%=uninstall-%)
TESTDIRS    = $(TESTD:%=test-%)
NGPDIRS     = $(NGPD:%=ngp-%)
#
install: $(INSTALLDIRS) directories
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install
	@(if [ $$? -eq 0 ] ; then echo "COMPILATION SUCESEFUL! WELL DONE" ; fi)
#
uninstall: $(REMOVEDIRS)
$(REMOVEDIRS):
	$(MAKE) -C $(@:uninstall-%=%) delete
#
test: $(TESTDIRS)
$(TESTDIRS):
	$(MAKE) -C $(@:test-%=%) test
#
ngp: $(NGPDIRS)
$(NGPDIRS):
	$(MAKE) -C $(@:ngp-%=%) ngp
#
directories: ${OUT_DIR}
#
${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}
#
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(REMOVEDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(NGPDIRS)
.PHONY: directories
#
.PHONY: install uninstall test ngp directories
