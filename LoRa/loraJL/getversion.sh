#!/bin/sh
device=/dev/ttyS4

echo -ne '\xC3\xC3\xC3' > $device


