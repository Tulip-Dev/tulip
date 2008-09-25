SHELL=bash
TARGET= libtulip_test
SRCS=   tuliplibtest.cpp \
	ExtendedClusterOperationTest.cpp \
	VectorTest.cpp \
	MatrixTest.cpp \
	TestAlgorithmTest.cpp \
	MutableContainerTest.cpp \
	SelectionProxyTest.cpp \
	MetaGraphProxyTest.cpp \
	SuperGraphTest.cpp \
	TlpImportExportTest.cpp \
	PluginsTest.cpp \
	IdManagerTest.cpp \
	DataSetTest.cpp \
	ObservablePropertyTest.cpp
#	PlanarConMapTest.cpp 



CXXFLAGS =  -g -Wall `../../../tulip-config --cxxflags` `xml2-config --cflags` `cppunit-config --cflags` -I$(QTDIR)/include/
#CXXFLAGS = -O2 -DNO_THREAD -DNDEBUG `../../../tulip-config --cxxflags` `xml2-config --cflags` `cppunit-config --cflags`
CXX=g++
LDFLAGS= `../../../tulip-config --libs` `cppunit-config --libs`

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(TARGET) testPlugin.so testPlugin2.so

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

testPlugin.so:
	$(CXX) $(CXXFLAGS) testPlugin.cpp -fPIC -shared -o testPlugin.so $(LDFLAGS) $(LIBS)

testPlugin2.so:
	$(CXX) $(CXXFLAGS) testPlugin2.cpp -fPIC -shared -o testPlugin2.so $(LDFLAGS) $(LIBS)



clean :
	-rm -f $(TARGET) *.o *.d *.so

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


-include $(DEPS)





