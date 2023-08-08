[![Build status](https://github.com/ntamas/crcmod-plus/actions/workflows/build.yml/badge.svg)](https://github.com/ntamas/crcmod-plus/actions/workflows/build.yml)

# `crcmod` for calculating CRCs

The software in this repository is a modernised version of the `crcmod` Python
package, originally written by Ray Buvel. I took the original source code of
`crcmod` 1.7, dropped support for Python 2, reformatted the source files with
`black`, ran it through `ruff`, and added a modern build pipeline around it
based on `setuptools`, `pytest`, `tox` and `cibuildwheel` so I can create
pre-compiled wheels for the package using Github Actions.

Version 2.0.0 of this package is a drop-in replacement for `crcmod` 1.7 if
you are running Python 3.7 or later.

The contents of the original README file follows from here, with minor
alterations to the building and testing instructions to describe how it is done
now with the new setup.

---

The software in this package is a Python module for generating objects that
compute the Cyclic Redundancy Check (CRC).  There is no attempt in this package
to explain how the CRC works.  There are a number of resources on the web that
give a good explanation of the algorithms.  Just do a Google search for "crc
calculation" and browse till you find what you need.  Another resource can be
found in chapter 20 of the book "Numerical Recipes in C" by Press et. al.

This package allows the use of any 8, 16, 24, 32, or 64 bit CRC.  You can
generate a Python function for the selected polynomial or an instance of the
Crc class which provides the same interface as the `md5` and `sha` modules
from the Python standard library.  A `Crc` class instance can also generate
C/C++ source code that can be used in another application.

## Guidelines

Documentation is available from the doc strings.  It is up to you to decide
what polynomials to use in your application.  If someone has not specified the
polynomials to use, you will need to do some research to find one suitable for
your application.  Examples are available in the unit tests in the `tests`
folder.  You may also use the `predefined` module to select one of the standard
polynomials.

If you need to generate code for another language, I suggest you subclass the
`Crc` class and replace the method `generateCode`.  Use `generateCode` as
a model for the new version.

## Dependencies

### Python version

This package supports Python 3.7 and newer.

### Building C extension

To build the C extension, the appropriate compiler tools for your platform must
be installed. Refer to the Python documentation for building C extensions for
details. If you have a C compiler and all the usual Python development tools,
including the `build` frontend, then running `python -m build` should build the
package.

## Installation

The `crcmod-plus` package is installed from PyPI using `pip`. Run the following
command:

```sh
$ pip install crcmod-plus
```

## Unit testing

`crcmod-plus` uses `pytest` as the testing environment. You can run the unit
tests with:

```sh
$ pytest tests
```

## Code generation

The `crcmod` package is capable of generating C functions that can be compiled
with a C or C++ compiler.  In the `tests` directory, there is an `examples.py`
script that demonstrates how to use the code generator.  The result of this is
written out to the file `examples.c`.  The generated code was checked to make
sure it compiles with the GCC compiler.

## License

The `crcmod-plus` package is released under the MIT license. See the `LICENSE`
file for details.

## Authors

* Ray Buvel (the original `crcmod` package)

## Contributors

* Craig McQueen (contributor of the original `crcmod` package)
* Tamas Nepusz (modernisation into `crcmod-plus`)
