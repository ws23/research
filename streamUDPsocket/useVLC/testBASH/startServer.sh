#! /bin/bash
# startServer.sh
# description: to startServer 
# author: ws23
# date: 2015. 3. 13

MEDIAFILE="test.mp3"
PORT=8554
STREAM="test"

func1() {
	./timing.out >> end.tmp
	./makeRec.out start.tmp end.tmp time.txt
	rm -rf start.tmp end.tmp
	exit
}

func2() {
	rm -rf time.txt
	echo "0" >> time.txt
}

./timing.out >> start.tmp

trap func1 INT

./execVLC.out $MEDIAFILE $PORT $STREAM time.txt
func2
