#! /bin/bash
#
rev_new=$(git rev-parse --short=7 HEAD)
version=$(cat ./src/version.h 2>/dev/null | cut -d '"' -f2)


rev_date=`date -u +%Y%m%d%H%M%S`

    cat <<EOF > ./meta/meta.xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>HID to VPAD</name>
  <coder>Maschell</coder>
  <category>tool</category>
  <url>https://gbatemp.net/threads/hid-to-vpad.424127/</url>
  <version>$version-nightly-$rev_new</version>
  <release_date>$rev_date</release_date>
  <short_description>USB HID to gamepad input</short_description>
  <long_description>Emulate input using various USB HID devices.
  </long_description>
</app>
EOF

    cat <<EOF > ./src/version.h
#define APP_VERION "$version-nightly-$rev_new"
EOF


echo $version-nightly-$rev_new
