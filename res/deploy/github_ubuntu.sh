#!/bin/bash

# Available when built with cmake.
# $1 : Qt installed version
# $2 : Qt installed directory

DEPLOYDIR="autobatchtools-ubuntu-x64"
QTVERSION="$1"
QTVERSION_MAJOR="${QTVERSION:0:1}"
ICUVERSION="56.1"
ICUVERSION_MAJOR="${ICUVERSION:0:2}"
QTDIR=${2//\"/}/Qt/${QTVERSION}/gcc_64

mkdir ${DEPLOYDIR}

# Binaries
cp ../../../build/AutoBatchRunner ${DEPLOYDIR}
cp ../../../build/AutoBatchScheduler ${DEPLOYDIR}
cp ../../../build/AutoBatchEditor ${DEPLOYDIR}
cp ../../../build/lib/*.so ${DEPLOYDIR}

# Translations
mkdir "${DEPLOYDIR}/translation"
pushd "${DEPLOYDIR}/translation"
for file in "../../src/translation/abt_*.ts"
do
  ../../${QTDIR}/bin/lrelease $file
done
cp ../../src/translation/abt_*.qm ./
popd

# License files
cp ./LICENSE ${DEPLOYDIR}
cp ./res/thirdparty/LICENSE-QT ${DEPLOYDIR}

# Icons
mkdir "${DEPLOYDIR}/icon"
pushd "${DEPLOYDIR}/icon"
cp ../../res/app_icons/app_abr.png ./
cp ../../res/app_icons/app_abs.png ./
cp ../../res/app_icons/app_pe.png ./
popd

# Install script
cp ./res/script/ubuntu_install.sh ${DEPLOYDIR}/install.sh
chmod +x ${DEPLOYDIR}/install.sh
cp ./res/script/ubuntu_uninstall.sh ${DEPLOYDIR}/uninstall.sh
chmod +x ${DEPLOYDIR}/uninstall.sh
cp ./res/script/ubuntu_launch.sh ${DEPLOYDIR}/launch.sh
chmod +x ${DEPLOYDIR}/launch.sh

# Initialize plugin directory
mkdir "${DEPLOYDIR}/plugins"

# Copy Qt libraries
cp ../../../${QTDIR}/lib/libQt6Core.so.${QTVERSION} ${DEPLOYDIR}/libQt6Core.so.${QTVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libQt6Gui.so.${QTVERSION} ${DEPLOYDIR}/libQt6Gui.so.${QTVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libQt6Svg.so.${QTVERSION} ${DEPLOYDIR}/libQt6Svg.so.${QTVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libQt6Widgets.so.${QTVERSION} ${DEPLOYDIR}/libQt6Widgets.so.${QTVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libQt6DBus.so.${QTVERSION} ${DEPLOYDIR}/libQt6DBus.so.${QTVERSION_MAJOR}

# Copy Window manager files
cp ../../../${QTDIR}/lib/libQt6XcbQpa.so.${QTVERSION} ${DEPLOYDIR}/libQt6XcbQpa.so.${QTVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libQt6OpenGL.so.${QTVERSION} ${DEPLOYDIR}/libQt6OpenGL.so.${QTVERSION_MAJOR}
cp -a ../../../${QTDIR}/plugins/platforms ${DEPLOYDIR}/platforms
cp -a ../../../${QTDIR}/plugins/imageformats ${DEPLOYDIR}/imageformats

# Copy libicu libraries
cp ../../../${QTDIR}/lib/libicui18n.so.${ICUVERSION} ${DEPLOYDIR}/libicui18n.so.${ICUVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libicuuc.so.${ICUVERSION} ${DEPLOYDIR}/libicuuc.so.${ICUVERSION_MAJOR}
cp ../../../${QTDIR}/lib/libicudata.so.${ICUVERSION} ${DEPLOYDIR}/libicudata.so.${ICUVERSION_MAJOR}
