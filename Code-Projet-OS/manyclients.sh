#!bin/bash

for j in $(seq 1 20)
do
	./wclient localhost 8000 /spin.cgi?10 &
	sleep 1
done
