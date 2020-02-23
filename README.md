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

Open Edit > Settings > Calibration. In the Pattern column input the number of horizontal and verical corners. In the left column select a folder where the calibration images will be collected. Make sure your user has permissions to write in that folder.

If you are not familiar to OpenCV's calibration framework just leave the other parameters to their default values.

Gago has two regimes for collecting the calibration images  - manual and looping. 
If the Loop capture checkbox on the settings tab is checked, then pressing the "Start capture" button will make a shot every "Wait Interval" seconds. Otherwise you will have to manually press "Capture" on the calibration window every time.


For calibrating go to File > Calibration > Calibrate. This will open the calibration dialog. Place the pattern so that it is visible to all cameras. If the number of corners was set coorectly, then Gago will color the corners as soon as they get extracted. If at the point of capturing the pattern is correctly visible on all cameras the image batch will be appended to the list of calibration images. If you go with the looping calibration regime, move the pattern slowly. I tried to write the program so that it takes images from both cameras as synchronous as possible, however full synchronization can be achieved only by hardware. As soon as you collected 20 images from different perspectives stop capturing and press calibrate. After a while the images on the right will be colored. The color strip on the left of each image corresponds to the calibration error of that very image. The background color of a row corresponds to the stereo calibration error of that pair. Red means that the error is greater than 1 pixel. Blue means that the error lies in the interval 0.5-1. Green means that the calibration error is lss than 0.5 pixel. If for some reason the calibration pattern was not extracted the image will be marked with gray color. If you are good with the error values press Save & Close. After this the action File > Calibration > Save As > OpenCv Yml will be activated and you can export the estimated intrinsic and extrinsic parameters to a directory.


