# --------------------------------------------------------------
# MAKE FILE FOR RINEX LIBRARY CHAOS-NGP
# --------------------------------------------------------------
#
DIRS = Rinex \
       ObsRinex
#
OBJ = ../../obj/rinex.o \
      ../../obj/rslvrnxver.o \
      ../../obj/rdrnxhdr_v2x.o \
      ../../obj/rdrnxhdr_v3x.o
#
LIB = ../../lib/librinex$(VERSION).a
#
# the sets of directories to do various things in
INSTALLDIRS = $(DIRS:%=install-%)
REMOVEDIRS  = $(DIRS:%=uninstall-%)
TESTDIRS    = $(TESTD:%=test-%)
NGPDIRS     = $(NGPD:%=ngp-%)
#
install: $(INSTALLDIRS) $(LIB)
	ar rc $(LIB) $(OBJ)

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
