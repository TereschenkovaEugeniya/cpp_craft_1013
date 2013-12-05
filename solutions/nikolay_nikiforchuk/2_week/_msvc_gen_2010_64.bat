@echo off

set BOOST_ROOT=D:/Development/Tools/boost/boost_1_54_0
set SOLUTION_NAME=2_week
::set BINARY_READER_ROOT=d:/some_path

set BUILD_TYPE=Debug
if [%1]==[Release] (
	set BUILD_TYPE=Release
)
set BUILD_FOLDER=_build_%BUILD_TYPE%_64

if not exist %BUILD_FOLDER% (
	mkdir %BUILD_FOLDER%
)

cd %BUILD_FOLDER%
cmake -DBOOST_STAGE_FOLDER_WITH_ADDRESS_MODEL=ON -DVERBOSE=ON -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSOLUTION_NAME=%SOLUTION_NAME%  -G "Visual Studio 12 Win64" ../ 
cd ../

echo "%BUILD_FOLDER%/%SOLUTION_NAME%.sln" > _start_msvc.bat
