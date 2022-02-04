import os
import pytest
import oftest
from oftest import run_case, run_reset_case
import shutil
from glob import glob

def test_dataLayer(run_reset_case):

    assert (run_reset_case.success)
