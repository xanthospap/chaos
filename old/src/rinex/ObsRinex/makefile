# --------------------------------------------------------------
# MAKE FILE FOR RINEX LIBRARY
# --------------------------------------------------------------
#
# Note that the source here depends on the libraries:
# datetime, varutils
LINKLIBS := ../../../lib/libdatetime$(VERSION).a \
            ../../../lib/libgnss$(VERSION).a
#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/ -L../../../lib/
#
# LIB      = ../../libs/libexternal$(VERSION).a
#
SRC      = rslvrnxver.cpp \
           rdrnxhdr_v2x.cpp \
           rdrnxhdr_v3x.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../../obj/, $(OBJ_))
#
HEAD     = obsrnx.hpp
#
# --------------------------------------------------------------

install: $(OBJ) link

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(CURDIR)/../../../inc/obsrnx.hpp

link:
	@/bin/ln -nsf $(CURDIR)/obsrnx.hpp $(CURDIR)/../../../inc/obsrnx.hpp

../../../obj/rdrnxhdr_v2x.o: rdrnxhdr_v2x.cpp $(HEAD) ../Rinex/rinex.hpp $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../../obj/rdrnxhdr_v2x.o rdrnxhdr_v2x.cpp
../../../obj/rdrnxhdr_v3x.o: rdrnxhdr_v3x.cpp $(HEAD) ../Rinex/rinex.hpp $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../../obj/rdrnxhdr_v3x.o rdrnxhdr_v3x.cpp
../../../obj/rslvrnxver.o: rslvrnxver.cpp $(HEAD) ../Rinex/rinex.hpp $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../../obj/rslvrnxver.o rslvrnxver.cpp