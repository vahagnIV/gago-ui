# GagoUI

This is the UI part of a stereo vision project Gago.

Currently, the only useful feature it has is its calibration module. I will try to add other modules along the time.

ACHTUNG: because GagoUI depends on LibGago and the latter implements the webcam driver only for linux (tested on Ubuntu 18.04), currently you cannot run it on other Operating Systems.
Please see the Developer Notes for more info.

## Installation 

### Dependencies

First install LibGago from [here](https://github.com/vahagnIV/libgago).

Next install the dependencies

```bash
sudo apt-get install -y libopencv-dev qt5-core ...
```

Finally 


```bash
git clone git@github.com:vahagnIV/gago-ui.git
cd gago-ui
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 5
sudo make install
```

After these steps completed successfully you can run GagoUI by simply running

```bash
Gago
``` 

from your terminal.

## Calibration

Gago's calibration is just a Qt wrapper around OpenCV's calibration framework. Hence, for calibration you will need a printed flat pattern.
Currently Gago supports only the Checkerboard patterns, but I will add (asymmetric) circles later. For calibrating a single camera or a camera rig first print a checkerboard
pattern on a paper and put it on a rigid flat board. 

Open Edit > Settings > Calibration. In the Pattern column input the number of horizontal and verical corners. 

If you are new to calibration and/or not familiar ro OpenCV's calibration framework just leave the other parameters to their default values.

Gago has two regimes for collecting the calibration images  - manual and looping. 
If the Loop capture checkbox on the settings tab is checked, then pressing the 
"Start capture" button will make a shot every "Wait Interval" seconds. Otherwise you will have to manually press "Capture" on the calibration window every time.


For calibrating go to File > Calibration > Calibrate. This will open the calibration dialog. If h
If the patterns was successfully extracted from all cameras the batch will be appended to the set of calibration images.


