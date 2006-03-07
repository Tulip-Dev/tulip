SHELL=bash
TARGET= qtviewer
SRCS=   qtviewer.cpp

CXXFLAGS = -DNDEBUG -O3 -funroll-loops -Wall `tulip-config --cxxflags` -I/usr/lib/qt-3.3/include -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED

#CXXFLAGS =-O0 -g -Wall `tulip-config --cxxflags` -I/usr/lib/qt-3.3/include -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED
CXX=g++
LDFLAGS=  -L/usr/lib/qt-3.3/lib -lqui -lqt-mt -L/usr/lib/GL -lGL -lGLU -lglut -lpthread `tulip-config --libs` 

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





