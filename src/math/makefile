# --------------------------------------------------------------
# MAKE FILE FOR MATH LIBRARY
# --------------------------------------------------------------

#
# Define precision
# DT_PRC   = -DDT_PRC_PICO
#

#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/ -L../../lib/

#
LIB      = ../../lib/libgmath$(VERSION).a

#
SRC      = polynev.cpp \
           ratintrp.cpp \
           cspline.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../obj/, $(OBJ_))

#
HEAD     = intext.hpp

# --------------------------------------------------------------

install: print $(LIB) link donem

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(LIB)
	-rm $(CURDIR)/../../inc/intext.hpp

link:
	@echo "... Linking header files ..."
	@/bin/ln -nsf $(CURDIR)/intext.hpp $(CURDIR)/../../inc/intext.hpp

print:
	@echo ""
	@echo "********************************************************************************"
	@echo " Compiling MATH library."
	@echo "--------------------------------------------------------------------------------"

donem:
	@echo "--------------------------------------------------------------------------------"
	@echo " done!"
	@echo " Created library: $(LIB)"
	@echo "********************************************************************************"
	@echo ""

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

../../obj/polynev.o: polynev.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/polynev.o polynev.cpp
../../obj/ratintrp.o: ratintrp.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/ratintrp.o ratintrp.cpp
../../obj/cspline.o: cspline.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/cspline.o cspline.cpp
