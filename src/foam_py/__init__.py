from ._version import __version__
import sys
import os 

class foam_pyPaths(object):
    foam_py_install_path = os.path.abspath(os.path.join(os.path.dirname(__file__)))

    foam_py_libs = os.path.join(foam_py_install_path)

sys.path.append(foam_pyPaths.foam_py_libs)

from foam_py_core import *