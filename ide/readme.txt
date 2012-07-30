The ART IDE is modified from Arduino IDE.

* "Arduino" is a trademark of Arduino team.

Steps for First Time Setup
==========================
1. Install Development Tools
----------------------------
Windows
On Windows, you'll need Cygwin, a Java JDK, and ant.

Cygwin is downloadable from http://www.cygwin.com/ or specifically: http://www.cygwin.com/setup.exe

Of the packages, begin with the defaults, and add:

git - used for version control
make, gcc-mingw, and g++ - used to build arduino.exe (this will also pull in gcc-core)
perl - use this version, activestate or other distros have trouble
unzip, zip - for dealing with archives
Included in the defaults, but make sure:

coreutils (or textutils), gzip, tar

And be sure to leave the option selected for 'unix line endings'

Download and install ant. Add the apache-ant-xxx\bin directory to your path.

Download and install a Java JDK and point the JAVA_HOME environment variable to its root directory. An error message that reads "Unable to locate tools.jar. Expected to find it in C:\Program Files\Java\jre6\lib\tools.jar" means you need to set JAVA_HOME to your JDK (not JRE) installation.

Mac OS X
On Mac OS X, install Apple's Developer Tools and git.

Linux
On Linux, you need the Sun Java SDK, avr-gcc, avr-g++, avr-libc, make, ant, and git.

2. Grab the code from GitHub
----------------------------
this grabs the code as an anonymous user.

# grab the code, it'll take a while
git clone https://github.com/RTGUI/ART.git
# (maybe even a long while for you dialup and international folks)
3. Build It
Use the command line.

cd /path/to/ide/build
ant

# if everything went well, you'll have no errors. (feel free to make
# suggestions for things to include here for common problems)

# then to run it
ant run

# each time you make a change, use ant to build the thing 
# and run to get it up and running. 
Updating to the Latest Version
Each time you want to update to latest version from svn:

cd /path/to/arduino
git pull
git update
If new folders have been added, or you're gettin odd errors, use:

ant clean
