#!/bin/sh
device=/dev/ttyS4

echo -ne '\xC1\xC1\xC1' > $device


