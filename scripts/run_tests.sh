#!/bin/bash

SCRIPT_DIR=$(dirname $0)
BUILD_DIR=${SCRIPT_DIR}/../build
LD_LIBRARY_PATH=${BUILD_DIR}/src ${BUILD_DIR}/tests/lua++_tests
exit 0

