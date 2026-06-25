# For debuging in glibc (path may vary)
set substitute-path ./csu /usr/src/glibc/glibc-2.35/csu
set substitute-path . /usr/src/glibc/glibc-2.35

# Disable LeakSanitizer in gdb
# (run program directly in terminal for LSan)
set env ASAN_OPTIONS=detect_leaks=0

set logging enabled on
set logging file debug.log
set print pretty on
# set args <arg1> <arg2> ...
b main
r
