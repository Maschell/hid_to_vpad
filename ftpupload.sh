#! /bin/bash
#

cd $TRAVIS_BUILD_DIR/wiiu/apps/hidtovpad

for entry in "$TRAVIS_BUILD_DIR/wiiu/apps/hidtovpad"/*; do
    if [ "$entry" != "languages" ] then
        echo "Uploading ${entry##*/}"
        curl --ftp-create-dirs -T "${entry##*/}" -u $FTP_USER:$FTP_PASSWORD "$FTP_SERVER/${entry##*/}"  > /dev/null 2>&1
    fi
done


IFS=$'\n'       # make newlines the only separator

# controller configs
for f in `curl -u $FTP_USER:$FTP_PASSWORD $FTP_SERVER/sd/wiiu/controller/  --silent --use-ascii --list-only`; do
    if [ "$f" != "." ] && [ "$f" != ".." ]; then
        echo "Deleting sd/wiiu/controller/${f##*/}"
        curl -v -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/ -Q "DELE sd/wiiu/controller/$f" --silent > /dev/null 2>&1
    fi
done

cd $TRAVIS_BUILD_DIR/wiiu/controller

for entry in "$TRAVIS_BUILD_DIR/wiiu/controller"/*
do
    echo "Uploading ${entry##*/}"
    curl --ftp-create-dirs -T "${entry##*/}" -u $FTP_USER:$FTP_PASSWORD "$FTP_SERVER/sd/wiiu/controller/${entry##*/}"  > /dev/null 2>&1
done


# Language files
for f in `curl -u $FTP_USER:$FTP_PASSWORD $FTP_SERVER/sd/wiiu/apps/hidtovpad/languages/  --silent --use-ascii --list-only`; do
    if [ "$f" != "." ] && [ "$f" != ".." ]; then
        echo "Deleting sd/wiiu/apps/hidtovpad/languages/${f##*/}"
        curl -v -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/ -Q "DELE sd/wiiu/apps/hidtovpad/languages/$f" --silent > /dev/null 2>&1
    fi
done

cd $TRAVIS_BUILD_DIR/languages

for entry in "$TRAVIS_BUILD_DIR/languages"/*
do
    echo "Uploading ${entry##*/}"
    curl --ftp-create-dirs -T "${entry##*/}" -u $FTP_USER:$FTP_PASSWORD "$FTP_SERVER/sd/wiiu/apps/hidtovpad/languages/${entry##*/}"  > /dev/null 2>&1
done


curl -v -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/ -Q "DELE .deletetoupdate" --silent > /dev/null 2>&1

curl $REFRESH
