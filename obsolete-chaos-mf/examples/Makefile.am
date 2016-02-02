bin_PROGRAMS = testReceiver testSatSys

testReceiver_SOURCES = test_receiver.cpp
testReceiver_CXXFLAGS = -std=c++11 -g -Wall -Wextra -pedantic -W -Wshadow -Winline -Wdisabled-optimization -I$(top_srcdir)/src -L$(top_srcdir)/src
testReceiver_LDADD = $(top_srcdir)/src/libngpt.a

# bin_PROGRAMS = testSatSys
testSatSys_SOURCES = test_satsys.cpp
testSatSys_CXXFLAGS = -std=c++11 -g -Wall -Wextra -pedantic -W -Wshadow -Winline -Wdisabled-optimization -I$(top_srcdir)/src -L$(top_srcdir)/src
testSatSys_LDADD = $(top_srcdir)/src/libngpt.a
