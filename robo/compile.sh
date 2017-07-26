#!/bin/bash
gcc -o robo -lwiringPi -lwiringPiDev -lpthread -lm robo.c
gcc -o robo6 -lwiringPi -lwiringPiDev -lpthread -lm robo_ipv6.c
