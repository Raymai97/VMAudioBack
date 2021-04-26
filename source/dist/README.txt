VMAudioBack 1.2 // 2021-04-27
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

To make it sticks, the program has to run on startup.
One way to do it is by making it a Windows service.
I added support to run as Windows service in Version 1.1.

Remember, if you face any issue, please enable logging and try again.
You may enable logging by deleting "DISABLE_LOGGING" or rename it.
It will generate a log file, useful for troubleshooting.

Version 1.2 added a new method "SET_TIMER_RES".
By default, old method (play MP3) used in previous version is used.
If it's not working for you, or you prefer not to play MP3, you may
enable this new method by renaming "SET_TIMER_RES_INSTEADD" to
"SET_TIMER_RES_INSTEAD".
