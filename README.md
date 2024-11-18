# Hermes
An alternative to 'ls' and 'cd' for Linux.

## Run
```
gcc hermes.c -o hermes
./hermes
```

## Debug
```
gcc -g -o hermes hermes.c
valgrind --leak-check=full --track-origins=yes ./hermes
```