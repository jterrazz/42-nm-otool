/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 16:00:02 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
#define SHARED_H

// TODO Check for endian everywhere + for 64/32
// TODO Check for all offsets
// Makefile messages
// Test to make a malloc fails (by limiting memory ?)
// Printf rename t_flag and update in global libs
// Secure all the lstnew

// Replace My ft_atoi 	int is_neg; ~!!!!!!!!!!!!!!!!! and in global libs !!!!!!!!

#define FAILURE -1
#define SUCCESS 0

#include <stdlib.h> // For uint32_t (check we use it)
#include <mach-o/loader.h>
#include <mach-o/nlist.h> // For segment structure
#include <mach/machine.h> // For CPU type
#include <mach-o/fat.h> // For fat structure
#include <ar.h> // Fir archive structure
#include "libft.h" // Remove in others ?


// Removes ft_nm.h
typedef enum e_bin { BIN_NM = 00000001, BIN_OTOOL = 00000010 } t_bin;
typedef enum e_flag { FLAG_N = 00000001, FLAG_R = 00000010, FLAG_G = 00000100 } t_flag;
typedef enum { ARCH_32, ARCH_64 } t_arch;
typedef enum { E_NULL, E_OVERFLOW } t_file_error;
typedef int t_bool;

typedef struct fat_header t_fat_header;
typedef struct fat_arch t_fat_arch;
typedef struct ar_hdr t_ar_hdr;

typedef struct mach_header t_mach_header;
typedef struct mach_header_64 t_mach_header_64;

/*
** The file has many 'load commands' after its header, the type defines how we will decode the data
*/

typedef struct load_command t_load_command;

/*
** A segment is made up of zero or more sections.
*/

typedef struct segment_command t_segment_command;
typedef struct segment_command_64 t_segment_command_64;
typedef struct section t_section;
typedef struct section_64 t_section_64;

typedef struct symtab_command t_symtab_command;
typedef struct nlist t_nlist;
typedef struct nlist_64 t_nlist_64;

// Rename to mysymbal
typedef struct s_symbol { // Maybe delete some (probably :) in simtab file particulary )
	char *name;
	t_bool namefailed;
	uint8_t type;
	char type_p;
	uint8_t sect;
	uint16_t desc;
	uint64_t value;
}				t_symbol;

typedef struct s_mysection {
	char *name;
	uint64_t index;
}				t_mysection;

typedef struct s_flag_detail {
	char symbol;
	char fullname[20];
	uint32_t value;
	uint32_t binaries;
} t_flag_detail;

// Put in cmd.h
typedef struct s_env {
	int argc;
	cpu_type_t cputype;
	char const **argv;
	t_bin bin;
	uint32_t flags;
}				t_env;

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

int cmd_init(t_env *env, int argc, char const *argv[], t_bin bin);
int cmd_start(t_env *env, char const *filename);

int handle_file(t_env *env, t_file *file);
int handle_archive(t_env *env, t_file *file);
int handle_fat(t_env *env, t_file *file, uint32_t magic);

int parse_mach(t_env *env, t_file *file);
int	parse_mach_segment(t_env *env, t_file *file, void *segment_command);
int parse_mach_symtab(t_file *file, t_symtab_command *symtab_command);

void init_file(t_file *file, char const *name, uint64_t size, void *start);
void init_virtual_file(t_file *file, t_file *old_file, char *virtualname);
void ft_hexdump(void *start, uint64_t size, uint64_t printed_start, t_arch arch);
void print_mysyms(t_env *env, t_file *file);

int32_t ft_bswap_int32(int32_t x);
uint32_t ft_bswap_uint32(uint32_t x);
uint64_t ft_bswap_uint64(uint64_t x);
uint32_t swapif_u32(t_file *file, uint32_t x);
uint64_t swapif_u64(t_file *file, uint64_t x);

t_bool check_over(t_file *file, void *ptr);
char *get_cpu_string(cpu_type_t cputype);

#endif
