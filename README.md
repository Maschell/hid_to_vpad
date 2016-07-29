#HID to VPAD#
This is a little tool that let you use a hid device instead of the WiiU Gamepad. <br />
Currently its very experimental and result in random crashes at exiting an application or attach/detach a device.<br />

##Usage##
Start the .elf with the homebrew launcher. The system menu will be loaded and you can use your device. You can attach and detach devices at any time, but maybe it will result in a random crash.

##Supported devices##
The official GC Adapter, PS3/PS4 Pad, Mouse, Keyboard and a few other devices are currenty supported, check out the controller_patcher repository for the full list(https://github.com/Maschell/controller_patcher), also 99% of the logic is this reposistory.

#### GC-Adapter: ####
You can plug in the controller in any port, if mutiple pads are connected only one pad is used.<br />
Mapping is the same as gc-to-vpad by FIX94. To quoute him:<br />
"It just works like you would expect for the most part except for Z, holding down Z will switch L to gamepad L, R to gamepad R and start to gamepad minus. This is done for convenience purposes."<br />

####Playstation 3/4 pad:####
Works as aspected, button layout is the same as on the gamepad (Circle is A, Cross is B etc.)<br />
Rumble is currently not working.<br />

####Mouse support:####
Currently mice are supported in two different modes. Pressing F1 on a connected keyboard will change the mode<br />
- Touch mode (Touchscreen emulation) (default)<br />
- Aim mode (Right stick emulation) (Left click is ZR, left click R)<br />

####Keyboard mapping:####
LStick = WSAD <br />
DPad = Arrowkeys<br />
A = E<br />
B = Q<br />
X = Space<br />
Y(Z) = R<br />
Plus = Enter<br />
Minus = Minus (Numpad)<br />
L = V<br />
R = B<br />
ZL = Shift<br />
ZR = N<br />

####Building####
In order to build this application, you need the custom liboGC and portlibs modified/created by dimok. You can find them on the loadiine_gx2 repo (https://github.com/dimok789/loadiine_gx2/releases/tag/v0.2). Simply put the files in your devkit folder and run the Makefile of HID to VPAD. 

##Credits##
A big thanks goes out to <b>dimok</b> for creating the HBL, the dynamic libs and every stuff he made. The "environment" of this app is copied from ddd, turned out to be a "hello world" with useful extra stuff.
Also huge thanks to <b>FIX94</b> who initally created his gc-to-vpad. Helped me a lot! Thanks!
And of course big thanks to everyone who has helped me testing! (dimok, dibas, EclipseSin,FunThomas,n1ghty etc.)
     

