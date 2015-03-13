# RunMetroAppWithUserAppId
* Run Metro Style App With its UserAppId

## How to Build
* Using Visual studio 2013, open the project file, RunMetroAppWithUserAppId.sln.
* Set the target platform (Win32 or x64)
* Set the configuration (Release or Debug)
* Build solution.
* There are output files in $(SolutionDir)\Output


## How to Use
* RunMetroAppWithUserAppId.exe must need just 1(one) argument.
* Run regedit.exe.
* Find HKEY_CLASSES_ROOT\ActivatableClasses\Package
* Find the package to launch with this program.
* Find "Server" subkey and main server subkey.
* There is "AppUserModelId" Value in above subkey.
* Its value is our argument.

For example,
Microsoft.Reader Metro App
- HKEY_CLASSES_ROOT\ActivatableClasses\Package\Microsoft.Reader_6.3.9654.17499_x64__8wekyb3d8bbwe\Server\Microsoft.Reader.AppXtszmc7avrx02s7n8gch63tzwg517wd9k.mca\AppUserModelId
- value : Microsoft.Reader_8wekyb3d8bbwe!Microsoft.Reader
- Go to commandline, and run "RunMetroAppWithUserAppId.exe Microsoft.Reader_8wekyb3d8bbwe!Microsoft.Reader"

## Developing
* C++
* Windows 8.1

### Tools
* VisualStudio 2013

#### License
MIT