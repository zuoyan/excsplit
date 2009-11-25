#!/bin/bash

function gen_prefix_seq {
    local p=$1
    shift
    for s in "$@"; do
        echo $p$s
    done
}

for k in 2 3 4; do
    for fn in "$@"; do
        rm -f /tmp/test-langrange-*
        echo langrange $k $fn $(gen_prefix_seq /tmp/test-langrange-split $(seq `echo $k+2|bc`))
        langrange $k $fn $(gen_prefix_seq /tmp/test-langrange-split $(seq `echo $k+2|bc`)) || exit 1
        echo langrange -$k /tmp/test-langrange-whole $(ls /tmp/test-langrange-split* |shuf) || exit 2
        langrange -$k /tmp/test-langrange-whole $(ls /tmp/test-langrange-split* |shuf) || exit 2
        if diff /tmp/test-langrange-whole $fn; then
            echo "correct for file $fn $k"
        else
            echo "correct for file $fn $k" >&2
            exit 3
        fi
    done
done
