#!/bin/bash

rm submission/*.c
rm submission/*.h
rm submission/project8.zip

cp inc/*.h submission/
cp src/*.c submission/
echo "debug" > lang.txt
mv lang.txt submission/
cd submission/
zip project8.zip *
