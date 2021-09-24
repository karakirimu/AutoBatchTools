:: Available when built with cmake.

cd %~dp0
set DEPLOYDIR="autobatchtools-1.0.0-win-msvc2019-x64"
set QTVERSION=6.1.3
set BUILDTYPE=MinSizeRel
set QTDIR=C:\Qt\%QTVERSION%\msvc2019_64\bin

mkdir %DEPLOYDIR%
cd %DEPLOYDIR%

:: Binaries
copy /b ..\build\AutoBatchTools-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchRunner.exe .\
copy /b ..\build\AutoBatchTools-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchScheduler.exe .\
copy /b ..\build\AutoBatchTools-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\AutoBatchEditor.exe .\
copy /b ..\build\AutoBatchTools-Desktop_Qt_%QTVERSION:.=_%_MSVC2019_64bit-%BUILDTYPE%\*.dll .\

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
%QTDIR%\windeployqt.exe .\ --no-translations

pause .
