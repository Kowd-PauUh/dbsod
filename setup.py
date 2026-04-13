import glob

from setuptools import setup, Extension
import pybind11


ext_modules = [
    Extension(
        "dbsod.dbsod_cpp",
        sources=glob.glob("dbsod_cpp/*.cpp"),
        include_dirs=[
            "include",
            pybind11.get_include(),
        ],
        language="c++",
        extra_compile_args=["-O3", "-std=c++23", "-Wall", "-fPIC"],
    )
]

setup(
    ext_modules=ext_modules,
    zip_safe=False,
)
