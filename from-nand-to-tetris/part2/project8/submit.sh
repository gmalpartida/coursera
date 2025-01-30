#!/bin/bash

rm submission/*.c
rm submission/*.h
rm submission/project7.zip

cp inc/*.h submission/
cp src/*.c submission/
echo "debug" > lang.txt
mv lang.txt submission/
cd submission/
zip project7.zip *
