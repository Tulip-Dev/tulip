from setuptools import setup
from setuptools.dist import Distribution
import platform

class BinaryDistribution(Distribution):
    def is_pure(self):
        return False

    def has_ext_modules(self):
        return True

tulip_native_libs = []
if platform.system() == 'Windows':
    tulip_native_libs = ['native/*.pyd', 'native/*.dll',
                         'native/plugins/*.dll']
elif platform.system() == 'Darwin':
    tulip_native_libs = ['native/*.so', 'native/*.dylib',
                         'native/plugins/*.dylib']
elif platform.system() == 'Linux':
    tulip_native_libs = ['native/*.so*', 'native/plugins/*.so']

setup(
    package_data={'tulip': tulip_native_libs},
    distclass=BinaryDistribution,
)
