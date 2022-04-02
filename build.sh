#!/bin/bash

function rand_suffix() {
    echo -n $RANDOM
}

export PrjName=`basename $PWD`

/mnt/Texas/prj/ue4-4.24.3-html5/Engine/Binaries/ThirdParty/Mono/Linux/bin/mono \
    /mnt/Texas/prj/ue4-4.24.3-html5/Engine/Binaries/DotNET/UnrealBuildTool.exe \
    -ModuleWithSuffix=${PrjName},$(rand_suffix) \
    ${PrjName}Editor \
    Linux \
    Development \
    -Project="/home/mika/prj/${PrjName}/${PrjName}.uproject" \
    "/home/mika/prj/${PrjName}/${PrjName}.uproject"  \
    -IgnoreJunk \
    -progress
