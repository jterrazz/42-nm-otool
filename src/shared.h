/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/11 12:36:03 by jterrazz         ###   ########.fr       */
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
#include <mach-o/nlist.h>
#include <ar.h>
#include "libft.h" // Remove in others ?


// Removes ft_nm.h
typedef enum { BIN_NM, BIN_OTOOL } t_bin;
typedef enum { ARCH_32, ARCH_64 } t_arch;
typedef int t_bool;

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

// Put in cmd.h
typedef struct s_env {
	int argc;
	char const **argv;
	t_bin bin;
}				t_env;

typedef struct s_file {
	void *start;
	char const *filename;
	uint64_t filesize;
	t_bool is_virtual;
	t_arch arch;
	uint64_t nsects;
	t_list *mysects;
	t_list *mysyms;
}				t_file;

int cmd_init_env(t_env *env, int argc, char const *argv[], t_bin bin);
int cmd_process_file(t_env *env, char const *filename);

void handle_file(t_env *env, t_file *file);
void handle_archive(t_env *env, t_file *file);

void parse_mach_file(t_env *env, t_file *file, uint32_t magic);
int	parse_segment(t_env *env, t_file *file, void *segment_command);
int parse_symtab(t_file *file, t_symtab_command *symtab_command);

void init_file(t_file *file, char const *name, uint64_t size, void *start);
void ft_hexdump(void *start, uint64_t size, uint64_t printed_start, t_arch arch);
void print_mysyms(t_file *file);

#endif
