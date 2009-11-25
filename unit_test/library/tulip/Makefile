TARGET= libtulip_test
SRCS=   tuliplibtest.cpp \
	ExtendedClusterOperationTest.cpp \
	VectorTest.cpp \
	MatrixTest.cpp \
	TestAlgorithmTest.cpp \
	SelectionProxyTest.cpp \
	MetaGraphProxyTest.cpp \
	SuperGraphTest.cpp \
	TlpImportExportTest.cpp \
	PluginsTest.cpp \
	IdManagerTest.cpp \
	DataSetTest.cpp \
	ObservablePropertyTest.cpp \
	MutableContainerTest.cpp \
	ObservableGraphTest.cpp \
	PushPopTest.cpp \
	IntegerPropertyMinMaxSubgraphTest.cpp \
	IntegerPropertyMinMaxUpdateTest.cpp
#	PlanarConMapTest.cpp

TULIP_CONFIG=$(shell which tulip-config)

ifeq ($?, 1)
TULIP_CONFIG=../../../tulip-config
endif

CXXFLAGS =  -g -Wall `${TULIP_CONFIG} --cxxflags` `cppunit-config --cflags`
#CXXFLAGS = -O2 -DNO_THREAD -DNDEBUG `${TULIP_CONFIG} --cxxflags` `cppunit-config --cflags`
CXX=g++
LDFLAGS= `${TULIP_CONFIG} --lib_tulip` `cppunit-config --libs`

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(TARGET) testPlugin.so testPlugin2.so

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

testPlugin.so:
	$(CXX) $(CXXFLAGS) testPlugin.cpp -o testPlugin.so `${TULIP_CONFIG} --plugincxxflags --pluginldflags --lib_tulip`

testPlugin2.so:
	$(CXX) $(CXXFLAGS) testPlugin2.cpp -o testPlugin2.so `${TULIP_CONFIG} --plugincxxflags --pluginldflags --lib_tulip`



clean :
	-rm -f $(TARGET) *.o *.d *.so

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


-include $(DEPS)





