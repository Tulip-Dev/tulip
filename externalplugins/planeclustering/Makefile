SHELL=bash

TARGET=libplaneclustering.so
SRCS=  PlaneClustering.cpp
CXXFLAGS = -O3 -Wall -DNDEBUG -fPIC -DPIC `xml2-config --cflags`  `../../tulip-config --cxxflags`
CXX=g++
LDFLAGS= -shared `xml2-config --libs` `freetype-config --libs` `../../tulip-config --libs`  

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS) 

clean :
	-rm -f $(TARGET) *.o *.d

install: all
	install $(TARGET) `tulip-config --pluginpath`/clustering  

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


-include $(DEPS)





