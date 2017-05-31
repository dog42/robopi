#!/bin/bash

gst-launch-1.0 rpicamsrc bitrate=1000000 \
    ! 'video/x-h264,width=640,height=480' \
    ! h264parse \
    ! queue \
    ! rtph264pay config-interval=1 pt=96 \
    ! gdppay \
    ! udpsink host=[INSERT_IP_ADDR] port=5000
