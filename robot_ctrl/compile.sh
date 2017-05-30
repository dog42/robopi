#!/bin/bash
gcc -o robo -lwiringPi -lwiringPiDev -lpthread -lm robo_control.c
