#! /bin/bash
#
find . -iname "*.cpp" | xargs xgettext --no-wrap -o languages/english.lang -j --no-location --omit-header
find . -iname "*.cpp" | xargs xgettext --no-wrap -o languages/english.lang -j --omit-header --sort-by-file
for fn in `find languages/*.lang`; do
    if [ "$fn" != "languages/english.lang" ]; then
        msgmerge --output-file=$fn $fn languages/english.lang  --no-wrap
    fi
    
done