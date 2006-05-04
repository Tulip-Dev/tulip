SHELL=bash
TARGET= tutorial
SRCS=  tutorial.cpp 
CXXFLAGS = -O3 -Wall -DNDEBUG `xml2-config --cflags`  `../../tulip-config --cxxflags`
CXX=g++
LDFLAGS= `xml2-config --libs` `freetype-config --libs` `../../tulip-config --libs`  

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

clean :
	-rm -f $(TARGET) *.o *.d

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


-include $(DEPS)





