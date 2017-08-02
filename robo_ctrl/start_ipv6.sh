#!/bin/bash
pimac="b8:27:eb:b6:d6:06"
piip="AF11::1"
myip="AF11:1000::2"
port=5006
camport=5000
sshport=666
sudo ip -6 addr add $myip/16 dev eth0
sudo ip -6 neigh add $piip lladdr $pimac dev eth0
./cam.sh &
ssh -p$sshport pi@$piip "cd /home/pi/robopi/; ./autostart_ipv6.sh $myip $camport; exit;" &
sleep 2
echo '>>>> WAIT...'
sleep 3
echo '>>>> WAIT...'
sleep 3
echo '>>>> START...'
sudo ./robo_control6 $piip $port /dev/hidraw3 1

killall gst-launch-1.0
killall robo

