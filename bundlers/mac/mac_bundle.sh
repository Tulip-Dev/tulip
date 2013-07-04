#!/bin/sh

function usage {
        echo "Usage : $0 <Tulip Install Dir> <Bundle Destination Dir> <Tulip Version>"
}

if [ $# -ne 3 ]
then
	usage $0
	exit 1
fi

TULIP_DIR=$1
DEST_DIR=$2
TULIP_VERSION=$3
SRC_DIR=$(pwd)

if [ ! -d $TULIP_DIR ]
then
	echo "Error : the specified Tulip installation directory does not exist"
	exit 1
fi

echo 'Wiping previous bundles into '$DEST_DIR
rm -rf $DEST_DIR/application 2>/dev/null
mkdir -p $DEST_DIR/application
cd $DEST_DIR/application
mkdir .background
cp $SRC_DIR/background.png .background/
rm -rf * 2>/dev/null

echo 'Creating base directories'
cd $DEST_DIR/application
mkdir -p Tulip.app/Contents/{MacOS,Resources,Frameworks}
cd Tulip.app/Contents
cp "$SRC_DIR/Info.plist" .
cp "$SRC_DIR/tulip.icns" "Resources/"

echo 'Copying binaries'
cd $DEST_DIR/application/Tulip.app/Contents/
cp -r $TULIP_DIR/* .
mv lib/*.dylib Frameworks/


chmod +x MacOS/tulip_launcher
	
echo 'Copying license'
cd $DEST_DIR/application
cp $SRC_DIR/../../COPYING .

echo 'Copying Frameworks'
cd $DEST_DIR/application/Tulip.app/Contents/Frameworks/
for cmp in QtOpenGL QtWebKit QtXmlPatterns QtGui QtTest QtXml QtNetwork QtCore phonon QtScript QtHelp QtDBus QtDeclarative QtSql QtSvg; do
  echo "Copying /Library/Frameworks/${cmp}.framework/$cmp" to $(pwd)
  cp "/Library/Frameworks/${cmp}.framework/$cmp" .
done
cp -r /Library/Frameworks/QtGui.framework/Resources/qt_menu.nib ../Resources
cp -r /Developer/Applications/Qt/plugins/imageformats ../
cp /usr/lib/libQtCLucene.4.dylib .

# Case when we build the bundle on Snow Leopard or lower version of MacOS with Python 2.7
# Python 2.7 has to be manually installed for building Tulip
# We include the Python Framework in the bundle in order for Tulip to run
# when Python 2.7 is not installed on the user sytem (Snow Leopard and Leopard)
if [ ! -e /usr/lib/libpython2.7.dylib -a -d /Library/Frameworks/Python.framework/Versions/2.7 ]
then
    echo "Copying /Library/Frameworks/Python".framework to $(pwd)
    # Copy the whole Python framework
    cp "/Library/Frameworks/Python.framework/Versions/2.7/lib/libpython2.7.dylib" .
    cp "/Library/Frameworks/Python.framework/Versions/2.7/Python" .
    cp -r "/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7" ../lib/ 
    find ../lib/python2.7 \( -iname \*\.pyc -o -iname \*\.pyo \) -print0 | xargs -0 rm -f     
fi

echo 'Copying libraries'
for lib in libGLEW libfreetype libz libstdc++; do
  for search_path in /usr/lib /opt/local/lib ; do
    [ "$(ls $search_path/$lib*.dylib 2> /dev/null)" != "" ] && cp $search_path/$lib*.dylib .
  done
done

echo 'Generating DMG'
cd $DEST_DIR
sh $SRC_DIR/make_dmg.sh "Tulip" "${TULIP_VERSION}" $DEST_DIR/application

rm -rf $DEST_DIR/application 2>/dev/null

echo "Tulip-${TULIP_VERSION}.dmg has been correctly generated in ${DEST_DIR}"
