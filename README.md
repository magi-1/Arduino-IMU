# Ergo
Prototyping an IMU based personal trainer for fitness

Currently sending data over wifi directly to a google sheet. Turns out the default request latency for google cloud is way too low given the IMU sample rate. Will prototype using serial -> csv for now.
