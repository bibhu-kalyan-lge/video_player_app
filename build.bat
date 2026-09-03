@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

set GSTREAMER_ROOT_X86_64=C:\Program Files\gstreamer\1.0\msvc_x86_64

echo ==================================
echo Building GStreamer Video Player
echo ==================================

if exist *.obj del *.obj
if exist main.exe del main.exe

cl /EHsc ^
main.cpp ^
FilePicker.cpp ^
MediaInfo.cpp ^
Utils.cpp ^
VideoPlayer.cpp ^
YoutubeHandler.cpp ^
/I"%GSTREAMER_ROOT_X86_64%\include\gstreamer-1.0" ^
/I"%GSTREAMER_ROOT_X86_64%\include\glib-2.0" ^
/I"%GSTREAMER_ROOT_X86_64%\lib\glib-2.0\include" ^
/link ^
/LIBPATH:"%GSTREAMER_ROOT_X86_64%\lib" ^
gstreamer-1.0.lib ^
gobject-2.0.lib ^
glib-2.0.lib ^
Comdlg32.lib

echo.
echo Build Complete
pause