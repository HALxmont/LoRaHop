#!/bin/sh
device=/dev/ttyS4

echo -ne '\xC0\x00\x00\x1A\0x0f\xc4' > $device


