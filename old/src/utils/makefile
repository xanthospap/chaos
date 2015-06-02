# --------------------------------------------------------------
# MAKE FILE FOR VARIOUS UTILITIES LIBRARY
# --------------------------------------------------------------
#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE)
LFLAGS   = $(C11) -Wall $(DBG_MODE) -I../../inc/ -L../../libs/
#
LIB      = ../../lib/libvutils$(VERSION).a
#
SRC      = splitstr.cpp stripstr.cpp fortran2cpp_sci.cpp rdbckws.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../obj/, $(OBJ_))
#
HEAD     = varutils.hpp
#
# --------------------------------------------------------------

test: all

install: print $(LIB) link donem

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(LIB)
	-rm $(CURDIR)/../../inc/varutils.hpp
	-rm $(CURDIR)/../../inc/cmdutils.hpp

link:
	@echo "... Linking header files ..."
	@/bin/ln -nsf $(CURDIR)/varutils.hpp $(CURDIR)/../../inc/varutils.hpp

print:
	@echo ""
	@echo "********************************************************************************"
	@echo " Compiling UTILITIES library."
	@echo "--------------------------------------------------------------------------------"

donem:
	@echo "--------------------------------------------------------------------------------"
	@echo " done!"
	@echo " Created library: $(LIB)"
	@echo "********************************************************************************"
	@echo ""

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

../../obj/splitstr.o: splitstr.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/splitstr.o splitstr.cpp
../../obj/stripstr.o: stripstr.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/stripstr.o stripstr.cpp
../../obj/fortran2cpp_sci.o: fortran2cpp_sci.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/fortran2cpp_sci.o fortran2cpp_sci.cpp
../../obj/rdbckws.o: rdbckws.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/rdbckws.o rdbckws.cpp