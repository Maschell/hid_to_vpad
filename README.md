[Current Stable Release](https://github.com/Maschell/hid_to_vpad/releases/latest) | [Nightly builds](https://github.com/Maschell/hid_to_vpad/releases) | [Issue Tracker](https://github.com/Maschell/hid_to_vpad/issues) | [Support Thread](https://gbatemp.net/threads/hid-to-vpad.424127/)
# HID to VPAD  [![Build Status](https://api.travis-ci.org/Maschell/hid_to_vpad.svg?branch=master)](https://travis-ci.org/Maschell/hid_to_vpad)


This is a little tool that let you use a hid device on the WiiU. <br />
It's based on the [controller_patcher](https://github.com/Maschell/controller_patcher) engine. 

## Usage
Start the .elf with the homebrew launcher. A GUI will open where you can assign your controller. You can attach and detach devices at any time. To change the mapping, open the Mii Maker.

Place the config files into the following folder:
```
sd:/wiiu/controller
```

## Supported devices
The official GC Adapter, PS3/PS4 Pad, Mouse, Keyboard have built in support. Other devices can be added when a valid config file in provided.  
Check out the [controller_patcher](https://github.com/Maschell/controller_patcher) repository for more details.  

You can find deep information for creating own config files, the default mapping, in the [controller patcher wiki](https://github.com/Maschell/controller_patcher/wiki)

Default button mapping:  
- [Mouse](https://github.com/Maschell/controller_patcher/wiki/3.-Mouses#default-configuration)
- [Keyboard](https://github.com/Maschell/controller_patcher/wiki/4.-Keyboards#default-configuration)
- [GameCube](https://github.com/Maschell/controller_patcher/wiki/5.a-Controller-%7C-Configurate-the-GameCube-controller#default-button-mapping)
- [Dualshock 3](https://github.com/Maschell/controller_patcher/wiki/5.b-Controller-%7C-Configurate-the-Dualshock-3-controller#default-button-mapping)
- [Dualshock 4](https://github.com/Maschell/controller_patcher/wiki/5.c-Controller-%7C-Configurate-the-Dualshock-4-controller#default-button-mapping)

In combination with the Network Client, more controllers are supported. You can find more information [here](http://gbatemp.net/threads/hid-to-vpad-network-client.466150/).

# FAQ

### What about XBOX controller
The Xbox controller are no HID devices. But it can be used in combination with the [Network Client](http://gbatemp.net/threads/hid-to-vpad-network-client.466150/)!

### Is my controller supported?
Take a  look at this [repo](https://github.com/Maschell/controller_patcher_configs)

### ???
Do you have an other question? First take a look at:
- the [controller_patcher repo](https://github.com/Maschell/controller_patcher)
- the thread on [gbatemp](http://gbatemp.net/threads/hid-to-vpad.424127/)
- the [wiki](https://github.com/Maschell/controller_patcher/wiki)  

If you don't find an anwser, please open an issue.

# Building
In order to build this application you need serval libs:

- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.
- [libutils](https://github.com/Maschell/libutils) for common functions.
- [controller_patcher](https://github.com/Maschell/controller_patcher) to emulate the controllers.
- [libgui](https://github.com/Maschell/libgui) for the gui elements.

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

# Credits
- A big thanks goes out to <b>dimok</b> for creating the HBL, the dynamic libs and every stuff he made. The "environment" of this app is copied from ddd, turned out to be a "hello world" with useful extra stuff.  
- Also huge thanks to <b>FIX94</b> who initally created his gc-to-vpad. Helped me a lot! Thanks!  
- And of course big thanks to everyone who has helped me testing! (dimok, dibas, EclipseSin,FunThomas,n1ghty etc.)  