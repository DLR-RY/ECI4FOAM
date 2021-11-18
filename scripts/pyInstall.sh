#!/bin/bash

function pylib {
    PYVERSION="-lpython"$(python --version | grep -o -P '3.{0,2}')
    echo $PYVERSION
}

function pyincludes {
    PYINC=$(python3-config --includes | sed 's/ / \\ \\n/g')
    # echo $($PY_INCS | sed 's/ / \\ \\n/g')
    includes="PY_INCS := \ \n
        -Wno-old-style-cast \ \n""$PYINC"
        
    echo -e $includes
}

function pylibs {
    flags=$(python3-config --ldflags)
    PYFLAGS="PY_LIBS := \ \n"
    for f in $flags
    do
        if [[ $f == *"-L"* ]]; then
            PYFLAGS+="$f \ \n"
        fi
    done
    if [[ $1 = "-rpath" ]]; then
        rpath=$(pyrpath)
        PYFLAGS+="$rpath \ \n"
    fi
    PYFLAGS+=$(pylib)
    echo -e $PYFLAGS
}

function pyrpath {
    PYPREFIX=$(python3-config --prefix)
    echo "-Wl,-rpath="$PYPREFIX"/lib"
}
