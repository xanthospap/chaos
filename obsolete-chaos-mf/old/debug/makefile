# --------------------------------------------------------------
# MAKE FILE FOR DEBUGING ROUTINES
# --------------------------------------------------------------
#
LINKLIBS :=  rinex$(VERSION) \
             external$(VERSION) \
             gnss$(VERSION) \
             datetime$(VERSION) \
             vutils$(VERSION) 
#	     rinex$(VERSION) \
#             igs$(VERSION) \
#             external$(VERSION) \
#             iostructs$(VERSION) \
#             gnss$(VERSION) \
#             iers$(VERSION) \
#             datetime$(VERSION) \
#             geodesy$(VERSION) \
#             gmath$(VERSION) \
#             vutils$(VERSION)
#
DLIBS_     = $(addprefix ../lib/lib, $(LINKLIBS))
DLIBS      = $(addsuffix .a, $(DLIBS_))
LLIBS      = $(addprefix -l, $(LINKLIBS))
#
LFLAGS   = $(C11) -Wall $(DBG_MODE) $(DT_PRC) -I../inc/ -L../lib/
#
#
SRC      = test_rinex.cpp
#
EXE_    := $(SRC:%.cpp=%.e)
EXE      = $(addprefix ../bin/, $(EXE_))
#
# --------------------------------------------------------------

test :$(EXE) # $(DLIBS)

../bin/test_datetime.e: test_datetime.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_datetime.e test_datetime.cpp $(LLIBS) $(DT_PRC)
../bin/test_geodesy.e: test_geodesy.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_geodesy.e test_geodesy.cpp $(LLIBS) 
../bin/test_rinex.e: test_rinex.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_rinex.e test_rinex.cpp $(LLIBS) 
../bin/test_varutils.e: test_varutils.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_varutils.e test_varutils.cpp $(LLIBS) 
../bin/test_gnssdio.e: test_gnssdio.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_gnssdio.e test_gnssdio.cpp $(LLIBS)
../bin/test_external.e: test_external.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_external.e test_external.cpp $(LLIBS) $(DT_PRC)
../bin/test_sp3.e: test_sp3.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_sp3.e test_sp3.cpp $(LLIBS)
../bin/test_iers.e: test_iers.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_iers.e test_iers.cpp $(LLIBS)
../bin/test_data.e: test_data.cpp $(DLIBS)
	$(CXX) $(LFLAGS) -o ../bin/test_data.e test_data.cpp $(LLIBS)
