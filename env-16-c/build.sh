set -xe

gcc -O2 -s -o bin/env16c env16c.c
sudo cp ./bin/env16c /usr/local/bin/env16c