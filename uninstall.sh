#!/bin/bash

EXECUTABLE="hermes"
SCRIPT_FILE="hermes.sh"
INSTALL_PATH="/usr/local/bin"
SCRIPT_INSTALL_PATH="$INSTALL_PATH/$SCRIPT_FILE"

if [[ $(id -u) -ne 0 ]]; then
    echo "This script requires root privileges to uninstall."
    echo "Please run: sudo bash uninstall.sh"
    exit 1
fi

if [ -f "$INSTALL_PATH/$EXECUTABLE" ]; then
    rm -f "$INSTALL_PATH/$EXECUTABLE"
    echo "Removed compiled executable from $INSTALL_PATH."
else
    echo "Executable $EXECUTABLE not found in $INSTALL_PATH. Skipping..."
fi

if [ -f "$SCRIPT_INSTALL_PATH" ]; then
    rm -f "$SCRIPT_INSTALL_PATH"
    echo "Removed $SCRIPT_FILE from $INSTALL_PATH."
else
    echo "Script $SCRIPT_FILE not found in $INSTALL_PATH. Skipping..."
fi

if grep -q "hermes()" ~/.bashrc; then
    sed -i '/hermes()/,+1 d' ~/.bashrc
    echo "hermes function removed from ~/.bashrc."
else
    echo "hermes function not found in ~/.bashrc. Skipping..."
fi

echo "To apply the uninstallation changes, please run the following command:"
echo "source ~/.bashrc"

exit 0
