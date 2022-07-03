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

### 2/7/22 - Progress

So, i've been working on this on and off the past three months, and not too much progress has been made. however, I was able to add ctrl + backspace to my keyboard driver, and I'm working on fixing a bug in my ATA PIO driver. Progress is again slow and steady. Meanwhile, I'll continue to work on video modes, and improve the bootloader, so that I may properly load all files & code. The largest issue, however, is that while reading from the hard drive works perfectly, writing to the hard drive is not working at all, for example, I'll try to write "Hello, World", the disk, while preserving currently existing data. However, what I get is the string i wish to write, followed by ((512 * sectorCount) - strlen(data)) null bytes. I will continue to work on this as it is a massive roadblock for me. 

### 2/7/22 - Progress pt 2
 After fiddling around with the driver for a bit, I found out that the issue was i was shifting the 16-bit value i was trying to write by eight bits, then incrementing it twice, so 
 
0x4565, 0x5f32, 0x2322, 0x2400 would be written as 0x4532, 0x2300, plus whatever garbage came after that.

it took me only about two months to patch -_-, but now it's working!!! once i found the issue, it was a quick fix, then an additional patch to write the null-terminator at the end of the string. For now, I'll maintain exclusive support for ATA PIO mode.

and on second thought, i'll start writing my devlogs in the wiki
