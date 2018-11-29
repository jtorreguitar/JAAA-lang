# JAAA-lang

## 1 - Install dependencies

### Install lex
```
sudo apt-get install flex
```

### Install yacc
```
sudo apt-get install bison
```
## 2 - Set up compiler

### Compile compiler
```
make
```

## 3 - Write your .jaaa program and compile it

### Compile .jaaa file to a .c file
```
./jaaatoc /path/to/myFile.jaaa
```
Output: myFile.c

### Compile .jaaa file to an executable file
```
./jaaac /path/to/myFile.jaaa
```
Output: myFile.x
