#!/bin/bash

new_dir=$(./hermes | tail -n 1)
if [ -d "$new_dir" ]; then
    echo "Hermes changed directory to: $new_dir"
    cd "$new_dir"
fi