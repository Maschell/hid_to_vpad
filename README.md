#HID to VPAD#
This is a little tool that let you use a hid device instead of the WiiU Gamepad. <br />
Currently its experimental and result in random crashes at exiting an application or attaching/detaching a device.<br />
It's based on the [controller_patcher](https://github.com/Maschell/controller_patcher) engine. 

##Usage##
Start the .elf with the homebrew launcher. The system menu will be loaded and you can use your device. You can attach and detach devices at any time, but maybe it will result in a random crash.

Place the config files into the following folder:
```
sd:/wiiu/controller
```

##Supported devices##
The official GC Adapter, PS3/PS4 Pad, Mouse, Keyboard have built in support. Other devices can be added when a valid config file in provided.  
Check out the [controller_patcher](https://github.com/Maschell/controller_patcher) repository for more details.  

You can find deep information for creating own config files, the default mapping, in the [controller patcher wiki](https://github.com/Maschell/controller_patcher/wiki)

Default button mapping:  
- [Mouse](https://github.com/Maschell/controller_patcher/wiki/3.-Mouses#default-configuration)
- [Keyboard](https://github.com/Maschell/controller_patcher/wiki/4.-Keyboards#default-configuration)
- [GameCube](https://github.com/Maschell/controller_patcher/wiki/5.a-Controller-%7C-Configurate-the-GameCube-controller#default-button-mapping)
- [Dualshock 3](https://github.com/Maschell/controller_patcher/wiki/5.b-Controller-%7C-Configurate-the-Dualshock-3-controller#default-button-mapping)
- [Dualshock 4](https://github.com/Maschell/controller_patcher/wiki/5.c-Controller-%7C-Configurate-the-Dualshock-4-controller#default-button-mapping)

#FAQ
### I'm getting this annoying screen at the beginning
Just press A to skip it.

### What about XBOX controller
The Xbox controller are no HID devices. Its not possible to use them with this method.

### Is my controller supported?
Take a  look at this [repo](https://github.com/Maschell/controller_patcher_configs)

### Can I really use my Mouse and Keyboard
Yes. Just plug in you mouse and keyboard. The mouse has 2 mode, switch between them with "F1" on the keyboard.

### ???
Do you have an other question? First take a look at:
- the [controller_patcher repo](https://github.com/Maschell/controller_patcher)
- the thread on [gbatemp](http://gbatemp.net/threads/hid-to-vpad.424127/)
- the [wiki](https://github.com/Maschell/controller_patcher/wiki)  

If you don't find an anwser, please open an issue.

#Building#
In order to build this application, you need the custom liboGC and portlibs modified/created by dimok. You can find them on the loadiine_gx2 repo (https://github.com/dimok789/loadiine_gx2/releases/tag/v0.2). Simply put the files in your devkit folder and run the Makefile of HID to VPAD. 

#Credits#
A big thanks goes out to <b>dimok</b> for creating the HBL, the dynamic libs and every stuff he made. The "environment" of this app is copied from ddd, turned out to be a "hello world" with useful extra stuff.
Also huge thanks to <b>FIX94</b> who initally created his gc-to-vpad. Helped me a lot! Thanks!
And of course big thanks to everyone who has helped me testing! (dimok, dibas, EclipseSin,FunThomas,n1ghty etc.)
     

