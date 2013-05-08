#!/bin/bash

DIR="Project TBIR"

rmtrash tbir

#clang++ -stdlib=libc++ -std=c++11 -O3 -mtune=native "$DIR/main.cpp" "$DIR/io.cpp" "$DIR/section.cpp" "$DIR/subclass.cpp" "$DIR/class.cpp" "$DIR/patent.cpp" "$DIR/evaluate.cpp" "$DIR/tc1.cpp" "$DIR/tc2.cpp" "$DIR/tc3.cpp" "$DIR/tc4.cpp" "$DIR/tc5.cpp" "$DIR/tc6.cpp" "$DIR/tc7.cpp" -o tbir
g++-mp-4.8 -std=c++11 -O3 -mtune=native "$DIR/main.cpp" "$DIR/io.cpp" "$DIR/section.cpp" "$DIR/subclass.cpp" "$DIR/class.cpp" "$DIR/patent.cpp" "$DIR/evaluate.cpp" "$DIR/tc1.cpp" "$DIR/tc2.cpp" "$DIR/tc3.cpp" "$DIR/tc4.cpp" "$DIR/tc5.cpp" "$DIR/tc6.cpp" "$DIR/tc7.cpp" -o tbir