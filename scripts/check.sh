#!/bin/bash

currentDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo `whoami`

echo $currentDir

cd $currentDir/..

qmake -r && make -j3 && make check
