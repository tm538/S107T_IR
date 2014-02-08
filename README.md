S107T_IR
========

Mbed library for controlling Smya 107T infra red controlled helicopter 

accepts a control string via serial of the format

xxx,xxx,xxx\n

chars 0-2 are throttle values (0-127)
chars 4-6 are pitch (0-127, 63 is neutral pitch) 
chars 8-10 are yaw (0-127, 63 is neutral yaw) 
