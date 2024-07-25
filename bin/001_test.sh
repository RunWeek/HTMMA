#!/bin/bash
cd ../build
rm -rf *
rm ../lib/* -rf
cmake ..
make 
cd -
#export OMP_NUM_THREADS=1
#echo pykuse62.com|sudo -S ./htmma 

