# Process to build TRAPTA server installer :


## Prerequisites :

* QT Creator
* windeployqt6.exe
* [nsis-3.10](https://sourceforge.net/projects/nsis/files/latest/download)


## Procedure :

* Build the TRAPTA Server Release using QT Creator
* Create the folder `release-build\Windows\TRAPTA`
* Copy on it the TRAPTA server Binary
* Copy the folder `traptatransfer\SQLSelect` to `release-build\Windows\TRAPTA\SQLSelect`
* Export QT6 DLL using the `windeployqt6.exe` command
```
windeployqt6.exe --qmldir <QT Installation Path>/6.8.1/mingw_64/qml <Trapta source code path>/release-build/Windows/TRAPTA/TRAPTA.exe
```
* Clean the generated DLL and folder to correspond to the schema provided here : `release-build\Windows\schema`
* run the NSIS script `release-build\Windows\trapta-install-script.nsi`
* The installer is generated on `release-build\Windows\archives`