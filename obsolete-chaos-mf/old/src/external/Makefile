# --------------------------------------------------------------
# MAKE FILE FOR EXTERNAL CHAOS-NGP
# --------------------------------------------------------------
#
DIRS = code
#
TESTD = debug
#
OBJ = ../../obj/bernsta.o
#
LIB = ../../lib/libexternal$(VERSION).a
#
# the sets of directories to do various things in
INSTALLDIRS = $(DIRS:%=install-%)
REMOVEDIRS  = $(DIRS:%=uninstall-%)
TESTDIRS    = $(TESTD:%=test-%)
NGPDIRS     = $(NGPD:%=ngp-%)
#
install: $(INSTALLDIRS) $(LIB)

$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install
#
uninstall: $(REMOVEDIRS)

$(REMOVEDIRS):
	$(MAKE) -C $(@:uninstall-%=%) delete
#
test: $(TESTDIRS)

$(TESTDIRS):
	$(MAKE) -C $(@:test-%=%) test

$(LIB) : $(OBJ)
	ar rc $(LIB) $(OBJ)
#
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(REMOVEDIRS)
.PHONY: subdirs $(TESTDIRS)
#
.PHONY: install uninstall test
