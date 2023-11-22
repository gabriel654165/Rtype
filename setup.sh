#!/bin/bash
rm -rf build && mkdir build && cd build && conan install .. --build=missing && cmake .. && cmake --build . -j8
echo "Bien ouej"
