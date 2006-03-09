SHELL=sh
TARGET= qtviewer
SRCS=   qtviewer.cpp

# linux is the default
QT_DIR=/usr/lib/qt-3.3
DEP_LDFLAGS=-L/usr/lib/GL -lGL -lGLU -lpthread

# windows version
#QT_DIR=/c/Qt/3.3.5
#DEP_LDFLAGS=-L/mingw/lib -lopengl32

QT_LIBS=-lqui -lqt-mt

CXXFLAGS = -DNDEBUG -O3 -funroll-loops -Wall `../../tulip-config --cxxflags` -I${QT_DIR}/include -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED

# for debugging purpose
#CXXFLAGS =-O0 -g -Wall `tulip-config --cxxflags` -I${QT_DIR}/include -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED

CXX=g++
LDFLAGS=`../../tulip-config --libs` -L${QT_DIR}/lib ${QT_LIBS} ${DEP_LDFLAGS}

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





