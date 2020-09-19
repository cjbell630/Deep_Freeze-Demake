# new config
# shell script
# set the path
# set the pwd from the config page you're corrently on to script options
# add " & exit" to the end (no quotes)
# so like this:
# C:/path/to/pwd & exit
# interperter path: D:\devkitPro\msys2\msys2_shell.cmd
# interpreter options: -mingw64
export DEVKITARM=D:/devkitPro/devkitARM
export DEVKITPRO=D:/devkitPro
cd $1
make