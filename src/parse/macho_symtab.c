/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_symtab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:03:36 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 16:28:26 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** The list is defined in <mach-o/stab.h>
** https://opensource.apple.com/source/cctools/cctools-773/include/mach-o/stab.h
*/

t_debug_symbol g_debug_symbols[DEBUG_SYMBOLS_LENGTH] =
{
	{"GSYM", 0x20},
	{"FNAME", 0x22},
	{"FUN", 0x24},
	{"STSYM", 0x26},
	{"LCSYM", 0x28},
	{"BNSYM", 0x2e},
	{"OPT", 0x3c},
	{"RSYM", 0x40},
	{"SLINE", 0x44},
	{"ENSYM", 0x4e},
	{"SSYM", 0x60},
	{"SO", 0x64},
	{"OSO", 0x66},
	{"LSYM", 0x80},
	{"BINCL", 0x82},
	{"SOL", 0x84},
	{"PARAMS" , 0x86},
	{"VERSION", 0x88},
	{"OLEVEL" , 0x8A},
	{"PSYM", 0xa0},
	{"EINCL", 0xa2},
	{"ENTRY", 0xa4},
	{"LBRAC", 0xc0},
	{"EXCL", 0xc2},
	{"RBRAC", 0xe0},
	{"BCOMM", 0xe2},
	{"ECOMM", 0xe4},
	{"ECOML", 0xe8},
	{"LENG", 0xfe}
};

// Redo the function
char	*ft_strdup_safe(t_file *file, char *s1, char c, t_bool inc_c, int *failed)
{
	char	*str;
	size_t		size;
	size_t		i;

	i = 0;
	size = 0;
	while (!check_overflow(file, s1 + size) && s1[size] && s1[size] != c)
		size++;
	if (check_overflow(file, s1 + size)) {
		*failed = 1;
		return ft_strdup("bad string index");
	}
	if ((str = (char *)malloc(sizeof(*str) * (size + 1 + inc_c))) == NULL)
		return (NULL);
	while (s1[i] && s1[i] != c)
	{
		str[i] = s1[i];
		i++;
	}
	if (inc_c && s1[size] == c){
		str[i] = c;
		str[i + 1] = 0;
	} else
		str[i] = 0;
	return (str);
}

// For the debugging values
//

static void init_mysym(t_file *file, t_mysymbol *mysym, char *symname, void *sym) {
	int failed;
	ft_bzero(mysym, sizeof(t_mysymbol));

	failed = 0;
	mysym->type_p = ' '; (void)symname;
	mysym->name = ft_strdup_safe(file, symname, '\n', 1, &failed); // maybe retest with last line /n to remove the special car in ftsafe
	if (failed)
		mysym->namefailed = TRUE;
	if (!mysym->name)
		return; // Set file to error and return (maybe some will return null in not check_overflow)
	// TODO Free it !!!!!!
	if (file->arch == ARCH_32) {
		mysym->type = ((t_nlist *)sym)->n_type; // No swap ???
		mysym->sect = ((t_nlist *)sym)->n_sect; // No swap ???
		mysym->desc = ((t_nlist *)sym)->n_desc; // No swap ???
		mysym->value = swapif_u32(file, ((t_nlist *)sym)->n_value);
	} else {
		mysym->type = ((t_nlist_64 *)sym)->n_type; // No swap ???
		mysym->sect = ((t_nlist_64 *)sym)->n_sect; // No swap ???
		mysym->desc = ((t_nlist_64 *)sym)->n_desc; // No swap ???
		mysym->value = swapif_u64(file, ((t_nlist_64 *)sym)->n_value);
	}
}

static t_mysection *find_mysection(t_list *lst, uint8_t n_sect) {
	while (lst && lst->content) {
		if (((t_mysection *)lst->content)->index == n_sect) {
			return lst->content;
		}
		lst = lst->next;
	}

	return NULL;
}

