* Identical assembly for `godbolt_async.cpp` and `godbolt_async_manual.cpp` on:
    * **g++ 7.0** `-std=c++1z -O3`.
    * **clang++ 3.9.0** `-std=c++1z -O3`.

* **Almost** identical assembly for `godbolt_waitall.cpp` and `godbolt_waitall_manual.cpp` on:
    * **g++ 7.0** `-std=c++1z -O3`.
    * **clang++ 3.9.0** `-std=c++1z -O3`.
    * The async chain version has 10 extra lines due to the presence of `movable_atomic`.
        * Putting the atomic into the `ll::context` class reduces the extra asm by 7 lines.
