INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})        

ADD_CORE_FILES(CMakeLists.txt *.cpp)

ADD_LIBRARY(AttractAndIntroduce-${TulipVersion} SHARED AttractAndIntroduce.cpp)
TARGET_LINK_LIBRARIES(AttractAndIntroduce-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(AttractAndIntroduce-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Bollobas-${TulipVersion} SHARED BollobasModel.cpp)
TARGET_LINK_LIBRARIES(Bollobas-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Bollobas-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(BuWangZhou-${TulipVersion} SHARED BuWangZhouModel.cpp)
TARGET_LINK_LIBRARIES(BuWangZhou-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(BuWangZhou-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Cantazaro-${TulipVersion} SHARED Catanzaro.cpp)
TARGET_LINK_LIBRARIES(Cantazaro-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Cantazaro-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Fuliao-${TulipVersion} SHARED FuLiao.cpp)
TARGET_LINK_LIBRARIES(Fuliao-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(Fuliao-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(GuillaumeLatapy-${TulipVersion} SHARED GuillaumeLatapyModel.cpp)
TARGET_LINK_LIBRARIES(GuillaumeLatapy-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(GuillaumeLatapy-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(HolmeKim-${TulipVersion} SHARED HolmeKim.cpp)
TARGET_LINK_LIBRARIES(HolmeKim-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(HolmeKim-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(KlemmEguiluz-${TulipVersion} SHARED KlemmEguiluzModel.cpp)
TARGET_LINK_LIBRARIES(KlemmEguiluz-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(KlemmEguiluz-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(LiuEtAl-${TulipVersion} SHARED LiuEtAl.cpp)
TARGET_LINK_LIBRARIES(LiuEtAl-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(LiuEtAl-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangEtAl-${TulipVersion} SHARED WangEtAl.cpp)
TARGET_LINK_LIBRARIES(WangEtAl-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WangEtAl-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangRong-${TulipVersion} SHARED WangRong.cpp)
TARGET_LINK_LIBRARIES(WangRong-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WangRong-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WattsStrotgatz-${TulipVersion} SHARED WattsStrotgatzModel.cpp)
TARGET_LINK_LIBRARIES(WattsStrotgatz-${TulipVersion} ${LibTulipCoreName})
TULIP_INSTALL_PLUGIN(WattsStrotgatz-${TulipVersion} ${TulipPluginsInstallDir})
