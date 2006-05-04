SHELL=bash

QT_INCLUDES=-I/usr/lib/qt-3.3/include
QT_LIBS=-L/usr/lib/qt-3.3/lib -lqt-mt

CXXFLAGS = -O3 -Wall -DNDEBUG -fPIC -DPIC $(QT_INCLUDES) `xml2-config --cflags`  `../../tulip-config --cxxflags`
CXX=g++
LDFLAGS= -shared `xml2-config --libs` `freetype-config --libs` `../../tulip-config --libs` $(QT_LIBS)

SRCS= $(ARITY_SRCS) $(ARITY_SEGMENT_SRCS) $(SEGMENT_SRCS)

OBJS=$(SRCS:.cpp=.o)

ARITY_TARGET=libarity.so
ARITY_SEGMENT_TARGET=libaritysegment.so
SEGMENT_TARGET=libsegment.so

all: $(ARITY_TARGET) $(ARITY_SEGMENT_TARGET) $(SEGMENT_TARGET)

clean :
	-rm -f $(ARITY_TARGET) $(ARITY_SEGMENT_TARGET) $(SEGMENT_TARGET) *.o *.d

install: all
	install $(ARITY_TARGET) $(ARITY_SEGMENT_TARGET) $(SEGMENT_TARGET) `../../tulip-config --pluginpath`/clustering  

ARITY_SRCS=ArityRecClustering.cpp
ARITY_OBJS=$(ARITY_SRCS:.cpp=.o)
$(ARITY_TARGET): $(ARITY_OBJS)
	$(CXX) $(ARITY_OBJS) -o $@ $(LDFLAGS) $(LIBS) 

ARITY_SEGMENT_SRCS=AritySegmentRecClustering.cpp
ARITY_SEGMENT_OBJS=$(ARITY_SEGMENT_SRCS:.cpp=.o)
$(ARITY_SEGMENT_TARGET): $(ARITY_SEGMENT_OBJS)
	$(CXX) $(ARITY_SEGMENT_OBJS) -o $@ $(LDFLAGS) $(LIBS) 

SEGMENT_SRCS=SegmentRecClustering.cpp
SEGMENT_OBJS=$(SEGMENT_SRCS:.cpp=.o)
$(SEGMENT_TARGET): $(SEGMENT_OBJS)
	$(CXX) $(SEGMENT_OBJS) -o $@ $(LDFLAGS) $(LIBS) 
