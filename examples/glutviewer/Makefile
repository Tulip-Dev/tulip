SHELL=bash
TARGET= glutviewer
SRCS=   glutviewer.cpp

CXXFLAGS = -DNDEBUG -O3 -funroll-loops -Wall `../../tulip-config --cxxflags` `xml2-config --libs`
CXX=g++
LDFLAGS=  -L/usr/lib/GL -lGL -lGLU -lglut `../../tulip-config --libs`

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





