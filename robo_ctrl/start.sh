#!/bin/bash
piip=robopi
myip=192.168.1.211
port=5005
dev='/dev/hidraw3'

./cam.sh &
ssh pi@$piip "cd /home/pi/robopi/; ./autostart.sh $myip; exit;" &
sleep 2
echo '>>>> WAIT...'
sleep 3
echo '>>>> WAIT...'
sleep 3
echo '>>>> START...'
sudo ./robo_control $piip $port $dev 1

killall gst-launch-1.0
killall robo

