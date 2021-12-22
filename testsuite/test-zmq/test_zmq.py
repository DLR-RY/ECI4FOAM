import os
import oftest, pytest
from oftest import run_reset_case
import zmq

def test_zmq(run_reset_case):
    if (not run_reset_case.success):
        oftest.copy_log_files()


    assert (run_reset_case.success)


