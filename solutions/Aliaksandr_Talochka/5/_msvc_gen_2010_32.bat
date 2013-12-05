@echo off

set SOLUTION_NAME=Task_5
set BOOST_ROOT=d:/Install/Programming/boost_1_54_0

set BUILD_TYPE=Debug
if [%1]==[Release] (
	set BUILD_TYPE=Release
)
set BUILD_FOLDER=_build_%BUILD_TYPE%_32

if not exist %BUILD_FOLDER% (
	mkdir %BUILD_FOLDER%
)

cd %BUILD_FOLDER%
cmake -DBOOST_STAGE_FOLDER_WITH_ADDRESS_MODEL=OFF -DVERBOSE=OFF -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSOLUTION_NAME=%SOLUTION_NAME%  -G "Visual Studio 10" ../ 
cd ../

echo "%BUILD_FOLDER%/%SOLUTION_NAME%.sln" > _start_msvc.bat
