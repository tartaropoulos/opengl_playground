@echo OFF

mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-mingw-static .. || goto ERROR
mingw32-make -j8 || goto ERROR
:: mingw32-make install || goto ERROR

goto :EOF

:ERROR
echo "FAILED. Errors were during building"
