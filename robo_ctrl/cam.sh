#!/bin/bash

port=5000
if  [[ $# == 1 ]]
        then
        port=$1
fi

gst-launch-1.0 udpsrc port=$port \
    ! gdpdepay \
    ! rtph264depay \
    ! avdec_h264 \
    ! videoconvert \
    ! autovideosink sync=false
