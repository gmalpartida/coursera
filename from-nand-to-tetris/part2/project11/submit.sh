#!/bin/bash

rm submission/*.c
rm submission/*.h
rm submission/project11.zip

cp inc/*.h submission/
cp src/*.c submission/
echo "debug" > submission/lang.txt
cd submission/
zip project11.zip *
