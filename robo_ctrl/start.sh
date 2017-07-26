#!/bin/bash
piip=192.168.2.66
myip=$(ifconfig eth0 | grep "inet " |cut -f2 -d: |tr " " ":"|cut -f1 -d:)
port=5005
dev='/dev/hidraw3'
camport=5000
sshport=666
./cam.sh &
ssh -p$sshport pi@$piip "cd /home/pi/robopi/; ./autostart.sh $myip $camport; exit;" &
sleep 2
echo '>>>> WAIT...'
sleep 3
echo '>>>> WAIT...'
sleep 3
echo '>>>> START...'

sudo ./robo_control $piip $port $dev 1

killall gst-launch-1.0
killall robo

