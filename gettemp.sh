#!/bin/bash
LOCKFILE="/tmp/gettemp.lock"

for i in {1..60}; do
    exec 200>"$LOCKFILE"
    if flock -n 200; then
        TEMP=$(sensors | awk '/Package id 0/{print $4}' | tr -d '+°C' | cut -d'.' -f1)
        /path/to/tempwrite/program "$TEMP"
    fi
    [ $i -lt 60 ] && sleep 1
done
