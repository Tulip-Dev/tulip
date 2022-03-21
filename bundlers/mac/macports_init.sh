# This scripts aims to perform automatic installation
# of macports latest version

# get macOS current major.minor version
MACOS_VERSION=$(sw_vers -productVersion | cut -f 1-2 -d .)

# get major version
MAJOR_VERSION=$(echo $MACOS_VERSION | cut -f 1 -d .)

# for macOS 11 & 12 (Big Sur and Monterey) only major is needed
if [ $MAJOR_VERSION -gt 10 ]
then
    MACOS_VERSION=$MAJOR_VERSION
fi

# get macport latest release page
curl -LO https://github.com/macports/macports-base/releases/latest
# extract install url corresponding to MACOS_VERSION
MACPORTS_URL=https://github.com$(grep 'href="/macports/macports-base/releases/download/' latest | grep '.pkg"' | grep "\-$MACOS_VERSION\-" | awk -F '\"' '{print $2}')

# download installer
curl -LO $MACPORTS_URL
# install MacPorts
sudo installer -store -pkg MacPorts-*.pkg -target /
# update it
export PATH=/opt/local/bin:$PATH
sudo port -d -N selfupdate

# remove donwloaded files
rm latest MacPorts-*.pkg
