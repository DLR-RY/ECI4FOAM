import pytest
import foam_py
import os

@pytest.fixture(scope="function")
def change_test_dir(request):
    os.chdir(request.fspath.dirname)
    yield
    os.chdir(request.config.invocation_dir)

def test_mesh(change_test_dir):
    p = foam_py.FoamMesh()
    assert p.time() == 0


def test_ofdict(change_test_dir):

    p = foam_py.dictionary("system/controlDict")

    print(p.toc())
    assert p.value("endTime") == 0

