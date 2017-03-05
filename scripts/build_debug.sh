#!/bin/bash

SCRIPT_DIR=$(dirname $0)
BUILD_DIR=${SCRIPT_DIR}/../build
NUM_CPUS=$(cat /proc/cpuinfo | grep processor | wc -l)

set -e
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j ${NUM_CPUS}
cd -
exit 0

