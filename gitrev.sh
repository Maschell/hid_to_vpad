#! /bin/bash
#
rev_new=$(git rev-parse --short=7 HEAD)
echo v0.1-nightly-$rev_new
