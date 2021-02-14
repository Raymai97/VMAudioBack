VMAudioBack 1.1 // 2020-02-14
Written by raymai97

Eliminate VMware sound distortion that occurs when:
* VMware is Workstation version 8 or above
* Host OS is Windows 8 or above
* Guest OS is Windows NT 5.x (e.g. Windows XP)

--- How to use ---

This program is meant to be run in your guest OS.

Copy the program folder to your guest OS.
I recommend you to copy it to "C:\VMAudioBack".

Now in your guest OS, locate the EXE and double-click to run it.
Nothing will appear but you can see it running in Task Manager.
When it is running, do something to test the sound.

If the sound distortion is gone, congratulations!
If it is still there, sorry I couldn't help you, you may stop reading.

Now, to make it sticks, the program has to run on startup.
One way to do it is by making it a Windows service.
I added support to run as Windows service in Version 1.1.

If you get "Error" installing service, enable logging and try again.
It will generate a log file which would tell us why.
