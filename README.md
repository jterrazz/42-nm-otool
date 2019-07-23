# 42-nm-otool

Custom C implementation of **nm** and **otool** commands.
- **ft_nm** displays the name list (symbol table of nlist structures) of each object file in the argument list.
- **ft_tool** display the contents of the (\__TEXT,__text).

## How to use it

### Compile
``` bash
make
```

### Usage

``` bash
./ft_nm file # File can be: exec, .a, .so, .dylib, .o.
./ft_otool file
```

### Clean
``` bash
make clean
make fclean
```

Bonus:
- Debug (explain how to debug)


## Implementation
For a better comprehension of this implementation, please refer to the related medium article.

### Structure
``` bash
/src
    /cmd
    /hande # handle mach-o files and find nested ones
    /parse # parse the segment and/or symtab of a mach-o file
    /print
    /utils
```

### System header ressources
- https://opensource.apple.com/source/xnu/xnu-792/EXTERNAL_HEADERS/mach-o/loader.h
- https://opensource.apple.com/source/xnu/xnu-1228/EXTERNAL_HEADERS/ar.h.auto.html
- https://opensource.apple.com/source/xnu/xnu-201/EXTERNAL_HEADERS/mach-o/nlist.h.auto.html
- https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h
- https://opensource.apple.com/source/xnu/xnu-4570.41.2/osfmk/mach/machine.h.auto.html

## Testing

### Build an executable with the debugger

``` bash
gcc -g <args>
```

How to test files here (automate cmd)

A big thank you to the following repos for their executables:
- https://gitlab.com/louisportay/break-nm
- https://github.com/mmeisson/tests_42
- https://forum.intra.42.fr/topics/1007/messages?page=1#6251

## TODO
Secure the lib functions: ft_memdel ft_strdel ft_lst...
