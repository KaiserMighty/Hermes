# Hermes
An alternative to 'ls' and 'cd' for Linux.  
Inspired by this [Digital Maker Video](https://www.youtube.com/watch?v=jvrDEKDhdB8).  

## Usage
After following the installation instructions below, you can use it anywhere by running this command:
```
hermes
```
This will store the state of your terminal, and show all the directories and files in the current directory. The directories are at the top half of the output, and the files are on the bottom half. Each directory has a key associated with it, which you can press to navigate there. Once you get to where you need to go, simply exit out by hitting `q` and the program will change your current working directory to where you navigated to, and restore your terminal from before.

Here is a mapping of the default controls except the navigation keys:
```
q - Exit the program at the current directory.
w - Move up one directory to the parent.
[ - If applicable, display the last page of directories.
] - If there are over 20 directories, display the next page.
; - If applicable, display the last page of files.
' - If there are over 20 files, display the next page.
```

## Installation
Run these commands to install Hermes:
```
./install.sh
source ~/.bashrc
```

## Uninstallation
Run these commands to uninstall Hermes:
```
./uninstall.sh
source ~/.bashrc
```

## Configuration
If you wish to change the spacing between contents or even the controls of the program, you can edit the defines at the top of `hermes.c`.  
Running the installation commands after any edits will automatically compile and place the edited program in the correct locations such that you can start using it right away.

## Demonstration