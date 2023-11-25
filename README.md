# VMAudioBack

Eliminate VMware sound distortion that occurs when:
* VMware is Workstation version 8 or above
* Host OS is Windows 8 or above
* Guest OS is Windows NT 5.x (e.g. Windows XP)

[Example of audio distortion](https://github.com/Raymai97/VMAudioBack/blob/trunk/vmware-win2k-distort.opus)

[Someone also demonstrated this on YouTube](https://youtu.be/4D8BRKvTp7E)

Not working for you? Try [VMAudioBackHost](https://github.com/Raymai97/VMAudioBackHost/).

## Features

* Do not require .NET Framework like VMAudioTrayFix
* Very lightweight as it linked with msvcrt.dll
* Can be installed as Windows service easily
