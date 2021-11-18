import os
from conans import ConanFile, tools


class fmu4foam(ConanFile):
    requires = ["pybind11/2.7.0","nlohmann_json/3.9.1","OpenFOAMGen/0.2@myuser/OpenFOAMGen"]
    generators = "OpenFOAMGen"