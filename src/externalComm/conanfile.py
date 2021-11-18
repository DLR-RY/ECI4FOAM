import os
from conans import ConanFile, tools


class fmu4foam(ConanFile):
    requires = ["cppzmq/4.7.1","nlohmann_json/3.9.1","OpenFOAMGen/0.2@myuser/OpenFOAMGen"]
    generators = "OpenFOAMGen"

    def imports(self):
        self.copy("*.hpp", "lnInclude", "include")
