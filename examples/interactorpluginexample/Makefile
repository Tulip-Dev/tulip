TULIP_CONFIG=tulip-config
TULIP_VERSION=$(shell ${TULIP_CONFIG} --version)
LIB_EXTENSION=$(shell ${TULIP_CONFIG} --pluginextension)
PLUGIN_NAME=InteractorPluginName
TARGET=lib$(PLUGIN_NAME)-$(TULIP_VERSION).$(LIB_EXTENSION)

INSTALL_PATH=$(shell ${TULIP_CONFIG} --pluginpath)/interactors/


UIC=uic
MOC=moc
RCC=rcc

UI= 

MOCS= 

SRCS= InteractorPluginExample.cpp

QRCS= qrc_InteractorPluginRessources.cpp

CXX=g++
CXXFLAGS = -O3 -Wall -DNDEBUG `${TULIP_CONFIG} --plugincxxflags` `${TULIP_CONFIG} --qtincludes` `${TULIP_CONFIG} --cxxflags` -I./
LDFLAGS= `${TULIP_CONFIG} --pluginldflags` `${TULIP_CONFIG} --libs` `${TULIP_CONFIG} --qtlibs` `${TULIP_CONFIG} --gllibs` 

OBJS=$(SRCS:.cpp=.o) $(MOCS:.cpp=.o) $(QRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(UI) $(MOCS) $(OBJS) $(QRCS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

clean :
	rm -f $(TARGET) $(OBJS) $(QRCS) *.d
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
	$(MOC) -o $@ $<

qrc_%.cpp: %.qrc
	$(RCC) -name $(PLUGIN_NAME) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

#-include $(DEPS)
