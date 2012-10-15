#!/bin/sh

TULIP_DIR=$1
DEST_DIR=$2
SRC_DIR=$(pwd)

echo 'Wiping previous bundles into '$DEST_DIR
rm -fr $DEST_DIR
mkdir -p $DEST_DIR/application
cd $DEST_DIR/application
mkdir .background
cp $SRC_DIR/background.png .background/
rm -fr *

echo 'Creating base directories'
cd $DEST_DIR/application
mkdir -p Tulip.app/Contents/{MacOS,Resources,Frameworks}
cd Tulip.app/Contents
cp "$SRC_DIR/Info.plist" .
cp "$SRC_DIR/tulip.icns" "Resources/"

echo 'Copying binaries'
cd $DEST_DIR/application/Tulip.app/Contents/MacOS
mkdir tulip
cd tulip
cp -r $TULIP_DIR/* .
mv macos/* ..
chmod +x ../tulip_launcher
rm -fr macos
rm -fr include/

echo 'Copying license'
cd $DEST_DIR/application
cp $SRC_DIR/../../COPYING .

echo 'Copying Frameworks'
cd $DEST_DIR/application/Tulip.app/Contents/Frameworks/
for cmp in QtOpenGl QtWebKit QtXmlPatterns QtGui QtTest QtXml QtNetwork QtCore phonon QtDBus; do
  echo "Copying /Library/Frameworks/$cmp".framework to $(pwd)
  cp -r "/Library/Frameworks/$cmp".framework .
done

echo 'Copying libraries'
cd $DEST_DIR/application/Tulip.app/Contents/MacOS/tulip/lib
for lib in libGLEW libfreetype libz libstdc++; do
  for search_path in /usr/lib /opt/local/lib /usr/X11/lib; do
    [ "$(ls $search_path/$lib*.dylib 2> /dev/null)" != "" ] && cp $search_path/$lib*.dylib .
  done
done

echo 'Generating DMG'
cd $DEST_DIR
sh $SRC_DIR/make_dmg.sh "Tulip" "4.0" $DEST_DIR/application
