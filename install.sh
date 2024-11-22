#!/bin/bash

SOURCE_FILE="hermes.c"
EXECUTABLE="hermes"
SCRIPT_FILE="hermes.sh"
INSTALL_PATH="/usr/local/bin"
SCRIPT_INSTALL_PATH="$INSTALL_PATH/$SCRIPT_FILE"

if [[ $(id -u) -ne 0 ]]; then
    echo "This script requires root privileges to install."
    echo "Please run: sudo bash install.sh"
    exit 1
fi

echo "Compiling $SOURCE_FILE..."
gcc -o "$EXECUTABLE" "$SOURCE_FILE"
if [[ $? -ne 0 ]]; then
    echo "Compilation failed."
    exit 1
fi
echo "$SOURCE_FILE Compiled."

mv "$EXECUTABLE" "$INSTALL_PATH/"
echo "Moved compiled executable to $INSTALL_PATH."
cp "$SCRIPT_FILE" "$SCRIPT_INSTALL_PATH"
echo "Copied $SCRIPT_FILE to $SCRIPT_INSTALL_PATH."
chmod +x "$SCRIPT_INSTALL_PATH"
echo "Made $SCRIPT_INSTALL_PATH executable."

if ! grep -q "hermes()" ~/.bashrc; then
    echo "Adding Hermes Navigator function to ~/.bashrc..."
    cat <<EOF >>~/.bashrc
# Hermes Navigator
hermes() { source $SCRIPT_INSTALL_PATH; }
EOF

else
    echo "Function for hermes already exists in ~/.bashrc. Skipping..."
fi

echo "To apply the installation changes, run the following command:"
echo "source ~/.bashrc"

exit 0