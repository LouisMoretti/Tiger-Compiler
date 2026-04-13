import os

import pytest
import subprocess as sp

folders = ["tests/bind", "tests/minimal", "tests/good", "tests/syntax", "tests/type"]
param_filepaths = []
for folder in folders:
    param_filepaths += [(open(f"{folder}/{file}", 'r').name, folder.split("/")[-1]) for file in os.listdir(folder)]

def get_ref_return_code(test_type: str) -> int:
    if test_type == "minimal" or test_type == "good":
        return 0
    elif test_type == "syntax":
        return 3
    elif test_type == "bind":
        return 4
    elif test_type == "type":
        return 5
    return 0

def get_mode(test_type : str) -> str:
    if test_type == "minimal" or test_type == "good":
        return "-bBA"
    elif test_type == "syntax":
        return "-A"
    elif test_type == "bind":
        return "-bBA"
    elif test_type == "type":
        return "-AT"
    return "-A"

@pytest.mark.parametrize("filepath,test_type", param_filepaths)
def test_file(filepath, test_type):
    executable = "./src/tc"

    proc = sp.Popen([executable, get_mode(test_type), "--parse", filepath], stdout=sp.PIPE, stderr=sp.STDOUT, bufsize=0)
    try:
        out, err = proc.communicate(timeout=0.5)
        assert err is None
        assert out is not None
        assert proc.returncode == get_ref_return_code(test_type)

        if test_type == "minimal" or test_type == "good":
            proc_piped = sp.Popen([executable, "-"], stdin=sp.PIPE, stderr=sp.STDOUT, bufsize=0)

            proc_piped.stdin.write(out)
            proc_piped.stdin.close()

            try:
                out_piped, err_piped = proc_piped.communicate()
                assert out_piped is None
                assert err_piped is None
                assert proc_piped.returncode == 0
            finally:
                proc_piped.kill()
    finally:
        proc.kill()
