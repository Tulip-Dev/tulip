#!/bin/sh

TULIP_DIR=$1
DEST_DIR=$2
SRC_DIR=$(pwd)

echo 'Wiping previous bundles into '$DEST_DIR
mkdir -p $DEST_DIR
cd $DEST_DIR
rm -fr Tulip.app
echo 'Creating base directories'
mkdir -p Tulip.app/Contents/{MacOS,Resources,Frameworks,PlugIns,SharedSupport}
cd Tulip.app/Contents
cp "$SRC_DIR/Info.plist" .
cp "$SRC_DIR/tulip.icns" "Resources/"
mkdir MacOS/tulip
cd MacOS/tulip
echo 'Copying binaries'
cp -r $TULIP_DIR/* .
rm -fr macos
cd ..
cp -r $TULIP_DIR/macos/* .
