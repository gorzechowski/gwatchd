#!/bin/bash

OPTIND=1

input=""
output=""

currentDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

while getopts "i:o:" opt; do
    case "$opt" in
        i)  input="$(pwd)/$OPTARG"
            ;;
        o)  output=$OPTARG
            ;;
    esac
done

if [ -z $input ]; then
    echo "Input directory not specified"
    exit 1
fi

if [[ ! -d $input ]]; then
    echo "Input is not directory"
    exit 1
fi

if [ -z $output ]; then
    echo "Output file not specified"
    exit 1
fi

temp="gwatchd.pkg"

pkgbuild --root $input --identifier pl.gorzechowski.gwatchd "${temp}"

productbuild --package-path "${temp}" --distribution "${currentDir}/../dist/distribution.xml" "${output}"

rm "$temp" && echo "Temporary ${temp} file removed"