//https://github.com/opensource-apple/cctools/blob/master/include/mach-o/nlist.h
static void match_sym_section(t_list *mysect_lst, t_mysymbol *mysym) {
	t_mysection *mysect;

	if ((mysect = find_mysection(mysect_lst, mysym->sect))) {
		if (!ft_strcmp(mysect->name, SECT_TEXT))
			mysym->type_p = 'T'; // Never found
		else if (!ft_strcmp(mysect->name, SECT_DATA))
			mysym->type_p = 'D';
		else if (!ft_strcmp(mysect->name, SECT_BSS)) // Not sure in print cause never found ?
			mysym->type_p = 'B';
		else
			mysym->type_p = 'S';

		if (!(mysym->type & N_EXT))
			mysym->type_p -= 'A' - 'a';
	}
}

static void fill_mysym(t_file *file, t_mysymbol *mysym) {
	// #define N_ABS 0x2		/* absolute, n_sect == NO_SECT */
	// #define N_SECT 0xe		/* defined in section number n_sect */
	// #define N_PBUD 0xc		/* prebound undefined (defined in a dylib) */
	// #define N_INDR 0xa
	if (N_STAB & mysym->type) {
		mysym->type_p = '-'; // Remove if no debug
		int i = 0;

		while (i < DEBUG_SYMBOLS_LENGTH) {
			if (g_debug_symbols[i].value == mysym->type) {
				mysym->debug_symbol = g_debug_symbols[i].symbol;
			}
			i++;
		}
	} else if ((N_TYPE & mysym->type) == N_UNDF) {
		if (mysym->namefailed)
			mysym->type_p = 'C';
		else if (mysym->type & N_EXT)
			mysym->type_p = 'U'; // Second U condition !!!
		else
			mysym->type_p = '?';
	} else if ((N_TYPE & mysym->type) == N_SECT) {
		match_sym_section(file->mysects, mysym);
	} else if ((N_TYPE & mysym->type) == N_ABS) {
		mysym->type_p = 'A';
	} else if ((N_TYPE & mysym->type) == N_INDR) {
		mysym->type_p = 'I';
	}
	// 	#define	N_STAB	0xe0  /* if any of these bits set, a symbolic debugging entry */
	// #define	N_PEXT	0x10  /* private external symbol bit */
	// #define	N_TYPE	0x0e  /* mask for the type bits */
	// #define	N_EXT	0x01  /* external symbol bit, set for external symbols */
}

/*
** Parse the symbol table, and set the file->mysym variable
*/

int parse_machoo_symtab(t_file *file, t_symtab_command *symtab_command) { // No need for 64 ?
	void *symtab;
	void *strtab;
	uint64_t nsyms; // 32
	uint64_t i; // 32too
	t_mysymbol mysym;
	char *symname;

	strtab = (void *) file->start + swapif_u32(file, symtab_command->stroff);
	symtab = (void *) file->start + swapif_u32(file, symtab_command->symoff);
	nsyms = swapif_u32(file, symtab_command->nsyms);
	i = 0;

	while (i < nsyms) {
		if (check_overflow(file, strtab) || check_overflow(file, symtab + ((file->arch == ARCH_32) ? sizeof(t_nlist) : sizeof(t_nlist_64))))
			return FAILURE;
		symname = strtab + swapif_u32(file, (file->arch == ARCH_32) ? ((t_nlist *)symtab + i)->n_un.n_strx : ((t_nlist_64 *)symtab + i)->n_un.n_strx);
		if (file->arch == ARCH_32) {
			init_mysym(file, &mysym, symname, (t_nlist *)symtab + i); // TODO Check for failure with overflow ???
		} else {
			init_mysym(file, &mysym, symname, (t_nlist_64 *)symtab + i);
		}
		fill_mysym(file, &mysym);

		ft_lstadd(&file->mysyms, ft_lstnew(&mysym, sizeof(t_mysymbol)));
		i++;
	}

	return SUCCESS;
}
