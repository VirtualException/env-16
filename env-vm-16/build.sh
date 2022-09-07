set -xe

gcc -O2 -g -pthread -lSDL2 -D_REENTRANT -o bin/env16vm env16.c inst.c dpy.c
sudo cp ./bin/env16vm /usr/local/bin/