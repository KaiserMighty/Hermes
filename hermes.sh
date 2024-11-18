#!/bin/bash

hermes
new_dir=$(cat /tmp/hermes_output.txt)
rm /tmp/hermes_output.txt
if [ -d "$new_dir" ]; then
    cd "$new_dir"
    echo "Changed to directory is: $new_dir"
else
    echo "Invalid directory: $new_dir"
fi