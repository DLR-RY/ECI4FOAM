# ECFOAM

Coupling between FMUs and OpenFOAM

# Documentation

[Documentation](https://henningscheufler.github.io/ECI4FOAM/)

## Installation

with conda:
### python

```
conda create -n omsim python=3.9
conda activate omsim
conda install requests
conda install numpy
conda install pandas
conda install -c conda-forge pyfmi 
pip install conan
pip install OMSimulator
pip install oftest
pip install pyzmq
pip install .
```

### OpenFOAM


### download OpenFOAMGen



tested with openfoam version 2012

```
./Allwmake
```

The installation requires python that is used to 




## Test Installation

The FMU are only compatible with glib 2.29 which is shipped with ubuntu 20.04
sudo apt-get install liblapack3
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:PATH_PYTHON3.9
py.test
```


## Build Documentation

The documentation is based on the [Jekyll Documentation Theme](https://idratherbewriting.com/documentation-theme-jekyll/)

```
cd docs
jekyll serve
```