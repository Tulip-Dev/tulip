TARGET= plugins_test
SRCS=   pluginstest.cpp \
	BasicPluginsTest.cpp

TULIP_CONFIG=$(shell which tulip-config)

ifeq ($?, 1)
TULIP_CONFIG=..\..\..\tulip-config
endif

CXXFLAGS =  -g -Wall `${TULIP_CONFIG} --cxxflags` `cppunit-config --cflags`
#CXXFLAGS = -O2 -DNO_THREAD -DNDEBUG `${TULIP_CONFIG} --cxxflags` `cppunit-config --cflags`
CXX=g++
LDFLAGS= `${TULIP_CONFIG} --lib_tulip` `cppunit-config --libs` -lpthread

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)


clean :
	-rm -f $(TARGET) *.o *.d *.so

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


-include $(DEPS)
