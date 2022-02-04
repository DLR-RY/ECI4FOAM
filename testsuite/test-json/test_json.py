import os
import oftest
from oftest import run_reset_case
import json

def test_json(run_reset_case):
    dir_name = oftest.base_dir()
    with open(os.path.join(dir_name,"test.json")) as f:
        d = json.load(f)
    assert (d["pi"] == 3.141)

    assert (run_reset_case.success)
