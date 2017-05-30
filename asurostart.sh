#!/bin/bash
echo 23 > /sys/class/gpio/export
echo 24 > /sys/class/gpio/export
echo 25 > /sys/class/gpio/export
echo 8 > /sys/class/gpio/export
sleep 2
echo "out"> /sys/class/gpio/gpio23/direction
echo "out"> /sys/class/gpio/gpio24/direction
echo "out"> /sys/class/gpio/gpio25/direction
echo "out"> /sys/class/gpio/gpio8/direction 

