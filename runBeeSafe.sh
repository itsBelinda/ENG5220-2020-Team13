#!/bin/sh

cd software/BeeSafePI/build/
cmake ..
make
cd src/
./BeeSafePI
