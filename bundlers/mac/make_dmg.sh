#!/bin/sh
#
# Creates a disk image (dmg) on Mac OS X from the command line.
# usage:
#    make_dmg.sh <volname> <vers> <srcdir>
#
# Where <volname> is the name to use for the mounted image, <vers> is the version
# number of the volume and <srcdir> is where the contents to put on the dmg are.
#
# The result will be a file called <volname>-<vers>.dmg

NAME=$1
VERSION=$2
SRC_DIR=$3

DMG=$NAME-$VERSION

SIZE=$(du -ms $SRC_DIR | cut -f 1)
SIZE=$((SIZE + 20))
hdiutil create -srcfolder "${SRC_DIR}" -volname "${DMG}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -megabytes ${SIZE} pack.temp.dmg
device=$(hdiutil attach -readwrite -noverify -noautoopen "pack.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')
echo '
   tell application "Finder"
     tell disk "'${DMG}'"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {400, 100, 885, 430}
           set theViewOptions to the icon view options of container window
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 72
           set background picture of theViewOptions to file ".background:background.png"
           make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
           set position of item "'${DMG}'" of container window to {100, 100}
           set position of item "Applications" of container window to {375, 100}
           update without registering applications
           delay 5
           eject
     end tell
   end tell
' | osascript
chmod -Rf go-w /Volumes/"${DMG}"
sync
sync
hdiutil detach ${device}
hdiutil convert "pack.temp.dmg" -format UDZO -imagekey zlib-level=9 -o "${DMG}"
rm -f pack.temp.dmg
