from setuptools import setup, Extension

setup_args = dict(
    ext_modules = [
        Extension(
            "crcmod._crcfunext",
            ["lib/_crcfunext.c"],
            py_limited_api = True
        )
    ]
)

setup(**setup_args)

