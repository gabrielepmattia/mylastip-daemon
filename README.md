# MyLastIP Daemon
*Client side of MyLastIP WebApp*
## What's MyLastIP?
MyLastIP is an online service that allows you to see the last IP of your Linux (for now) computers registered. This is useful if your provider assigns to your machines a dynamic IP and you want to use softwares that provide a remote side, for example the [Transmission's (web or cli) remote control](https://trac.transmissionbt.com/wiki/WebInterface) or just a SSH connection to your machine. (In these cases you also have to forward the right ports on your router). In other words, let's suppose you have a pc (or just a Raspberry Pi) with Transmission installed and you want to add/remove/view torrents from the web interface. How do you do if the internet provider gives you a dynamic IP? You'll use some service (even paying for it) that gives you a STATIC ip, but this it's not necessary, we like the dynamic IP. The solution is simple: MyLastIP.

## Instructions
1. Go to [MyLastIP online WebApp](http://mylastip-tuttodinternet.rhcloud.com/) and [register](http://mylastip-tuttodinternet.rhcloud.com/signup);
2. [Login](http://mylastip-tuttodinternet.rhcloud.com/login) and add a device providing a name;
3. Then click to "View" in the row of new device and click "Generate setting file": this will download a **settings.json** file;
4. Now go to [Download section](http://mylastip-tuttodinternet.rhcloud.com/download) and install MyLastIP daemon (or build from git source) with `sudo dpkg -i package.deb`;
5. Now stop the service if active with `sudo service mylastipd stop`
6. Put the previous **settings.json** in the path */etc/mylastip* overwriting if already present
7. Restart the service `sudo service mylastipd start`
8. Login to MyLastIP online WebApp and see the last ip of you machine where daemon is installed 
## Problems?
If you don't manage to see the machine IP on [MyLastIP online WebApp](http://mylastip-tuttodinternet.rhcloud.com/) just do `sudo service mylastip status` to see the log or just hit in the terminal `mylastipd`, then Ctrl-C to exit. Log will tell you where is the problem.

# This git
This is the source of the daemon logs the machine ip to [MyLastIP online WebApp](http://mylastip-tuttodinternet.rhcloud.com/).
## Building
Just `make` or `sudo make package` if you want to build the .deb (Makefile will put it in *_out/*)