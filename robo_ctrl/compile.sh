#!/bin/bash
gcc -o robo_control -pthread -lm robo_control.c
gcc -o robo_control6 -pthread -lm robo_control_ipv6.c
