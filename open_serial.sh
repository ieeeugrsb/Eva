#!/bin/bash
echo "To close the communication press Ctrl+A AND Ctrl+X"
echo

ano serial -p /dev/ttyACM0 -- --omap crlf,delbs --imap inglf,bsdel --emap crcrlf --echo
