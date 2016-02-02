# --------------------------------------------------------------
# MAKE FILE FOR GEODESY LIBRARY
# --------------------------------------------------------------
#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE)
LFLAGS   = $(C11) -Wall $(DBG_MODE) -I../../include/ -L../../libs/
#
LIB      = ../../lib/libgeodesy$(VERSION).a
#
SRC      = car2ell.cpp datums.cpp ell2car.cpp topocentric.cpp topsaz.cpp anp.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../obj/, $(OBJ_))
#
HEAD     = datums.hpp gencon.hpp geodesy.hpp
#
# --------------------------------------------------------------

install: print $(LIB) link donem

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(LIB)
	-rm $(CURDIR)/../../inc/datums.hpp
	-rm $(CURDIR)/../../inc/geodesy.hpp

link:
	@echo "... Linking header files ..."
	@/bin/ln -nsf $(CURDIR)/datums.hpp $(CURDIR)/../../inc/datums.hpp
	@/bin/ln -nsf $(CURDIR)/geodesy.hpp $(CURDIR)/../../inc/geodesy.hpp

print:
	@echo ""
	@echo "********************************************************************************"
	@echo " Compiling GEODESY library."
	@echo "--------------------------------------------------------------------------------"

donem:
	@echo "--------------------------------------------------------------------------------"
	@echo " done!"
	@echo " Created library: $(LIB)"
	@echo "********************************************************************************"
	@echo ""

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

../../obj/car2ell.o: car2ell.cpp $(HEAD) datums.?pp
	$(CXX) $(CFLAGS) -o ../../obj/car2ell.o car2ell.cpp
../../obj/ell2car.o: ell2car.cpp $(HEAD) datums.?pp
	$(CXX) $(CFLAGS) -o ../../obj/ell2car.o ell2car.cpp
../../obj/datums.o: datums.cpp datums.hpp
	$(CXX) $(CFLAGS) -o ../../obj/datums.o datums.cpp
../../obj/topocentric.o: topocentric.cpp $(HEAD) datums.?pp car2ell.cpp
	$(CXX) $(CFLAGS) -o ../../obj/topocentric.o topocentric.cpp
../../obj/topsaz.o: topsaz.cpp $(HEAD) datums.?pp 
	$(CXX) $(CFLAGS) -o ../../obj/topsaz.o topsaz.cpp
../../obj/anp.o: anp.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/anp.o anp.cpp
