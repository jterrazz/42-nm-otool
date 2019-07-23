/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_symtab_mysym.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 19:58:50 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 23:19:53 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** The list is defined in <mach-o/stab.h>
** https://opensource.apple.com/source/cctools/cctools-773/include/mach-o/stab.h
*/

static t_debug_symbol g_debug_symbols[] =
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
	{"LENG", 0xfe},
	{0, 0}
};

static t_mysection *find_mysection(t_list *lst, uint8_t n_sect)
{
	while (lst && lst->content) {
		if (((t_mysection *)lst->content)->index == n_sect)
			return lst->content;
		lst = lst->next;
	}

	return NULL;
}

static void match_sym_section(t_list *mysect_lst, t_mysymbol *mysym)
{
	t_mysection *mysect;

	if ((mysect = find_mysection(mysect_lst, mysym->sect)))
	{
		if (!ft_strcmp(mysect->name, SECT_TEXT))
			mysym->type_p = 'T';
		else if (!ft_strcmp(mysect->name, SECT_DATA))
			mysym->type_p = 'D';
		else if (!ft_strcmp(mysect->name, SECT_BSS))
			mysym->type_p = 'B';
		else
			mysym->type_p = 'S';
		if (!(mysym->type & N_EXT))
			mysym->type_p -= 'A' - 'a';
	}
}

char *get_debug_symbol(uint16_t type)
{
	int i;

	i = 0;
	while (g_debug_symbols[i].symbol) {
		if (g_debug_symbols[i].typevalue == type)
			 return g_debug_symbols[i].symbol;
		i++;
	}
	return (NULL);
}

void fill_mysym(t_file *file, t_mysymbol *mysym)
{
	if (N_STAB & mysym->type)
	{
		mysym->type_p = '-';
		mysym->debug_symbol = get_debug_symbol(mysym->type);
	}
	else if ((N_TYPE & mysym->type) == N_UNDF)
	{
		if (mysym->namefailed)
			mysym->type_p = 'C';
		else if (mysym->type & N_EXT)
			mysym->type_p = 'U';
		else
			mysym->type_p = '?';
	}
	else if ((N_TYPE & mysym->type) == N_SECT)
		match_sym_section(file->mysects, mysym);
	else if ((N_TYPE & mysym->type) == N_ABS)
		mysym->type_p = 'A';
	else if ((N_TYPE & mysym->type) == N_INDR)
		mysym->type_p = 'I';
}

/*
** Copy basic informations about a t_nlist element to the t_mysymbol variable
** I do that for not dealing with 32/64 structures anymore
*/

t_mysymbol *init_mysym(t_file *file, t_mysymbol *mysym, char *symname, void *sym)
{
	int namefailed;

	ft_bzero(mysym, sizeof(t_mysymbol));
	namefailed = 0;
	mysym->type_p = ' ';
	mysym->name = ft_strdup_overflow(file, symname, '\n', &namefailed);
	if (namefailed)
		mysym->namefailed = TRUE;
	if (!mysym->name)
		return (NULL);
	if (file->arch == ARCH_32) {
		mysym->type = swapif_u32(file, ((t_nlist *)sym)->n_type);
		mysym->sect = swapif_u32(file, ((t_nlist *)sym)->n_sect);
		mysym->desc = swapif_u32(file, ((t_nlist *)sym)->n_desc);
		mysym->value = swapif_u32(file, ((t_nlist *)sym)->n_value);
	} else {
		mysym->type = swapif_u64(file, ((t_nlist_64 *)sym)->n_type);
		mysym->sect = swapif_u64(file, ((t_nlist_64 *)sym)->n_sect);
		mysym->desc = swapif_u64(file, ((t_nlist_64 *)sym)->n_desc);
		mysym->value = swapif_u64(file, ((t_nlist_64 *)sym)->n_value);
	}
	return (mysym);
}
