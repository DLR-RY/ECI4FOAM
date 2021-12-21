#!/bin/sh
cd "${0%/*}" || exit                                # Run from this directory

git submodule update --init --recursive
pip install oftest conan

#install generator
cd OpenFOAMGen/OpenFOAMGen
conan export . myuser/OpenFOAMGen
cd ../..
# compile ECI4FOAM
./Allwmake

