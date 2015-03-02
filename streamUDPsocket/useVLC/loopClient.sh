#! /bin/bash
# description: to make "RTSPClient" execute repeatly
# author: ws23
# date: 2015.02.06
for (( a=0; a<=10000 ;a=a+1)) do
		mplayer $1
done
