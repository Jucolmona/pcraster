#!/bin/bash
set -e
set -v

eval "${MATRIX_EVAL}"

mkdir $TRAVIS_BUILD_DIR/build
cd $TRAVIS_BUILD_DIR/build
echo $CC
echo $CXX

cmake -G"${TRAVIS_CMAKE_GENERATOR}" -DCMAKE_RULE_MESSAGES=OFF -DCMAKE_BUILD_TYPE=${TRAVIS_BUILD_TYPE} -DPCRASTER_BUILD_TEST:BOOL=TRUE -DPYTHON_EXECUTABLE:FILEPATH=/usr/bin/python3 ..
