#!/bin/bash

# Available when built with cmake.
# This script only puts the binaries together, 
# so it may not work on anything other than ubuntu 21.04 64bit.

cd `dirname $0`

DEPLOYDIR="autobatchtools-1.0.0-ubuntu-gcc-x64"
BUILDTYPE="MinSizeRel"
QTVERSION="6.1.3"
QTVERSION_MAJOR="6"
ICUVERSION="56.1"
ICUVERSION_MAJOR="56"
QTDIR="/opt/Qt/${QTVERSION}/gcc_64"

mkdir ${DEPLOYDIR}

# Binaries
cp ../build-AutoBatchTools-master-Desktop_Qt_${QTVERSION//./_}_GCC_64bit-${BUILDTYPE}/AutoBatchRunner ${DEPLOYDIR}
cp ../build-AutoBatchTools-master-Desktop_Qt_${QTVERSION//./_}_GCC_64bit-${BUILDTYPE}/AutoBatchScheduler ${DEPLOYDIR}
cp ../build-AutoBatchTools-master-Desktop_Qt_${QTVERSION//./_}_GCC_64bit-${BUILDTYPE}/AutoBatchEditor ${DEPLOYDIR}
cp ../build-AutoBatchTools-master-Desktop_Qt_${QTVERSION//./_}_GCC_64bit-${BUILDTYPE}/lib/*.so ${DEPLOYDIR}

# Translations
mkdir "${DEPLOYDIR}/translation"
pushd "${DEPLOYDIR}/translation"
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
cp ${QTDIR}/lib/libQt6Core.so.${QTVERSION} ${DEPLOYDIR}/libQt6Core.so.${QTVERSION_MAJOR}
cp ${QTDIR}/lib/libQt6Gui.so.${QTVERSION} ${DEPLOYDIR}/libQt6Gui.so.${QTVERSION_MAJOR}
cp ${QTDIR}/lib/libQt6Svg.so.${QTVERSION} ${DEPLOYDIR}/libQt6Svg.so.${QTVERSION_MAJOR}
cp ${QTDIR}/lib/libQt6Widgets.so.${QTVERSION} ${DEPLOYDIR}/libQt6Widgets.so.${QTVERSION_MAJOR}
cp ${QTDIR}/lib/libQt6DBus.so.${QTVERSION} ${DEPLOYDIR}/libQt6DBus.so.${QTVERSION_MAJOR}

# Copy Window manager files
cp ${QTDIR}/lib/libQt6XcbQpa.so.${QTVERSION} ${DEPLOYDIR}/libQt6XcbQpa.so.${QTVERSION_MAJOR}
cp ${QTDIR}/lib/libQt6OpenGL.so.${QTVERSION} ${DEPLOYDIR}/libQt6OpenGL.so.${QTVERSION_MAJOR}
cp -a ${QTDIR}/plugins/platforms ${DEPLOYDIR}/platforms
cp -a ${QTDIR}/plugins/imageformats ${DEPLOYDIR}/imageformats

# Copy libicu libraries
cp ${QTDIR}/lib/libicui18n.so.${ICUVERSION} ${DEPLOYDIR}/libicui18n.so.${ICUVERSION_MAJOR}
cp ${QTDIR}/lib/libicuuc.so.${ICUVERSION} ${DEPLOYDIR}/libicuuc.so.${ICUVERSION_MAJOR}
cp ${QTDIR}/lib/libicudata.so.${ICUVERSION} ${DEPLOYDIR}/libicudata.so.${ICUVERSION_MAJOR}
