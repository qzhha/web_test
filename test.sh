#!/bin/bash
cd build
rm -rf * -y 
cmake ..
make 

echo "--------------------------"
echo "start test ..."
sleep 1s
./test

echo "end test ..."
