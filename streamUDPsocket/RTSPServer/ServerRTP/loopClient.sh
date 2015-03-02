#! /bin/bash
# description: to make "RTSPClient" execute repeatly
# author: ws23
# date: 2015.02.06
for (( a=0; a<=10 ;a=a+1)) do
		./RTSPClient $1
done
