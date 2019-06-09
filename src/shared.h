/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/09 23:28:26 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
#define SHARED_H

// TODO Check for endian everywhere + for 64/32
// TODO Check for all offsets

#define FAILURE -1
#define SUCCESS 0

#include <stdlib.h> // For uint32_t (check we use it)
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

typedef enum { BIN_NM, BIN_OTOOL } t_bin;
typedef enum { ARCH_32, ARCH_64 } t_arch;

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

typedef struct symtab_command t_symtab_command;
typedef struct nlist t_nlist;
typedef struct nlist_64 t_nlist_64;

// TODO Check all properties are used

typedef struct s_env {
	int argc;
	char const **argv;
	t_bin bin;
}				t_env;

typedef struct s_file {
	char const *filename;
	size_t filesize;
	char *start;
	t_arch arch;
}				t_file;

int cmd_init_env(t_env *env, int argc, char const *argv[], t_bin bin);
int cmd_process_file(t_env *env, char const *filename);

void parse_file(t_env *env, t_file *file);
int	parse_segment(void *segment_command, t_arch arch);

#endif
