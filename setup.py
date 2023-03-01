from setuptools import Extension, setup

#May be needed to define a cap.h file to work.
module = Extension("mykmeansssp", 
                   sources=[
                       'spkmeans.c',
                       'spkmeansmodule.c'
                   ])
setup(name='mykmeansssp',
      version='1.0',
      description='Python wrapper for spkmeans C extension',
      ext_modules=[module])