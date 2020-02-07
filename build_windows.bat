rmdir /s /q build
mkdir build
cd build
cmake "-DCMAKE_TOOLCHAIN_FILE=E:\projects\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x64-windows" ../
cd ..
