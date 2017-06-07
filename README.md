# ROBOPI

## On the pi:


* First install GStreamer 1.0 for Video-Streaming. Use this instructions from [wumpus-cave.net](http://www.wumpus-cave.net/2015/11/06/low-latency-fpv-streaming-with-the-raspberry-pi/)

#### OR

* add `wireless-power off` to `/etc/network/interfaces`

* run `sudo apt-get update && sudo apt-get install gstreamer1.0`

* install gstreamer1.0-plugin `gst-rpicamsrc`: 

	```apt-get install git autoconf automake libtool pkg-config libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libraspberrypi-dev```

	`git clone https://github.com/thaytan/gst-rpicamsrc.git`

	`cd gst-rpicamsrc`

	```./autogen.sh --prefix=/usr --libdir=/usr/lib/arm-linux-gnueabihf/```

	```make```

	```sudo make install```

* you may add the ssh-key-pub of the linux pc to pi's `authorized_keys`

	



## On the linux pc:



* Also install GStreamer 1.0:

 `sudo apt-get update && sudo apt-get install gstreamer1.0 libgstreamer1.0-dev gstreamer1.0-plugins-bad gstreamer1.0-tools`

* clone repo:

	`git clone https://github.com/dog42/robopi.git`

	`cd robopi/robo_ctrl`

* open start.sh and change the following lines to your needs:
	```
	piip=robopi
	myip=192.168.1.211
	port=5005
	dev='/dev/hidraw3'
	```

## Run

* just run `./start.sh` in `robopi/robo_ctrl/`

# TODO
## hardware connection











