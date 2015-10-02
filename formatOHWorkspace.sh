#!/bin/zsh

function fmtproject
{
    (cd $1 && ../formatAll.sh && git commit -a -m "automatic clang-format" && git push)
}

folders=(AutoSyncGen DelversChoice DiscountCpp LD27 NetLayer SSVBloodshed SSVEntitySystem SSVMenuSystem SSVNewRogue SSVOpenHexagon SSVSCollision SSVStart SSVUtils TestUDPChat WEBVittorioRomeo2 git-ws vrm_pp)

for var in "${folders[@]}"
do
    echo "!!!!! formatting $var"
    fmtproject $var
done