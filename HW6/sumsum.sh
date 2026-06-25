#!/bin/sh
sum() {
    s=0
    for n in "$@"; do
        s=$(expr "$s" + "$n" 2>/dev/null)
        if [ $? -ne 0 ]; then
            echo 0
            return
        fi
    done
    echo "$s"
}

read line1
read line2

if [ "$(sum $line1)" = "$(sum $line2)" ]; then
    echo "Equal"
else
    echo "Not equal"
fi
