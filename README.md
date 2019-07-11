# 42-nm-otool

Custom C implementation of **nm** and **otool** commands.

- **ft_nm** displays the name list (symbol table of nlist structures) of each object file in the argument list.

- **ft_tool** display the contents of the (\__TEXT,__text).

## How to use it

### Compilation
``` bash
make

make clean
make fclean
```

### Usage

``` bash
./ft_nm file
./ft_otool file

# File can be: exec, .a, .so, .dylib, .o.
# All files from /bin /sbin /usr/lib /usr/bin /usr/sbin
```

## Implementation
To better understant this project, you can read the medium article made for it here.

### Structure
```
/src
  /cmd: save the arguments in the state
  /hande: decode files until a macho part can be parsed
  /parse: can parse the segment and/or symtab of a mach file
  /print
  /utils
```

### System header files
https://opensource.apple.com/source/xnu/xnu-792/EXTERNAL_HEADERS/mach-o/loader.h
https://opensource.apple.com/source/xnu/xnu-1228/EXTERNAL_HEADERS/ar.h.auto.html
https://opensource.apple.com/source/xnu/xnu-201/EXTERNAL_HEADERS/mach-o/nlist.h.auto.html
https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h
https://opensource.apple.com/source/xnu/xnu-4570.41.2/osfmk/mach/machine.h.auto.html

### Testing
How to test files here (automate cmd)

Thanks to the following repos for their executables:
https://gitlab.com/louisportay/break-nm
https://github.com/mmeisson/tests_42
https://forum.intra.42.fr/topics/1007/messages?page=1#6251

## TODO
Secure the lib functions: ft_memdel ft_strdel ft_lst...
