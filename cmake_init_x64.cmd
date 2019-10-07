set BOOST_ROOT=c:\boost_1_x
set BOOST_LIBRARYDIR=C:\boost_1_64_0\lib64-msvc-14.1
"c:\Program Files\CMake\bin\cmake.exe" ^
  -Boutput_64 ^
  -H. ^
  -G"Visual Studio 15 2017 Win64" 

pause