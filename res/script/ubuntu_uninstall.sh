#!/bin/bash

EDITORNAME=AutoBatchEditor
SCHEDULERNAME=AutoBatchScheduler
RUNNERNAME=AutoBatchRunner

pushd "${HOME}/.local/share/applications"
rm -rf ${EDITORNAME}.desktop
rm -rf ${SCHEDULERNAME}.desktop
rm -rf ${RUNNERNAME}.desktop
popd

echo "The shortcut has been removed."