#!/bin/sh

TULIP_DIR=$1
DEST_DIR=$2
SRC_DIR=$(pwd)

echo 'Wiping destination directory'
rm -fr "$DEST_DIR"
mkdir -p "$DEST_DIR/files/lib"
mkdir -p "$DEST_DIR/files/bin"
mkdir -p "$DEST_DIR/files/include"

echo 'Copying tulip files'
cp -r $TULIP_DIR/* "$DEST_DIR/files/"

echo 'Copying NSIS script'
cp "$SRC_DIR/Tulip.nsi" "$DEST_DIR/"
cp "$SRC_DIR/../../COPYING" "$DEST_DIR/"

echo 'Running NSIS generator'
cd "$DEST_DIR"
makensis Tulip.nsi
