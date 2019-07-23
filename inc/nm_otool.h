/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:50:42 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
#define NM_OTOOL_H

// Add void * for all of check_overflow
// TODO Check for endian everywhere + for 64/32
// TODO Check for all offsets
// Free all virtual files
// Makefile messages
// Test to make a malloc fails (by limiting memory ?)
// Printf rename t_flag and update in global libs
// Where create_file = free_file
 // Check to create a file with 1 char size
 // Compare errors results
 // Simulate a malloc error in mach_segment
 /// Leaks
 // Make tests with -g -n -r
 // Test flag -u WITH -U
 // Is a directory. error
 // Explain the letters meaning
 // Reset test from git + add 3 others

#define FAILURE -1
#define SUCCESS 0
#define DEFAULT_FILE "a.out"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <ar.h>

#include "libft.h"
#include "ft_printf.h"

typedef enum e_bin {
	BIN_NM = 1<<0,
	BIN_OTOOL = 1<<1
} t_bin;

typedef enum e_flag {
	FLAG_N = 1<<0,
	FLAG_R = 1<<2,
	FLAG_G = 1<<3,
	FLAG_P = 1<<4,
	FLAG_U = 1<<5,
	FLAG_UU = 1<<6,
	FLAG_J = 1<<7,
	FLAG_A = 1<<8,
	FLAG_HELP = 1<<9 // Check it indeed does a long value
	// Add flag for otool -t (also default) -...
} t_flag;

typedef enum {
	ARCH_32,
	ARCH_64
} t_arch;

typedef enum {
	E_NULL,
	E_OVERFLOW
} t_file_error; // why

typedef uint8_t t_bool;

/*
** Keep the state of the exectution
*/

typedef struct s_env {
	char const *execname;
	int nfiles;
	char const **filenames;
	cpu_type_t cputype;
	t_bin bin;
	uint64_t flags;
}				t_env;

typedef struct s_flag_info {
	char shortname;
	char fullname[20];
	uint64_t value;
	uint32_t binaries;
} t_flag_info;

/*
** Keep informations about a file, can be a virtual file
** (for nested macho files in fat / archives)
*/

typedef struct s_file {
	void *start;
	void *end;
	t_file_error error;
	char const *filename;
	char const *virtualname;
	uint64_t filesize;
	t_bool swap_bits;
	t_bool is_virtual;
	t_arch arch;
	uint64_t nsects;
	t_list *mysects;
	t_list *mysyms;
}				t_file;

/*
** This structures parse the header of files types supported by nm and otool
*/

typedef struct fat_header t_fat_header;
typedef struct fat_arch t_fat_arch;
typedef struct ar_hdr t_ar_hdr;
typedef struct mach_header t_mach_header;
typedef struct mach_header_64 t_mach_header_64;

/*
** A macho file contains many type of load commands.
** For this project, we will only need LC_SYMTAB and LC_SEGMENT.
*/

typedef struct load_command t_load_command;

/*
** A LC_SEGMENT contains a list of sections defined below.
*/

typedef struct segment_command t_segment_command;
typedef struct segment_command_64 t_segment_command_64;
typedef struct section t_section;
typedef struct section_64 t_section_64;

/*
** A LC_SYMTAB contains a list of symbols defined below.
*/

typedef struct symtab_command t_symtab_command;
typedef struct nlist t_nlist;
typedef struct nlist_64 t_nlist_64;

/*
** List of debug values used by symbols.
*/

#define DEBUG_SYMBOLS_LENGTH 29

typedef struct s_debug_symbol {
	char *symbol;
	uint16_t typevalue;
} t_debug_symbol;

extern t_debug_symbol g_debug_symbols[DEBUG_SYMBOLS_LENGTH];

/*
** Custom symbol and section structures,
** Used mainly when priting the data.
*/

typedef struct s_mysymbol { // Maybe delete some (probably :) in simtab file particulary )
	char *name;
	t_bool namefailed; // for what ?
	uint8_t type;
	char type_p;
	uint8_t sect;
	uint16_t desc;
	uint64_t value;
	char *debug_symbol;
}				t_mysymbol;

typedef struct s_mysection {
	char *name;
	uint64_t index;
}				t_mysection;

/*
** Command methods
*/

int cmd_init(t_env *env, int argc, char const *argv[], t_bin bin);
int cmd_start(t_env *env, char const *filename);
void cmd_end(t_env *env);

/*
** Handle methods, used for parsing files structures
*/

int handle_binary(t_env *env, t_file *file);
int handle_archive(t_env *env, t_file *file);
int handle_fat(t_env *env, t_file *file);

/*
** Mach-o methods
*/

int parse_macho(t_env *env, t_file *file);
int	parse_machoo_segment(t_env *env, t_file *file, void *segment_command);
int parse_macho_symtab(t_file *file, t_symtab_command *symtab_command);

/*
** Shared methods
*/

void create_file(t_file *file, char const *name, uint64_t size, void *start);
void create_virtual_file(t_file *file, char *virtualname);
void destroy_file(t_file *file);
t_bool check_overflow(t_file *file, void *ptr);
char *get_cpu_string(cpu_type_t cputype);
t_mysymbol *init_mysym(t_file *file, t_mysymbol *mysym, char *symname, void *sym);
void fill_mysym(t_file *file, t_mysymbol *mysym);
char	*ft_strdup_overflow(t_file *file, char *s1, char c, int *failed);

int32_t ft_bswap_int32(int32_t x);
uint32_t ft_bswap_uint32(uint32_t x);
uint64_t ft_bswap_uint64(uint64_t x);
uint32_t swapif_u32(t_file *file, uint32_t x);
uint64_t swapif_u64(t_file *file, uint64_t x);

void ft_hexdump(void *start, uint64_t size, uint64_t printed_start, t_arch arch);
void print_mysyms(t_env *env, t_file *file);

#endif
