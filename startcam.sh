#!/bin/bash

ip=192.168.1.211
port=5000
if  [[ $# == 1 ]]
	then
	ip=$1
elif [[ $# == 2 ]]
	then
        ip=$1
	port=$2
fi

echo Verbinde zu: $ip:$port

gst-launch-1.0 rpicamsrc bitrate=1000000 \
    ! 'video/x-h264,width=640,height=480' \
    ! h264parse \
    ! queue \
    ! rtph264pay config-interval=1 pt=96 \
    ! gdppay \
    ! udpsink host=$ip port=$port
