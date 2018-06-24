#! /bin/bash
#
rev_new=$(git rev-parse --short=7 HEAD)
echo HID_to-VPAD_WUPS-nightly-$rev_new
