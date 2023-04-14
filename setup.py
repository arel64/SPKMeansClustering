from setuptools import Extension, setup

#May be needed to define a cap.h file to work.
module = Extension("mykmeanssp", 
                   sources=[
                        'linked_list.c',
                        'vector.c',
                        'matrix.c',
                        'ioparser.c',
                        'spkmeans.c',
                        'kmeanslib.c',
                        'spkmeansmodule.c'
                   ])
setup(name='mykmeanssp',
      version='1.0',
      description='Python wrapper for spkmeans C extension',
      ext_modules=[module])
