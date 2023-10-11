# TODO
C-based utility for interfacing with the waveshare sensor HAT...

# Dependencies ??
* CMake
* CMocka

# Run
```
# create build dir
$ mkdir build && cd build
# create CMake build system
$ cmake -S ..
# compile final WaveShareHAT binary
$ make
# run the binary
$ ./WaveShareHAT
```

## Run Unit Tests
```
$ mkdir build && cd build
$ cmake -DBUILD_TESTS=ON -S ../
$ ./WaveShareTESTS
```


