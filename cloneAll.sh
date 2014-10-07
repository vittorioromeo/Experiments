#!/bin/bash

testArray=("SSVUtils" "SSVUtilsJson" "SSVMenuSystem" "SSVEntitySystem" "SSVLuaWrapper" "SSVStart" "git-ws" "WEBVittorioRomeo2" "SSVNewRogue" "SSVSCollision" "SSVOpenHexagon" "SSVRPGSystem" "SSVBloodshed" "cppcon2014" "Tutorials" "Experiments" "UNIME" "TestUDPChat" "WEBVittorioRomeo2" "SSVCMake" "LD27" "DiscountCpp")

for repo in "${testArray[@]}"; do
	git clone "https://github.com/SuperV1234/${repo}.git"
	(cd $repo; ./init-repository.sh)
done