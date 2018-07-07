#! /bin/bash
# perform all the necessary steps for automake
# aclocal
# autoconf
# autoheader
# automake --add-missing
# mkdir build
cd build
../configure
make
sudo make install
