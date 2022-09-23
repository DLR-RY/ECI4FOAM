import pytest
import foam_py
from foam_py.time_series import Force
import os
import numpy as np

@pytest.fixture(scope="function")
def change_test_dir(request):
    os.chdir(request.fspath.dirname)
    yield
    os.chdir(request.config.invocation_dir)


def test_postProcess(change_test_dir):

    time = foam_py.Time(".", ".")
    times = foam_py.selectTimes(time,["test_mesh"])
    mesh = foam_py.fvMesh(time)

    f = Force(mesh,["lowerWall"])


    for idx, t in enumerate(times):
        time.setTime(t,idx)
        print(t)
        p_rgh = foam_py.volScalarField("p_rgh", mesh)

        T = foam_py.volScalarField("T", mesh)

        magU = foam_py.mag(foam_py.volVectorField("U", mesh))

        force = f.compute()
        print(force)
    
