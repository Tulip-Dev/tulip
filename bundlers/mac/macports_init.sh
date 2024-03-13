# This scripts aims to perform automatic installation
# of macports latest version

# get macOS current major.minor version
MACOS_VERSION=$(sw_vers -productVersion | cut -f 1-2 -d .)

# get major version
MAJOR_VERSION=$(echo $MACOS_VERSION | cut -f 1 -d .)

# for macOS > 10 only major is needed
if [ $MAJOR_VERSION -gt 10 ]
then
    MACOS_VERSION=$MAJOR_VERSION
fi

# set MACOS_NAME
case $MACOS_VERSION in
    10.14)
	MACOS_NAME=Mojave
	;;
    10.15)
	MACOS_NAME=Catalina
	;;
    11)
	MACOS_NAME=BigSur
	;;
    12)
	MACOS_NAME=Monterey
	;;
    13)
	MACOS_NAME=Ventura
	;;
    14)
	MACOS_NAME=Sonoma
	;;
    *)
	echo "Unknown MACOSX version $MACOS_VERSION"
        exit 1
	;;
esac

# get macport latest release page
curl -LO https://github.com/macports/macports-base/releases/latest
# extract MACPORTS_VERSION
MACPORTS_VERSION=$(grep '<title>' latest | awk '{print $3}')

# set macports installer and url
MACPORTS_INSTALLER=MacPorts-${MACPORTS_VERSION}-${MACOS_VERSION}-${MACOS_NAME}.pkg
MACPORTS_INSTALLER_URL=https://github.com/macports/macports-base/releases/download/v${MACPORTS_VERSION}/${MACPORTS_INSTALLER}

# download installer
curl -LO ${MACPORTS_INSTALLER_URL}
# install MacPorts
sudo installer -store -pkg ${MACPORTS_INSTALLER} -target /
# synchronize port trees if needed
export PATH=/opt/local/bin:$PATH
sudo port sync

# remove downloaded files
rm latest ${MACPORTS_INSTALLER}
