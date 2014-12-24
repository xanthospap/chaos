# --------------------------------------------------------------
# MAKE FILE FOR GNSS LIBRARY
# --------------------------------------------------------------

CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/ -L../../lib/
#
LINKLIBS := ../../lib/libvutils$(VERSION).a
#
LIB      = ../../lib/libgnss$(VERSION).a
#
SRC      = gstructs.cpp \
           resolve_v2x.cpp \
           resolve_v3x.cpp \
           gsats.cpp \
           greceiver.cpp \
           gantenna.cpp \
           gsite.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../obj/, $(OBJ_))
#
HEAD     =
#
# --------------------------------------------------------------

test: all

install: print $(LIB) link donem

delete:
	-rm $(OBJ)
	-rm $(LIB)
	-rm $(CURDIR)/../../inc/gnsscon.hpp
	-rm $(CURDIR)/../../inc/gstructs.hpp
	-rm $(CURDIR)/../../inc/gsats.hpp
	-rm $(CURDIR)/../../inc/gobst.hpp

link:
	@echo "... Linking header files ..."
	@/bin/ln -nsf $(CURDIR)/gnsscon.hpp $(CURDIR)/../../inc/gnsscon.hpp
	@/bin/ln -nsf $(CURDIR)/gstructs.hpp $(CURDIR)/../../inc/gstructs.hpp
	@/bin/ln -nsf $(CURDIR)/gsats.hpp $(CURDIR)/../../inc/gsats.hpp
	@/bin/ln -nsf $(CURDIR)/gobst.hpp $(CURDIR)/../../inc/gobst.hpp
	@/bin/ln -nsf $(CURDIR)/gsite.hpp $(CURDIR)/../../inc/gsite.hpp

print:
	@echo ""
	@echo "********************************************************************************"
	@echo " Compiling GNSS library."
	@echo "--------------------------------------------------------------------------------"

donem:
	@echo "--------------------------------------------------------------------------------"
	@echo " done!"
	@echo " Created library: $(LIB)"
	@echo "********************************************************************************"
	@echo ""

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

../../obj/gstructs.o: gstructs.cpp gstructs.hpp
	$(CXX) $(CFLAGS) -o ../../obj/gstructs.o gstructs.cpp
../../obj/resolve_v2x.o: gstructs.cpp gstructs.hpp resolve_v2x.cpp gobst.hpp
	$(CXX) $(CFLAGS) -o ../../obj/resolve_v2x.o resolve_v2x.cpp
../../obj/resolve_v3x.o: gstructs.cpp gstructs.hpp resolve_v3x.cpp gobst.hpp
	$(CXX) $(CFLAGS) -o ../../obj/resolve_v3x.o resolve_v3x.cpp
../../obj/gsats.o: gsats.cpp gsats.hpp gstructs.cpp gstructs.hpp  $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../obj/gsats.o gsats.cpp
../../obj/gobst.o: gobst.cpp gobst.hpp gstructs.cpp gstructs.hpp  $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../obj/gobst.o gobst.cpp
../../obj/greceiver.o: greceiver.cpp gsite.hpp
	$(CXX) $(CFLAGS) -o ../../obj/greceiver.o greceiver.cpp
../../obj/gantenna.o: gantenna.cpp gsite.hpp
	$(CXX) $(CFLAGS) -o ../../obj/gantenna.o gantenna.cpp
../../obj/gsite.o: gsite.cpp gsite.hpp greceiver.cpp gantenna.cpp
	$(CXX) $(CFLAGS) -o ../../obj/gsite.o gsite.cpp
../../obj/fromhstr.o: gobst.cpp gobst.hpp gstructs.cpp gstructs.hpp fromhstr.cpp $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../obj/fromhstr.o fromhstr.cpp
