How to build Mali Midgard kernel device drivers?
================================================

1. cd kernel
2. export KDIR=<kernel root directory>
3. make profile=<profile>

<kernel root directory> is the root directory of kernel source, which is ready for compiling device drivers.

<profile> is the setting file which contains the pre-preackaged set of options
The available profile could be found under directory 'bldsys/profiles'.
e.g. monaco-android-kitkat, monaco-supernova

All binaries (*.ko) would be put in directory 'bin'.

Type following command to clean device drivers:
make profile=<profile> clean
