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

unzip_so

if [ $# -eq 1 ]
then
    if  [ $1 = 'clean' ]
    then
        cmake --build . --target clean
    else
        if [ $1 = 'test' ]
        then
            cmake . -DBUILD_TEST=1 -DCMAKE_BUILD_TYPE=Release
            cmake --build . -j 16
        elif [ $1 = 'debug' ]
        then
            cmake . -DBUILD_TEST=0 -DCMAKE_BUILD_TYPE=Debug
            cmake --build . -j 16
        fi
    fi
else
    cmake . -DBUILD_TEST=0 -DCMAKE_BUILD_TYPE=Release
    cmake --build . -j 16
fi

