import os
import pytest
import oftest
from oftest import run_case, run_reset_case
import shutil
from glob import glob

def test_dataLayer(run_reset_case):
    if (not run_reset_case.success):
        oftest.copy_log_files()

    assert (run_reset_case.success)
