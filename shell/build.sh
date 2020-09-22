# new config
# shell script
# set the path
# set the pwd from the config page you're currently on to script options
# add " & exit" to the end (no quotes)
# so like this:
# C:/path/to/pwd & exit
# interpreter path: D:\devkitPro\msys2\msys2_shell.cmd
# interpreter options: -mingw64
export DEVKITARM=D:/devkitPro/devkitARM
export DEVKITPRO=D:/devkitPro
cd "$1" || exit
make
if [ "$2" == "h" ]; then
  echo Press enter to close.
  read
fi
