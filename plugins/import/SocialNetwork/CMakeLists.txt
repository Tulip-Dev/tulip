INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})        

ADD_CORE_FILES(CMakeLists.txt *.cpp)

ADD_LIBRARY(AttractAndIntroduce-${Tulip_VERSION} SHARED AttractAndIntroduce.cpp)
TARGET_LINK_LIBRARIES(AttractAndIntroduce-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(AttractAndIntroduce-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(Bollobas-${Tulip_VERSION} SHARED BollobasModel.cpp)
TARGET_LINK_LIBRARIES(Bollobas-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Bollobas-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(BuWangZhou-${Tulip_VERSION} SHARED BuWangZhouModel.cpp)
TARGET_LINK_LIBRARIES(BuWangZhou-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(BuWangZhou-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(Cantazaro-${Tulip_VERSION} SHARED Catanzaro.cpp)
TARGET_LINK_LIBRARIES(Cantazaro-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Cantazaro-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(Fuliao-${Tulip_VERSION} SHARED FuLiao.cpp)
TARGET_LINK_LIBRARIES(Fuliao-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Fuliao-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(GuillaumeLatapy-${Tulip_VERSION} SHARED GuillaumeLatapyModel.cpp)
TARGET_LINK_LIBRARIES(GuillaumeLatapy-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(GuillaumeLatapy-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(HolmeKim-${Tulip_VERSION} SHARED HolmeKim.cpp)
TARGET_LINK_LIBRARIES(HolmeKim-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(HolmeKim-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(KlemmEguiluz-${Tulip_VERSION} SHARED KlemmEguiluzModel.cpp)
TARGET_LINK_LIBRARIES(KlemmEguiluz-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(KlemmEguiluz-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(LiuEtAl-${Tulip_VERSION} SHARED LiuEtAl.cpp)
TARGET_LINK_LIBRARIES(LiuEtAl-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(LiuEtAl-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangEtAl-${Tulip_VERSION} SHARED WangEtAl.cpp)
TARGET_LINK_LIBRARIES(WangEtAl-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WangEtAl-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangRong-${Tulip_VERSION} SHARED WangRong.cpp)
TARGET_LINK_LIBRARIES(WangRong-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WangRong-${Tulip_VERSION} ${TulipPluginsInstallDir})

ADD_LIBRARY(WattsStrotgatz-${Tulip_VERSION} SHARED WattsStrotgatzModel.cpp)
TARGET_LINK_LIBRARIES(WattsStrotgatz-${Tulip_VERSION} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WattsStrotgatz-${Tulip_VERSION} ${TulipPluginsInstallDir})
