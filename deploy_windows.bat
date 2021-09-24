:: Available when built with cmake.

cd %dp0
set DEPLOYDIR="autobatchtools-1.0.0-win-msvc2019-x64"
set QTVERSION=6.1.3
set BUILDTYPE=MinSizeRel

mkdir %DEPLOYDIR%
cd %DEPLOYDIR%

:: Binaries
copy /b ..\build\master-autobatchrunner-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchRunner.exe .\
copy /b ..\build\master-autobatchrunner-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchScheduler.exe .\
copy /b ..\build\master-autobatchrunner-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchEditor.exe .\
copy /b ..\build\master-autobatchrunner-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\*.dll .\

:: Translations
mkdir "translation"
pushd "translation"
copy /b ..\..\src\translation\abt_*.qm .\
popd .

:: License files
copy /b ..\LICENSE .\LICENSE.txt
copy /b ..\res\thirdparty\LICENSE-QT .\LICENSE-QT.txt

:: Initialize plugin directory
mkdir "plugins"

:: Deploy Qt files
C:\Qt\6.1.2\msvc2019_64\bin\windeployqt.exe .\ --no-translations
