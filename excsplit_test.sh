#!/bin/bash

EXCSPLIT=./excsplit

function gen_prefix_seq {
    local p=$1
    shift
    for s in "$@"; do
        echo $p$s
    done
}

for k in 1 2 3 4 10 17; do
    for fn in "$@"; do
        rm -f /tmp/test-excsplit-*
        echo $EXCSPLIT $k $fn $(gen_prefix_seq /tmp/test-excsplit-split $(seq `echo $k+2|bc`))
        $EXCSPLIT $k $fn $(gen_prefix_seq /tmp/test-excsplit-split $(seq `echo $k+2|bc`)) || exit 1
        echo $EXCSPLIT -$k /tmp/test-excsplit-whole $(ls /tmp/test-excsplit-split* |shuf) || exit 2
        $EXCSPLIT -$k /tmp/test-excsplit-whole $(ls /tmp/test-excsplit-split* |shuf) || exit 2
        if diff /tmp/test-excsplit-whole $fn; then
            echo "correct for file $fn $k"
        else
            echo "error for file $fn $k" >&2
            exit 3
        fi
    done
done
