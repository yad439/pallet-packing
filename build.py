import platform
import shutil
import subprocess
import sys


def build_with_cmake():
    build_dir = 'cpp/build'
    src_dir = 'cpp'
    cmake = 'cmake'

    cmake_generator = []
    print(sys.argv)
    if len(sys.argv) == 2:
        cmake_generator = ['-G', sys.argv[1]]
    elif platform.system() == 'Windows':
        if subprocess.run([cmake, src_dir, '-B', build_dir]).returncode != 0:
            cmake_generator = ['-G', 'MinGW Makefiles']
            shutil.rmtree(build_dir)
    else:
        cmake_generator = []

    subprocess.run([cmake, src_dir, '-B', build_dir, '-DCMAKE_BUILD_TYPE=Release'] + cmake_generator)
    subprocess.run([cmake, '--build', build_dir, '--config', 'Release', '--target', 'install'])


if __name__ == '__main__':
    build_with_cmake()
