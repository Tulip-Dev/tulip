#!/bin/bash

echo "Getting current version number for package tulip $1 in Tulip PPA"

sed_regexp="s/.*tulip_$1-[0-9]*ubuntu\([0-9][0-9]*[\.]*[0-9]*\).*\.tar\.gz.*/\1/"
existing_package_versions=`wget -r -np --spider $2 2>&1 | grep ".*Removing.*tar.gz" | sed ${sed_regexp}`
max_version=0
for version in ${existing_package_versions[@]}
do
  if [[ "$version" =~ ^[0-9]+\.*[0-9]*$ ]] ; then
    comp_result=`echo "$version > $max_version" | bc`
    if [ $comp_result == 1 ]
    then
      max_version=$version
    fi
  fi
done

echo "Current version number for package tulip $1 is $max_version"

if [ $max_version == 0 ]
then
proposed_version=1
else
proposed_version=`echo "$max_version + 0.1" | bc | sed 's/\(.*\)\.0/\1/'`
fi

version_ok=0

while [ $version_ok == 0 ]
do
echo "Enter the version number for the package to upload or press ENTER to used the proposed one ($proposed_version):"
read user_version

final_version=""

if [ "$user_version" == "" ]
then
  final_version=$proposed_version
  version_ok=1
else
  user_version_ok=`echo $user_version | sed 's/^[0-9]*\.*[0-9]*$//' | wc -c`
  if [ $user_version_ok == 1 ]
  then
    user_version_greater=`echo "$user_version > $max_version" | bc`
    if [ $user_version_greater == 1 ]
    then
      final_version=`echo $user_version | sed 's/\(.*\)\.0/\1/'`
      version_ok=1
    else
      echo "The version number you entered is lesser than the current tulip $1 package version"
      echo ""
    fi
  else
    echo "The package version number you entered is not well formated (must be X[.Y])"
    echo ""
  fi
fi
done

# print the choosen version to stderr instead of stdout (because some echo are preformed above) to retrieve it from a CMake script
echo -n $final_version 1>&2
  