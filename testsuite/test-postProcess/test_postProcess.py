import pytest
import foam_py
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


    for idx, t in enumerate(times):
        time.setTime(t,idx)
        print(t)
        p_rgh = foam_py.volScalarField("p_rgh", mesh)

        print(np.average(p_rgh["internalField"].to_numpy()))

        print(foam_py.postProcess.compute_average("T",mesh))

        T = foam_py.volScalarField("T", mesh)

        print(np.average(T["internalField"].to_numpy()))

        magU = foam_py.mag(foam_py.volVectorField("U", mesh))

        print(np.average(magU["internalField"].to_numpy()))
    
