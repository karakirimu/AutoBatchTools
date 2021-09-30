:: Available when built with cmake.
:: %1 : Qt installed version
:: %2 : Qt installed directory

set DEPLOYDIR="autobatchtools-win-x64"
set QTVERSION=%1
set BUILDTYPE=MinSizeRel
set QTDIR=%~2\Qt\%QTVERSION%\msvc2019_64\bin

mkdir %DEPLOYDIR%
cd %DEPLOYDIR%

:: Binaries
copy /b ..\build\%BUILDTYPE%\AutoBatchRunner.exe .\
copy /b ..\build\%BUILDTYPE%\AutoBatchScheduler.exe .\
copy /b ..\build\%BUILDTYPE%\AutoBatchEditor.exe .\
copy /b ..\build\%BUILDTYPE%\*.dll .\

:: Translations
mkdir "translation"
pushd "translation"
for %%f in (..\..\src\translation\abt_*.ts) do (
  %QTDIR%\lrelease.exe %%f
)
copy /b ..\..\src\translation\abt_*.qm .\
popd .

:: License files
copy /b ..\LICENSE .\LICENSE.txt
copy /b ..\res\thirdparty\LICENSE-QT .\LICENSE-QT.txt

:: Initialize plugin directory
mkdir "plugins"

:: Deploy Qt files
.\%QTDIR%\windeployqt.exe .\ --no-translations
