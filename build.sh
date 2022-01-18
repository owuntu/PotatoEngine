#!/bin/sh -x

unzip_so()
{
    SO_DIR='./Engine/Extern/dll'
    cd $SO_DIR
    if [ ! -f libassimp.so ]
    then
        unzip  libassimp.zip
        if [ ! -f libassimp.so.5 ]
        then
            ln -s libassimp.so libassimp.so.5
        fi
    fi
    cd -
}

if [ $# -eq 1 ]
then
    if [ $1 = 'test' ]
    then
        unzip_so
        cmake . -DBUILD_TEST=1
        cmake --build . -j 16
    elif [ $1 = 'clean' ]
    then
        cmake --build . --target clean
    fi
else
    unzip_so
    cmake . -DBUILD_TEST=0
    cmake --build . -j 16
fi

