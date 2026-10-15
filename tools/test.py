#!/usr/bin/env python3
"""Compile and execute the production portable core, without ESP-IDF mocks."""
import argparse
from pathlib import Path
import subprocess

parser = argparse.ArgumentParser()
mode = parser.add_mutually_exclusive_group()
mode.add_argument('--sanitize', action='store_true')
mode.add_argument('--ubsan', action='store_true')
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
out = root / 'build-native'
out.mkdir(exist_ok=True)
command = ['g++', '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic', '-g',
           '-Icomponents/ddr_core/include', 'components/ddr_core/ddr_core.cpp',
           'tests/test_core.cpp', '-o', str(out / 'test_core')]
if args.sanitize:
    command += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
elif args.ubsan:
    command += ['-fsanitize=undefined', '-fno-omit-frame-pointer']
subprocess.run(command, cwd=root, check=True)
subprocess.run([str(out / 'test_core')], cwd=root, check=True)
