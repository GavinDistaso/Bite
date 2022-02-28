@echo off
cd ..
mkdir build
cd build

cmake .. -G "MinGW Makefiles"
make
cd ../tools