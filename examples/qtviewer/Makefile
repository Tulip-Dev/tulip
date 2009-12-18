SHELL=sh
TARGET= qtviewer
SRCS=   qtviewer.cpp

CXXFLAGS = -DNDEBUG -O3 -funroll-loops -Wall `tulip-config --glincludes` `tulip-config --qtincludes` `tulip-config --cxxflags` -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED

# for debugging purpose
#CXXFLAGS =-O0 -g -Wall `../../tulip-config --qtincludes` `../../tulip-config --cxxflags` -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED

CXX=g++
LDFLAGS=`tulip-config --libs` `tulip-config --qtlibs` `tulip-config --gllibs`

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
