#!/bin/bash

defaultTarget="/usr/local/gwatchd"

echo -n "Please enter install location [${defaultTarget}]: "

read target

if [ -z $target ]; then
    target=$defaultTarget
fi

echo -e "Using ${target} as \$target\n"
echo "Copying files to ${target}"

mkdir -p $target

if [ ! -d "$target" ]; then
    echo "Create target directory failed"
    exit 1
fi

cp -a * "$target/"

if [ ! $? -eq 0 ]; then
    echo "Files could not be copied"
    exit 1
fi

echo "All done!"
