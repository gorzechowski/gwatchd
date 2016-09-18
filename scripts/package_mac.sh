#!/bin/bash

OPTIND=1

input=""
output=""
temp="/tmp/gwatchd.tmp.pkg"

currentDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

while getopts "i:o:" opt; do
    case "$opt" in
        i)  input=$OPTARG
            ;;
        o)  output=$OPTARG
            ;;
    esac
done

if [ -z $input ]; then
    echo "Input directory not specified"
    exit 1
fi

if [ -z $output ]; then
    echo "Output file not specified"
    exit 1
fi

pkgbuild --root $input --identifier pl.gorzechowski.gwatchd $temp

productbuild --package-path $temp --distribution "${currentDir}/../dist/distribution.xml" $output

rm $temp
