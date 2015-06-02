# --------------------------------------------------------------
# MAKE FILE FOR RINEX LIBRARY
# --------------------------------------------------------------
#
# Note that the source here depends on the libraries:
# datetime, varutils
LINKLIBS := ../../../lib/libdatetime$(VERSION).a \
            ../../../lib/libgnss$(VERSION).a \
#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/ -L../../../lib/
#
# LIB      = ../../libs/libexternal$(VERSION).a
#
SRC      = rinex.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../../obj/, $(OBJ_))
#
HEAD     = rinex.hpp
#
# --------------------------------------------------------------

install: $(OBJ) link

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(CURDIR)/../../../inc/rinex.hpp

link:
	@/bin/ln -nsf $(CURDIR)/rinex.hpp $(CURDIR)/../../../inc/rinex.hpp

$(OBJ): rinex.cpp $(HEAD) $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../../obj/rinex.o rinex.cpp
