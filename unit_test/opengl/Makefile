TULIP_CONFIG=tulip-config
TULIP_VERSION=$(shell ${TULIP_CONFIG} --version)
LIB_EXTENSION=$(shell ${TULIP_CONFIG} --pluginextension)
TARGET=ComparePicture

INSTALL_PATH=$(shell ${TULIP_CONFIG} --pluginpath)

UIC=uic
MOC=moc
RCC=rcc

UI= ComparePictureData.h

MOCS= moc_ComparePictureMainWindow.cpp

SRCS= ComparePicture.cpp ComparePictureMainWindow.cpp


CXX=g++
CXXFLAGS = -g -O3 -Wall -DNDEBUG `${TULIP_CONFIG} --qtincludes` `${TULIP_CONFIG} --cxxflags`  -I./
#CXXFLAGS = -g -Wall `${TULIP_CONFIG} --plugincxxflags` `${TULIP_CONFIG} --qtincludes` `${TULIP_CONFIG} --cxxflags`  -I./ -I/usr/include/libxml2

LDFLAGS= `${TULIP_CONFIG} --qtlibs` `${TULIP_CONFIG} --gllibs` 

OBJS=$(SRCS:.cpp=.o) $(MOCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(UI) $(MOCS) $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean :
	rm -f $(TARGET) *.o *.d
	rm -f $(MOCS)
	rm -f $(UI)

install: all
	cp $(TARGET) $(INSTALL_PATH)


%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< 	

%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

moc_%.cpp: %.h 
	$(MOC)  -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<



