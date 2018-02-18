# picusduino
Repo contains sketches for controlling arduino pins over serial connection

Note:  picusduino.ino   pairs with picuspy.pindriver

## picusduino
Sketch for communicating with Arduino Mega or Uno (expandable to other types).

  **Depends on** Arduino Board Manager Library (see links in the README [here](https://github.com/niuroverteam/libraries)).
  
  Uses picuspy.pindriver to manage pin modes and communications.
  
  picuspy.pindriver automatically changes pinMode when needed.
  
  Will raise assertion error if actions are taken on an invalid pin (according to board type).
  
  For usage see [picuspy/examples/gpio](https://github.com/NIURoverTeam/picuspy/tree/master/examples/gpio)


## MEGA_pin_driver *(to be phased out)*
Old script for communicating with Arduino Mega.
  
  Currently used in picuspy/rover.py
