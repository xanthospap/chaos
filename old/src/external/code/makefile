# --------------------------------------------------------------
# MAKE FILE FOR EXTERNAL UTILITIES LIBRARY
# --------------------------------------------------------------
#
# Note that the source here depends on the libraries:
# datetime, varutils
LINKLIBS := ../../../lib/libdatetime$(VERSION).a \
            ../../../lib/libvutils$(VERSION).a \
#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../../inc/ -L../../../lib/
#
# LIB      = ../../libs/libexternal$(VERSION).a
#
SRC      = bernsta.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../../obj/, $(OBJ_))
#
HEAD     = codeaiub.hpp
#
# --------------------------------------------------------------

install: $(OBJ) link

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(CURDIR)/../../../inc/codeaiub.hpp

link:
	@/bin/ln -nsf $(CURDIR)/codeaiub.hpp $(CURDIR)/../../../inc/codeaiub.hpp

$(OBJ): bernsta.cpp $(HEAD) $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../../obj/bernsta.o bernsta.cpp
