#!/bin/sh
cd "/run/media/vittorioromeo/8CDA3737DA371D40/Riot Games/League of Legends/RADS/system/"
WINEDEBUG=-all wine "rads_user_kernel.exe" run lol_launcher $(ls ../projects/lol_launcher/releases/) LoLLauncher.exe

 