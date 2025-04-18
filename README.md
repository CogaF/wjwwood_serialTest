This repositore starts as a template for Visual Studio (currently ver. 2022). It gives the possibility to use wxWidgets as gui (tested ver 3.2.6 and 3.2.7) and implement serial communication.

Depends on:

- wxWidgets: https://github.com/wxWidgets/wxWidgets
- wjwwood serial: https://github.com/wjwwood/serial


in order to use it the system must have two environmental variables for wxWidgets: "WXWIN" and for serial library: "VC_WJWWOOD_SERIAL"

By default there build can be done both for 32 and 64 bit in Release, Debug and ReleaseDll mode.

ReleaseDll uses the dynamic build version of wxWidgets thus it must be build "shared" in case this options is going to be used.

The other two are as they speak for themselves, Debug and Release libraries for both wxWidgets and wjwwood_serial.

environmental variables must point to root folders of each library and must have the following structure:

---------------------------------------------------------------------------------------------------------------------------------
VC_WJWWOOD_SERIAL must point to the folder that has the following structure:

-include (inside the exact copy of wjwwod_serial include folder, the same one used to build the library)
-x64
  -Debug
    -serial.lib
  -Release
    -serial.lib
-Win32
  -Debug
    -serial.lib
  -Release
    -serial.lib

---------------------------------------------------------------------------------------------------------------------------------
WXWIN must point to the folder that has the following structure:

-include (inside the exact copy of wxWidgets include folder, the same one used to build the library)
-lib
  -vc_dll (this must contain all .dll files that were build for Windows 32 bit used by Releasedll version)
  -vc_lib (this must contain all .lib files that were build for Windows 32 bit used by Release and Debug version)
  -vc_x64_dll (this must contain all .dll files that were build for Windows 64 bit used by Releasedll version)
  -vc_x64_lib (this must contain all .lib files that were build for  Windows 64 bit used by Release and Debug version)

---------------------------------------------------------------------------------------------------------------------------------


WJWWOOD_SERIAL is a bit old and seems not maintained anymore so pay attention and think twice using it.

The library at the above github link already contains .sln file to build it using with visual studio. It was created with older version of visual studio.
I Did build it using Visual studio 17 (2022) and the porting was automatic and painless. It build with warning in converting wchar in char.
Did some digging and this warning is generated when getting portname of the serial port. Might become an issue if port has more than one byte characters in its name.
To my knowledge port naming has ASCII naming thus never occur an error. Nevertheless, to aviod dragging that I did some tiny modification and made the library return a wstring.
Doing so now the problem won't be in the .lib but in correctly handling the result from .lib function. These modifications occurd in header files for each platform and the on .cpp file.

---------------------------------------------------------------------------------------------------------------------------------

WXWIDGETS is well documented, well maintained, open source and the team behind, IMO, is doing an amazing work. 

They have a very rich forum (https://forums.wxwidgets.org/) and there is always someone of the team that gives suggestions.

"Hope this helps"
Coga F.
