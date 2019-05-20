#!/bin/bash
make cleanC
make clean
make parser
make test

# for file in $(find ../Test_2_Super/ -name "single-*.cmm")
# do
#     echo ${file}
#     ./parser ${file}
# done
