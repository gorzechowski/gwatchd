#!/bin/bash

OPTIND=1

input=""
output=""

currentDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

startup="${currentDir}/package_linux_startup.sh"

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

eval "${currentDir}/makeself.sh ${input} ${output} 'GWatchD' ${startup}"
