#! /bin/bash
#
find . -iname "*.cpp" | xargs xgettext -o languages/english.lang -j
for fn in `find languages/*.lang`; do
    if [ "$fn" != "languages/english.lang" ]; then
        msgmerge --output-file=$fn $fn languages/english.lang
    fi
    
done