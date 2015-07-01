#!/bin/sh -ex

mkdir build
cd build
cmake .. $@
make
make test
make bench
make install
cd ..
rm -rf build

