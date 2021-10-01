#!/bin/bash
cd `dirname $0`

ROOTDIR=$(cd $(dirname $0); pwd)

EDITORNAME=AutoBatchEditor
EDITORICON=app_pe.png

SCHEDULERNAME=AutoBatchScheduler
SCHEDULERICON=app_abs.png

RUNNERNAME=AutoBatchRunner
RUNNERICON=app_abr.png

install_check () {
    dpkg -s $1 &> /dev/null
    if [ $? -eq 0 ]; then
        echo "Package $1 is installed!"
    
    else
        echo "Package $1 is NOT installed!"
        echo "Installing $1 ..."
        sudo apt install -y $1
    fi
}

echo "Install dependencies ..."
install_check libopengl0
echo "Completed."

make_shortcut () {
    touch $1.desktop
    {
        echo "#!/usr/bin/env xdg-open"
        echo "[Desktop Entry]"
        echo "Version=1.0"
        echo "Type=Application"
        echo "Name=$1"
        echo "Icon=${ROOTDIR}/icon/$2"
        echo "Path=${ROOTDIR}"
        echo "Exec=${ROOTDIR}/launch.sh ./$1"
    } >> $1.desktop
    chmod +x $1.desktop
}

pushd "${HOME}/.local/share/applications"
make_shortcut ${EDITORNAME} ${EDITORICON}
make_shortcut ${SCHEDULERNAME} ${SCHEDULERICON}
make_shortcut ${RUNNERNAME} ${RUNNERICON}
popd

echo "Copying the shortcut was completed."

chmod +x ./AutoBatchRunner
chmod +x ./AutoBatchEditor
chmod +x ./AutoBatchScheduler
chmod +x ./launch.sh