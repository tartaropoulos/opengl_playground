@echo OFF

mkdir build
cd build
cmake -G "MinGW Makefiles" .. || goto ERROR
mingw32-make -j8 || goto ERROR
:: mingw32-make install || goto ERROR

goto :EOF

:ERROR
echo "FAILED. Errors were during building"
