import os
import pytest
import subprocess as sp
import time

folders = ["bind", "minimal", "good", "syntax", "type"]
param_filepaths = []
for folder in folders:
    param_filepaths += [(open(f"{folder}/{file}", 'r').name, f"{folder}/{file}") for file in os.listdir(folder)]


@pytest.mark.parametrize("name,filepath", param_filepaths)
def test_file(name, filepath):
    executable = "../src/tc"

    proc = sp.Popen([executable, "-XA", "--parse", filepath], stdout=sp.PIPE, stderr=sp.STDOUT, bufsize=0)
    try:
        out, err = proc.communicate(timeout=0.5)
        assert err is None
        assert out is not None
        assert proc.returncode == 0

        proc_piped = sp.Popen([executable, "-"], stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.STDOUT, bufsize=0)

        proc_piped.stdin.write(out)
        proc_piped.stdin.close()

        try:
            out_piped, err_piped = proc.communicate(timeout=0.5)
            assert out_piped is not None
            assert err_piped is None
            assert proc_piped.returncode == 0
        finally:
            proc_piped.kill()

    finally:
        proc.kill()
