#!/bin/bash
# I am using a temp file instead of piping the output because the program uses tput calls.

/usr/local/bin/hermes
if [ -f /tmp/hermes_output ]; then
    new_dir=$(cat /tmp/hermes_output)
    echo "Hermes Changed Directory to: $new_dir"
    rm -f /tmp/hermes_output
    cd "$new_dir"
else
    echo "Error: Output file not found."
fi