import pytest
import foam_py
import os

@pytest.fixture(scope="function")
def change_test_dir(request):
    os.chdir(request.fspath.dirname)
    yield
    os.chdir(request.config.invocation_dir)

def test_mesh(change_test_dir):
    mesh = foam_py.FvMesh()


def test_ofdict(change_test_dir):

    d = foam_py.dictionary("system/controlDict")

    print(d.toc())
    # assert d.value("endTime") == 0

def test_volScalarField(change_test_dir):

    mesh = foam_py.FvMesh()

    p_rgh2 = foam_py.volScalarField("p_rgh",mesh)

    p_rgh = foam_py.volScalarField("p_rgh",mesh)
    assert sum(p_rgh["internalField"]) == 0
    p_rgh["internalField"] += 1
    assert sum(p_rgh["internalField"]) == p_rgh.internalField.size

    p_rgh2["internalField"] += 1
    print(p_rgh2["internalField"])
    test = p_rgh2 + p_rgh
    # print(test["internalField"])

    assert sum(p_rgh["leftWall"]) == 0
    p_rgh["leftWall"] += 1
    assert sum(p_rgh["leftWall"]) == p_rgh.bField("leftWall").size

    U = foam_py.volVectorField("U",mesh)
    assert sum(U["internalField"]) == 0
    U["internalField"] += 2
    assert sum(U["internalField"]) == U["internalField"].size*2



