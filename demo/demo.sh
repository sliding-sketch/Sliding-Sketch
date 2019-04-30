#!/bin/bash

cd ../src/Frequency/Sliding_Sketch/demo
make clean
make
./main
make clean

cd ../../ECM/demo
make clean
make
./main
make clean

cd ../../SWCM/demo
make clean
make
./main
make clean

cd ../../../HeavyHitter/Sl_HK/demo/
make clean
make
./main
make clean

cd ../../lambda_Algorithm/demo
make clean
make
./main
make clean

ulimit -s 1024000

cd ../../WCSS/demo
make clean
make prec
make recall
./prec
./recall
make clean

cd ../../../Membership
make clean
make
./main
make clean


