#!/bin/sh

cd ENG5220-2020-Team13/software/BeeSafePI/build/
cmake ..
make
cd src/
./BeeSafePI
