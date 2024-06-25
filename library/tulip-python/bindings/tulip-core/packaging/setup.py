from setuptools import setup
from setuptools.dist import Distribution
import os
from os import path
import fnmatch
import platform
import subprocess
import shutil

# On MacOS, we need to relink dylibs and set correct rpaths in
# order for the modules to be imported in a portable way when
# distributed through pip
if platform.system() == 'Darwin':
    def getDyLibDependencies(dylib):
        otool = subprocess.check_output('otool -L %s' % dylib, shell=True)
        otool = otool.decode('utf-8')
        L = otool.split('\n')[1:-1]
        L = map(lambda d: d[1:].split(' ')[0], L)
        L = filter(lambda d: '/usr/lib' not in d and
                             '/System' not in d and
                             'Python' not in d and
                             '@rpath' not in d, L)
        return set(L)

    def relinkDyLibAndSetRpaths(dylib, rpaths=[]):
        libs = getDyLibDependencies(dylib)
        for L in libs:
            subprocess.call(
                'install_name_tool -change %s @rpath/%s %s 2>/dev/null' %
                (L, os.path.basename(L), dylib),
                shell=True)
        # add rpaths in dylib
        for rpath in rpaths:
            subprocess.call(
                'install_name_tool -add_rpath %s %s 2>/dev/null' %
                (rpath, dylib),
                shell=True)

    def relinkDyLibsInDirAndSetRpaths(dirPath, rpaths):
        for lib in os.listdir(dirPath):
            if not fnmatch.fnmatch(lib, '*.py'):
                libPath = dirPath + lib
                if not os.path.isdir(libPath):
                    relinkDyLibAndSetRpaths(libPath, rpaths)

    def copyDyLibsDependencies(dirPath, relativeCopyPath='./'):
        lastNbProcessed = 0
        toProcess = os.listdir(dirPath)
        while lastNbProcessed == 0 or len(toProcess) > lastNbProcessed:
            for lib in toProcess:
                if not fnmatch.fnmatch(lib, '*.py'):
                    libPath = dirPath + lib
                    if not os.path.isdir(libPath):
                        libs = getDyLibDependencies(libPath)
                        for L in libs:
                            filename = os.path.basename(L)
                            dylibCopy = dirPath + relativeCopyPath + filename
                            if not os.path.isfile(dylibCopy):
                                shutil.copyfile(L, dylibCopy)
                                subprocess.call(
                                    ('install_name_tool -id @rpath/%s %s '
                                     '2>/dev/null') % (filename, dylibCopy),
                                    shell=True)
            lastNbProcessed = len(toProcess)
            toProcess = os.listdir(dirPath)

    tulipNativeLibsPath = os.getcwd() + '/tulip/native/'
    tulipNativePluginsLibsPath = os.getcwd() + '/tulip/native/plugins/'

    print("Copying dylibs dependencies in tulip-python wheel ...")
    copyDyLibsDependencies(tulipNativeLibsPath)
    copyDyLibsDependencies(tulipNativePluginsLibsPath, '../')
    print("Relinking dylibs and setting rpaths in tulip-python wheel ...")
    relinkDyLibsInDirAndSetRpaths(tulipNativeLibsPath, ['@loader_path/'])
    relinkDyLibsInDirAndSetRpaths(tulipNativePluginsLibsPath,
                                  ['@loader_path/..'])


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
