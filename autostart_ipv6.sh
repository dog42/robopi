#!/bin/bash
./asurostart.sh &
./startcam.sh $1 $2 &
echo '>>>> WAIT...'
sleep 5
robo/robo6
killall gst-launch-1.0
killall robo6
