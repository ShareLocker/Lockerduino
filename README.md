# Lockerduino
Arduino scripts for the motor based lockers

http://bildr.org/2011/06/arduino-ethernet-pin-control/

# Connection Test

WebClient_Test works as of Tuesday, July 7, 2015. It uses an IP on a local
network not connected to the internet. To get one's IP address use
ifconfig, and it will be somewhere in the output.

To run the server and make it available on the network

    python manage.py runserver {IP}:8000

Where {IP} is the IP address from the last step.
