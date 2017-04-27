INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})        

ADD_CORE_FILES(CMakeLists.txt *.cpp)

ADD_LIBRARY(AttractAndIntroduce-${TulipVersion} SHARED AttractAndIntroduce.cpp)
TARGET_LINK_LIBRARIES(AttractAndIntroduce-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(AttractAndIntroduce-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Bollobas-${TulipVersion} SHARED BollobasModel.cpp)
TARGET_LINK_LIBRARIES(Bollobas-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(Bollobas-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(BuWangZhou-${TulipVersion} SHARED BuWangZhouModel.cpp)
TARGET_LINK_LIBRARIES(BuWangZhou-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(BuWangZhou-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Cantazaro-${TulipVersion} SHARED Catanzaro.cpp)
TARGET_LINK_LIBRARIES(Cantazaro-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(Cantazaro-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(Fuliao-${TulipVersion} SHARED FuLiao.cpp)
TARGET_LINK_LIBRARIES(Fuliao-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(Fuliao-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(GuillaumeLatapy-${TulipVersion} SHARED GuillaumeLatapyModel.cpp)
TARGET_LINK_LIBRARIES(GuillaumeLatapy-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(GuillaumeLatapy-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(HolmeKim-${TulipVersion} SHARED HolmeKim.cpp)
TARGET_LINK_LIBRARIES(HolmeKim-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(HolmeKim-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(KlemmEguiluz-${TulipVersion} SHARED KlemmEguiluzModel.cpp)
TARGET_LINK_LIBRARIES(KlemmEguiluz-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(KlemmEguiluz-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(LiuEtAl-${TulipVersion} SHARED LiuEtAl.cpp)
TARGET_LINK_LIBRARIES(LiuEtAl-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(LiuEtAl-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangEtAl-${TulipVersion} SHARED WangEtAl.cpp)
TARGET_LINK_LIBRARIES(WangEtAl-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(WangEtAl-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WangRong-${TulipVersion} SHARED WangRong.cpp)
TARGET_LINK_LIBRARIES(WangRong-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(WangRong-${TulipVersion} ${TulipPluginsInstallDir})

ADD_LIBRARY(WattsStrotgatz-${TulipVersion} SHARED WattsStrotgatzModel.cpp)
TARGET_LINK_LIBRARIES(WattsStrotgatz-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(WattsStrotgatz-${TulipVersion} ${TulipPluginsInstallDir})
