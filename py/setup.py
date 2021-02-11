#!/usr/bin/env python
from distutils.spawn import find_executable
from subprocess import run
import setuptools

# Generate sources from the flatbuffers compiler.
FLATC = find_executable('flatc')
if FLATC is None:
    raise FileNotFoundError('Cannot find flatc. Is it in your PATH?')
run([FLATC, '--python', '../batprotocol.fbs'], check=True)

# Usual setuptools stuff.
setuptools.setup(
    name='batprotocol-py',
    version='0.1.0',
    packages=setuptools.find_packages(),
    python_requires='>=3.6',
    install_requires=[
        'flatbuffers>=1.12.0'
    ],
    description="Batsim Protocol Python Library.",
    author='Millian Poquet',
    author_email='millian.poquet@gmail.com',
    url='https://framagit.org/batsim/batprotocol',
    license='LGPL-3.0',
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
    ],
)
