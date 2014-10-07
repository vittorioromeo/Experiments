#!/bin/bash 

function finish 
{
	xrandr --output HDMI-0 --off
	xrandr --output DVI-I-0 --off

	xrandr --output HDMI-0 --primary --mode 1920x1080 --pos 0x0 
	xrandr --output DVI-I-0 --mode 1680x1050 --pos -1920x0 
}

trap finish EXIT

xrandr --output HDMI-0 --off
xrandr --output DVI-I-0 --off

xrandr --output DVI-I-0 --off
xrandr --output HDMI-0 --primary --mode 640x480

export WINEDEBUG=-all
WINEDEBUG=-all wine "$@"

sleep 2

echo "looking for processes"

echo "output"
echo $(ps -A | grep -P "((th|TH)[0-9]+.*)|vpatch.*" | sed 's/^ *//' | cut -d' ' -f1) 

processes=( $(ps -A | grep -P "((th|TH)[0-9]+.*)|vpatch.*" | sed 's/^ *//' | cut -d' ' -f1) )


for i in "${processes[@]}"
do
	echo "process ${i} $i"
	while [ -e "/proc/$i" ]; do sleep 1; done
	echo "done."
done

exit