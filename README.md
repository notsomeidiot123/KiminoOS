# KiminoOS
君のOS, or Your Operating system

## [FEATURES]

developed by Notsomeidiot123/Kiminosei <-- same person

Only has 1 language setting <- :) ✅

Sound 🚫

Terminal, based on bash ✅ 🚫

Custom scripting languge 🚫

Read/Write to hard drive <- currently working on

PNP 🚫

USB Support 🚫 (In queue)

Legacy Support ✅ (compiled for i386, max RAM supported is 4 gb)

documentation will be in the wiki

## [PLANS]

Full Keyboard support

Internet Access

POSIX

Compiler for C/C++ (May not add)

Permission Levels & Userspace


## [LONG TERM]

GUI & Windows

Graphics Card support

UEFI Support

AMD64 & Modern internal ports (SATA, M.2, PCI(e))

SD Card Support

Bluetooth & Aux support for headphones

\> 4GB Memory & Paging

TSS Segment

## [BOTTOM HAD MOST RECENT UPDATES]

### 2/4/22 - Progress

It took a while, but I finally made some progress


### 5/4/22 - Progress

I now have a almost fully functioning keyboard driver

And i'm setting up Disk drive reading, but for some reason on boot, the drive will not stop polling

I Know this im using qemu but i really dont understand why it's constantly polling

maybe it's a bug in my code, maybe it's qemu. who knows? not me.

thank god for the OSDEV discord & wikipedia

i'll try running my code on actual hardware, but since i use a ps/2 keyboard driver and a outdated disk drive driver method, it might not work.

### 12/4/22 - Progress

Keyboard is (pretty much) done, just a few keys that are lacking support, and I finally fixed the ATA drive error, and now it works magically. I was writing the IDENTIFY command to the wrong port because i'm dumb and couldn't read the stupid documentation. I dropped support for Floppy drives, but will re-implement them, for no particular reason other than it's cool to support legacy \:sunglasses\: Currently i'm working on the Disk driver, implementing R/W functions and whatnot. Speed is picking up significantly and I think i'll have something semi-usable by the end of the month! I'm trying to balace school & work along with my hobbies (this), and somehow i've managed to spend 3-4 hours a day researching or working on this project. I've been going back and fourth based on what's working well and what's not, so my priorities are a bit messed up.

When i'm finished with this, feel free to fork and make your own changes to it! This is my personal hobby project, but i'd love to see others build something cool with it!

Just please keep the kernel open source, and leave my name in the main kernel file!
