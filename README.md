# 42-nm-otool
Custom C implementation of **nm** and **otool** commands.
- `ft_nm <args>` displays the name list (symbol table of nlist structures) of the `<args>` files
- `ft_tool` displays the content of the (\__TEXT,__text) segment

This project will makes you understand how Mach-O files are structured.

## How to use

### Build
``` bash
make

# Clean commands
make clean
make fclean
make re
```

### Usage

``` bash
# Files supported: exec, .a, .so, .dylib, .o.
./ft_nm file
./ft_otool file
```

## Implementation
- Support for archives
- Support for fat binaries
- Support for debug symbols (`gcc -g <args>` files).
- Many flags (see ./ft_nm -h or ./ft_otool -h)
- Checks for corruption

To understand how I implemented this program, you can refer to the medium article of this project.

### File structure
``` bash
/inc
    # Self made printf
    # Self made libc
    nm_otool.h
/src
    /cmd # Parse argv and set the program state
    /hande # Handle structure for magic files
    /parse # Parse the inner structures
    /shared

    # Root files
    ft_nm.c
    ft_otool.c
```

### Predefined values
- [mach-o/loader.h](https://opensource.apple.com/source/xnu/xnu-792/EXTERNAL_HEADERS/mach-o/loader.h)
- [mach-o/ar.h](https://opensource.apple.com/source/xnu/xnu-1228/EXTERNAL_HEADERS/ar.h.auto.html)
- [mach-o/n-list.h](https://opensource.apple.com/source/xnu/xnu-201/EXTERNAL_HEADERS/mach-o/nlist.h.auto.html)
- [mach-o/fat.h](https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h)
- [mach-o/machine.h](https://opensource.apple.com/source/xnu/xnu-4570.41.2/osfmk/mach/machine.h.auto.html)
