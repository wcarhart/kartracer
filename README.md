# kartracer
A simple car-driving "game", written in C using OpenGL.

## Summary
This was the final project for my sophomore year graphics class. I didn't really know how to code, but I had a ton of fun writing it. It is rudimentary because the course focused on theory, not practical application. As a result, we only used C instead of C++, and all graphics objects are created from scratch via OpenGL primitives.

For a longer summary, please take a look at `infosheet.txt`.

## Demo
There are three different cars and many different colors to choose from:
![demo of cars](vehiclesdemo.gif)

A full track with speedometer:
![demo of track](trackdemo.gif)

#### Key features
* 3 fully custom vehicles (sports car, F1 car, truck)
* 20 color options
* 4 customizable camera positions (front bumper, 1st person, 3rd person, and fixed central location)
* Real time model updates, with toggleable headlights, wheels that turn, and working brake + reverse lights
* speedometer
* three gears (D, N, R)
* full track with checkpoints
* toggleable minimap of current track position
* lap timer

## Install
If you're on Windows, just use the compiled `KartRacer.exe`. If you'd like to recompile the source code, you'll have to install OpenGL (I've tried to include some of the dependencies here, but you'll have to get the rest from https://www.opengl.org/). Good luck 😉. Next you'll need a compiler. `gcc` works well if you have it, but it's not installed by default on Windows. While developing `kartracer`, I used [lcc](https://github.com/drh/lcc). Refer to `use.bat` for how to compile and link with the OpenGL dll files.

## Full Controls
For a longer summary, please take a look at `infosheet.txt`.

#### Movement
```
W = accelerate
S = brake
A = turn left
D = turn right
E = emergency brake (only works when car is in drive)
```

#### Car Options
````
LEFT-CLICK = change car model
RIGHT-CLICK = open color selection menu
H = turn on headlights
Y = turn off headlights
B = turn on minimap
N = turn off minimap
C = show lap timer
V = hide lap timer
R = restart lap timer
````

#### Camera
```
I = move camera position forward
O = move camera position backward
U = move camera position up
J = move camera position down
1 = nose camera position
2 = body camera position
3 = behind camera position
4 = zoomed-out camera position
9 = car camera view
0 = fixed camera view
```

#### Game options
```
P = pause
Q = quit
```
