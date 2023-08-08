from platform import python_implementation
from setuptools import setup, Extension
from sys import version_info
from wheel.bdist_wheel import bdist_wheel

# fmt: off
can_use_limited_api = (
    python_implementation() == "CPython" and
    version_info >= (3, 11, 0)
)
# fmt: on


class bdist_wheel_abi3(bdist_wheel):
    def get_tag(self):
        python, abi, plat = super().get_tag()

        if can_use_limited_api and python.startswith("cp"):
            # on CPython, our wheels are abi3 and compatible back to 3.11
            return "cp311", "abi3", plat

        return python, abi, plat


extension_kwargs = {}
if can_use_limited_api:
    extension_kwargs.update(
        define_macros=[("Py_LIMITED_API", "0x030b0000")], py_limited_api=True
    )

setup_args = dict(
    ext_modules=[
        Extension("crcmod._crcfunext", ["lib/_crcfunext.c"], **extension_kwargs)
    ],
    cmdclass={"bdist_wheel": bdist_wheel_abi3},
)

setup(**setup_args)  # type: ignore
