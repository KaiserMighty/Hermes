#!/bin/bash

new_dir=$(./hermes | tail -n 1)
if [ -d "$new_dir" ]; then
    cd "$new_dir" || exit
    echo "Changed to directory: $new_dir"
else
    echo "Invalid directory: $new_dir"
fi
