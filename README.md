# crcmod for Calculating CRCs

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
your application.  Examples are available in the unit test script `test.py`.
You may also use the `predefined` module to select one of the standard
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
details.

## Installation

The crcmod package is installed using `pip`. Run the following command:

```sh
$ pip install crcmod
```

## Unit testing

The `crcmod` package has a module `crcmod.test`, which contains unit
tests for both `crcmod` and `crcmod.predefined`.

When you first install `crcmod`, you should run the unit tests to make sure
everything is installed properly.  The test script performs a number of tests
including a comparison to the direct method which uses a class implementing
polynomials over the integers mod 2.

To run the unit tests:

```sh
$ python -m crcmod.test
```

## Code generation

The `crcmod` package is capable of generating C functions that can be compiled
with a C or C++ compiler.  In the test directory, there is an `examples.py`
script that demonstrates how to use the code generator.  The result of this is
written out to the file `examples.c`.  The generated code was checked to make
sure it compiles with the GCC compiler.

## License

The `crcmod` package is released under the MIT license. See the `LICENSE`
file for details.

## Contributors

Craig McQueen
