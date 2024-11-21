#!/bin/bash

new_dir=$(./hermes 3>&1 1>/dev/tty)
if [ -d "$new_dir" ]; then
    echo "Hermes changed directory to: $new_dir"
    cd "$new_dir"
fi