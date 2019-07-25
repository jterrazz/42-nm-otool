/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 12:37:01 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# define FAILURE -1
# define SUCCESS 0
# define DEFAULT_FILE "a.out"

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>

# include "libft.h"
# include "ft_printf.h"

typedef uint8_t				t_bool;

typedef enum	e_bin {
	BIN_NM = 0,
	BIN_OTOOL = 1
}				t_bin;

typedef enum	e_flag {
	FLAG_N = 1 << 0,
	FLAG_R = 1 << 2,
	FLAG_G = 1 << 3,
	FLAG_P = 1 << 4,
	FLAG_U = 1 << 5,
	FLAG_UU = 1 << 6,
	FLAG_J = 1 << 7,
	FLAG_A = 1 << 8,
	FLAG_D = 1 << 9,
	FLAG_M = 1 << 10,
	FLAG_F = 1 << 11,
	FLAG_T = 1 << 12,
	FLAG_HELP = 1 << 13
}				t_flag;

typedef enum	e_file_type {
	FILE_MACHO = 1 << 0,
	FILE_ARCHIVE = 1 << 1,
	FILE_FAT = 1 << 2
}				t_file_type;

typedef enum	e_arch {
	ARCH_32,
	ARCH_64
}				t_arch;

typedef enum	e_file_error {
	E_NULL,
	E_OVERFLOW,
	E_WAS_PRINTED
}				t_file_error;

/*
** Keep the state of the exectution
*/

typedef struct	s_env {
	char const	*execname;
	int			nfiles;
	char const	**filenames;
	cpu_type_t	cputype;
	t_bin		bin;
	uint64_t	flags;
}				t_env;

typedef struct	s_flag_info {
	char		shortname;
	char		fullname[20];
	uint64_t	value;
}				t_flag_info;

typedef struct	s_arch_flag {
	char			*name;
	cpu_type_t		cputype;
	cpu_subtype_t	cpusubtype;
}				t_arch_flag;

/*
** Keep informations about a file, can be a virtual file
** (for nested macho files in fat / archives)
*/

typedef struct	s_file {
	void			*start;
	void			*end;
	t_file_error	error;
	char const		*filename;
	char const		*virtualname;
	uint64_t		filesize;
	t_bool			swap_bits;
	t_file_type		virtualtype;
	t_arch			arch;
	uint64_t		nsects;
	t_list			*mysects;
	t_list			*mysyms;
}				t_file;

/*
** This structures parse the header of files types supported by nm and otool
*/

typedef struct fat_header			t_fat_header;
typedef struct fat_arch				t_fat_arch;
typedef struct ar_hdr				t_ar_hdr;
typedef struct mach_header			t_mach_header;
typedef struct mach_header_64		t_mach_header_64;

/*
** A macho file contains many type of load commands.
** For this project, we will only need LC_SYMTAB and LC_SEGMENT.
*/

typedef struct load_command			t_load_command;

/*
** A LC_SEGMENT contains a list of sections defined below.
*/

typedef struct segment_command		t_segment_command;
typedef struct segment_command_64	t_segment_command_64;
typedef struct section				t_section;
typedef struct section_64			t_section_64;

/*
** A LC_SYMTAB contains a list of symbols defined below.
*/

typedef struct symtab_command		t_symtab_command;
typedef struct nlist				t_nlist;
typedef struct nlist_64				t_nlist_64;

/*
** List of debug values used by symbols.
*/

typedef struct	s_debug_symbol {
	char		*symbol;
	uint16_t	typevalue;
}				t_debug_symbol;

/*
** Custom symbol and section structures,
** Used mainly when priting the data.
*/

typedef struct	s_mysymbol {
	char		*name;
	t_bool		namefailed;
	uint8_t		type;
	char		type_p;
	uint8_t		sect;
	uint16_t	desc;
	uint64_t	value;
	char		*debug_symbol;
}				t_mysymbol;

typedef struct	s_mysection {
	char		*name;
	uint64_t	index;
	uint64_t	addr;
	uint64_t	size;
	uint32_t	offset;
}				t_mysection;

/*
** Command methods
*/

int				cmd_init(t_env *env, int argc, char const *argv[], t_bin bin);
int				cmd_start(t_env *env, char const *filename);
void			cmd_end(t_env *env);

/*
** Handle methods, used for parsing files structures
*/

int				handle_binary(t_env *env, t_file *file);
int				handle_archive(t_env *env, t_file *file);
int				handle_fat(t_env *env, t_file *file);
int				print_fat_header(t_file *file, t_fat_header *fat_header,
	unsigned long nfat_arch, t_fat_arch *fat_arch);

/*
** Mach-o methods
*/

int				parse_macho(t_env *env, t_file *file);
int				parse_macho_segment(t_env *env,
	t_file *file, void *segment_command);
int				parse_macho_symtab(t_file *file,
	t_symtab_command *symtab_command);

/*
** Shared methods
*/

void			create_file(t_file *file,
	char const *name, uint64_t size, void *start);
int				create_virtual_file(t_file *file,
	t_file *old_file, char const *virtualname, t_file_type type);
void			destroy_file(t_file *file);
t_bool			check_overflow(t_file *file, void *ptr);
char			*get_cpu_string(cpu_type_t cputype,
	cpu_subtype_t cpusubtype);
t_mysymbol		*init_mysym(t_file *file, t_mysymbol *mysym,
	char *symname, void *sym);
void			fill_mysym(t_file *file, t_mysymbol *mysym);
char			*ft_strdup_overflow(t_file *file, char *s1,
	char c, int *failed);

int32_t			ft_bswap_int32(int32_t x);
uint32_t		ft_bswap_uint32(uint32_t x);
uint64_t		ft_bswap_uint64(uint64_t x);
uint32_t		swapif_u32(t_file *file, uint32_t x);
uint64_t		swapif_u64(t_file *file, uint64_t x);
size_t			ft_pow(size_t a, size_t b);

void			ft_hexdump(void *start, int64_t size,
	int64_t printed_start, t_file *file);
void			print_mysyms(t_env *env, t_file *file);

int				sort_mysyms_num(t_list *lst1, t_list *lst2);
int				sort_mysyms_alpha(t_list *lst1, t_list *lst2);
void			ft_lstsort(t_list *lst, int (*f)(t_list *lst1, t_list *lst2));
void			ft_lstreverse(t_list *lst);

#endif
