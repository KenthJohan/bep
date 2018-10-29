# Skopy


### demo_single
Input file at argument 1


### demo_all
Reads all files at data/barktree***
and evaluates answer.

### tri_random
Randomizes scenario for a park.
Uses point in triangle test to shorten the leash.
TODO: Take multiple trees inside the triangle into consideration.

### tri_fromfile
Input file at argument 1
Uses point in triangle test to shorten the leash.
TODO: Take multiple trees inside the triangle into consideration.

### star
Work in progress.
Randomizes scenario for a park.
Skopy and every tree has a connection (star topology).
Every tree can count number of windings it got from the leash.
TODO: Using the information from the windings to create a track around the trees.



```
gcc -std=gnu11 src/triangle/demo_single.c -Isrc/common
gcc -std=gnu11 src/triangle/demo_all.c -Isrc/common
g++ -std=c++11 src/triangle/tri_random.cpp -Isrc/common -IC:/include -LC:/lib -lopencv_highgui343 -lopencv_imgcodecs343 -lopencv_imgproc343 -lopencv_core343 -lgdi32 -lcomdlg32 -lzlib -lstdc++
g++ -std=c++11 src/triangle/tri_fromfile.cpp -Isrc/common -IC:/include -LC:/lib -lopencv_highgui343 -lopencv_imgcodecs343 -lopencv_imgproc343 -lopencv_core343 -lgdi32 -lcomdlg32 -lzlib -lstdc++
```



```
$ ldd tri_random.exe
        ntdll.dll => /c/WINDOWS/SYSTEM32/ntdll.dll (0x7ffb45550000)
        KERNEL32.DLL => /c/WINDOWS/System32/KERNEL32.DLL (0x7ffb44160000)
        KERNELBASE.dll => /c/WINDOWS/System32/KERNELBASE.dll (0x7ffb42760000)
        ADVAPI32.dll => /c/WINDOWS/System32/ADVAPI32.dll (0x7ffb44220000)
        msvcrt.dll => /c/WINDOWS/System32/msvcrt.dll (0x7ffb44c10000)
        sechost.dll => /c/WINDOWS/System32/sechost.dll (0x7ffb45060000)
        RPCRT4.dll => /c/WINDOWS/System32/RPCRT4.dll (0x7ffb44dd0000)
        comdlg32.dll => /c/WINDOWS/System32/comdlg32.dll (0x7ffb44cb0000)
        combase.dll => /c/WINDOWS/System32/combase.dll (0x7ffb45140000)
        ucrtbase.dll => /c/WINDOWS/System32/ucrtbase.dll (0x7ffb41b30000)
        bcryptPrimitives.dll => /c/WINDOWS/System32/bcryptPrimitives.dll (0x7ffb42640000)
        shcore.dll => /c/WINDOWS/System32/shcore.dll (0x7ffb45470000)
        USER32.dll => /c/WINDOWS/System32/USER32.dll (0x7ffb42a80000)
        win32u.dll => /c/WINDOWS/System32/win32u.dll (0x7ffb41b10000)
        GDI32.dll => /c/WINDOWS/System32/GDI32.dll (0x7ffb429e0000)
        gdi32full.dll => /c/WINDOWS/System32/gdi32full.dll (0x7ffb41c30000)
        msvcp_win.dll => /c/WINDOWS/System32/msvcp_win.dll (0x7ffb426c0000)
        SHLWAPI.dll => /c/WINDOWS/System32/SHLWAPI.dll (0x7ffb44100000)
        SHELL32.dll => /c/WINDOWS/System32/SHELL32.dll (0x7ffb42c10000)
        COMCTL32.dll => /c/WINDOWS/WinSxS/amd64_microsoft.windows.common-controls_6595b64144ccf1df_5.82.17134.345_none_f95245bf3095c4c9/COMCTL32.dll (0x7ffb3a530000)
        cfgmgr32.dll => /c/WINDOWS/System32/cfgmgr32.dll (0x7ffb425f0000)
        windows.storage.dll => /c/WINDOWS/System32/windows.storage.dll (0x7ffb41e80000)
        kernel.appcore.dll => /c/WINDOWS/System32/kernel.appcore.dll (0x7ffb41900000)
        profapi.dll => /c/WINDOWS/System32/profapi.dll (0x7ffb41870000)
        powrprof.dll => /c/WINDOWS/System32/powrprof.dll (0x7ffb418b0000)
        FLTLIB.DLL => /c/WINDOWS/System32/FLTLIB.DLL (0x7ffb41860000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x64940000)
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x61440000)
```
