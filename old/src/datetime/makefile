# --------------------------------------------------------------
# MAKE FILE FOR DATETIME LIBRARY
# --------------------------------------------------------------

# --------------------------------------------------------------
# DEPENDANCIES :
# --------------------------------------------------------------
# The function splitstr() from library varutils is needed for the
# fromymdstr.cpp file.
#
LINKLIBS := ../../lib/libvutils$(VERSION).a \

#
# Define precision
# DT_PRC   = -DDT_PRC_PICO
#

#
CFLAGS   = -c $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../../inc/ -L../../lib/

#
LIB      = ../../lib/libdatetime$(VERSION).a

#
SRC      = cal2jd.cpp \
           d2dtf.cpp \
           d2tf.cpp \
           dat.cpp \
           dtf2d.cpp \
           jd2cal.cpp \
           jdcalf.cpp \
           tf2d.cpp \
           dctrs.cpp \
           fromymdstr.cpp \
           toymdstr.cpp \
           rearange.cpp \
           dseconds.cpp \
           strconv.cpp \
           setnow.cpp
#
OBJ_    := $(SRC:%.cpp=%.o)
OBJ      = $(addprefix ../../obj/, $(OBJ_))

#
HEAD     = datetime.hpp ../../inc/gencon.hpp

# --------------------------------------------------------------

install: print $(LIB) link donem

clean:
	-rm $(OBJ)

delete:
	-rm $(OBJ)
	-rm $(LIB)
	-rm $(CURDIR)/../../inc/datetime.hpp
	-rm $(CURDIR)/../../inc/dates.hpp

link:
	@echo "... Linking header files ..."
	@/bin/ln -nsf $(CURDIR)/datetime.hpp $(CURDIR)/../../inc/datetime.hpp
	@/bin/ln -nsf $(CURDIR)/dates.hpp    $(CURDIR)/../../inc/dates.hpp

print:
	@echo ""
	@echo "********************************************************************************"
	@echo " Compiling DATETIME library."
	@echo "--------------------------------------------------------------------------------"

donem:
	@echo "--------------------------------------------------------------------------------"
	@echo " done!"
	@echo " Created library: $(LIB)"
	@echo "********************************************************************************"
	@echo ""

$(LIB): $(OBJ)
	ar rc $(LIB) $(OBJ)

../../obj/cal2jd.o: cal2jd.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/cal2jd.o cal2jd.cpp
../../obj/d2dtf.o: d2dtf.cpp jd2cal.cpp dat.cpp d2tf.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/d2dtf.o d2dtf.cpp
../../obj/d2tf.o: d2tf.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/d2tf.o d2tf.cpp
../../obj/dat.o: dat.cpp cal2jd.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/dat.o dat.cpp
../../obj/dtf2d.o: dtf2d.cpp cal2jd.cpp dat.cpp jd2cal.cpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/dtf2d.o dtf2d.cpp
../../obj/jd2cal.o: jd2cal.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/jd2cal.o jd2cal.cpp
../../obj/jdcalf.o: jdcalf.cpp jd2cal.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/jdcalf.o jdcalf.cpp
../../obj/tf2d.o: tf2d.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/tf2d.o tf2d.cpp
../../obj/dctrs.o: dctrs.cpp dtf2d.cpp cal2jd.cpp dat.cpp jd2cal.cpp dates.hpp rearange.cpp $(HEAD) $(LINKLIBS)
	$(CXX) $(CFLAGS) -o ../../obj/dctrs.o dctrs.cpp
../../obj/fromymdstr.o: fromymdstr.cpp dtf2d.cpp cal2jd.cpp dat.cpp jd2cal.cpp dates.hpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/fromymdstr.o fromymdstr.cpp
../../obj/toymdstr.o: toymdstr.cpp d2dtf.cpp cal2jd.cpp dat.cpp jd2cal.cpp dates.hpp $(HEAD)
	$(CXX) $(CFLAGS) -o ../../obj/toymdstr.o toymdstr.cpp
../../obj/rearange.o: rearange.cpp dates.hpp
	$(CXX) $(CFLAGS) -o ../../obj/rearange.o rearange.cpp
../../obj/dseconds.o: dseconds.cpp dates.hpp rearange.cpp $(HEAD) 
	$(CXX) $(CFLAGS) -o ../../obj/dseconds.o dseconds.cpp
../../obj/strconv.o: strconv.cpp dates.hpp
	$(CXX) $(CFLAGS) -o ../../obj/strconv.o strconv.cpp
../../obj/setnow.o: setnow.cpp dates.hpp
	$(CXX) $(CFLAGS) -o ../../obj/setnow.o setnow.cpp
