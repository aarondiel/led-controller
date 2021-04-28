# led-controller::rasp

a led controller intended to control some WS2812B leds via a raspberry pi.

## setup

this implementation uses the gpio userspace character devices under /dev/gpiochipX.
to use them one needs to change the priveleges of these devices.
i recommend to create a gpio user group for this.

```sh
sudo groupadd gpio

// change $user for your username
sudo usermod $user -aG gpio

// permanently channge group of gpio devices
sudo sh -c 'echo SUBSYSTEM==\"gpio\", GROUP=\"gpio\", MODE=\"0660\" >> /etc/udev/rules.d/raspberrypi.rules'

// restart udev
sudo udevadm control --reload
```

## compiling and runnning

to compile simply run

```sh
make
```

to copy the project to a ssh server and run it remotely you can use
(for this you need to edit the settings.env file accordingly)

```sh
make run
```
