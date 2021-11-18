import os
from conans import ConanFile, tools


class fmu4foam(ConanFile):
    requires = ["cppzmq/4.7.1","nlohmann_json/3.9.1","OpenFOAMGen/0.2@myuser/OpenFOAMGen"]
    generators = "OpenFOAMGen"

    def imports(self):
        
        self.copy("*.hpp", "lnInclude", "include")
        # self.copy("*.lib", os.path.join('..','..','resources','binaries','win64'), "lib")
        # self.copy("*.a", os.path.join('..','..','resources','binaries','linux64'), "lib")
        # self.copy("*.so", os.path.join('..','..','resources','binaries','linux64'), "lib")