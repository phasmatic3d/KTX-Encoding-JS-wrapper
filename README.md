# ppc-ktx

- git clone https://github.com/phasmatic3d/ppc-ktx.git
- git submodule update  --init --recursive
- emsdk activate latest
- emsdk_env.bat
- emcmake cmake -S . -B build -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM=D:\Tools\make\bin\make.exe
    * Make sure "cmake.exe" is in your path
- cmake.exe --build build --config Release