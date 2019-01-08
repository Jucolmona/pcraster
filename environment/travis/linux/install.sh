# Source this script to keep settings valid for other scripts!
#!/bin/bash
set -e
set -v

eval "${MATRIX_EVAL}"

cwdir=${PWD}

sudo apt-get install qt56base
export PATH=/opt/qt56/bin:$PATH

sudo apt-get install libqwt-qt5-dev

sudo apt-get install gdal-bin libgdal-dev libxerces-c-dev libxml2-dev libxslt1-dev libboost-all-dev libncurses5-dev libpython-dev libxml2 libxml2-utils mesa-common-dev cmake



cd $TRAVIS_BUILD_DIR
pip install --user --upgrade pip
pip install --user --upgrade numpy
pip install --user --upgrade docopt
pip install --user --upgrade sphinx
# pip install --user --upgrade pillow
pip install --user --upgrade psutil


# mkdir $TRAVIS_BUILD_DIR/local

# cd $TRAVIS_BUILD_DIR/local
# tar zxf ../cmake-3.12.1-Linux-x86_64.tar.gz
# export PATH=$TRAVIS_BUILD_DIR/local/cmake-3.12.1-Linux-x86_64/bin:$PATH

# cd $TRAVIS_BUILD_DIR/peacock
# mkdir build
# cd build
# echo $CC
# echo $CXX
# cmake -G"${TRAVIS_CMAKE_GENERATOR}" -Dpeacock_prefix=$TRAVIS_BUILD_DIR/local -Dbuild_qwt=true -Dqwt_version=6.1.2 ..
# cmake --build . --target all -- -j2


# Return to initial directory
cd ${cwdir}
