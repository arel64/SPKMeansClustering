from setuptools import Extension, setup

#May be needed to define a cap.h file to work.
module = Extension("mykmeanssp", 
                   sources=[
                        'spkmeans.c',
                        'spkmeansmodule.c',
                        'matrix.c',
                        'vector.c'
                   ])
setup(name='mykmeanssp',
      version='1.0',
      description='Python wrapper for spkmeans C extension',
      ext_modules=[module])