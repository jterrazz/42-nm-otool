/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 16:26:00 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

void parse_mach_file(t_env *env, t_file *file, uint32_t magic) {
	t_arch arch;
	arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
	uint64_t ncmds;
	t_load_command *lc;
	(void)env;

	file->arch = arch;
	ncmds = (arch == ARCH_32) ?
		((t_mach_header *)(file->start))->ncmds : // inverted
		((t_mach_header_64 *)(file->start))->ncmds; // inverted

	lc = (t_load_command *)(file->start +
		((arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64))); // secure
	// Section number (not needed i guess)

	// TODO Put this in dedicated function (load_command)
	// + Do clearer way to separate otool/nm
	while (ncmds--) {
		if (lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
			parse_segment(env, file, lc, arch);
		else if (env->bin == BIN_NM && lc->cmd == LC_SYMTAB) // 64 / If no otool
			parse_symtab(file, (void *)lc, arch);
		lc = (void *)lc + lc->cmdsize;
	}
}

// static int sort_mysyms_alpha(t_list *lst1, t_list *lst2) {
// 	t_symbol *sym1;
// 	t_symbol *sym2;
//
// 	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
// 		return 1;
// 	sym1 = lst1->content;
// 	sym2 = lst2->content;
// 	// ft_printf("s1 :%s, s3 %s\n",sym1->name, sym2->name);
// 	return (ft_strcmp(sym1->name, sym2->name) > 0);
// }
//
// static void ft_lstsort(t_list *lst, int (*f)(t_list *lst1, t_list *lst2)) {
// 	t_list *start;
// 	t_list *el;
// 	void *content_tmp;
// 	size_t content_size_tmp;
//
// 	if (!lst)
// 		return;
// 	start = lst;
// 	el = lst;
// 	int i = 30;
// 	t_list *el2;
// 	while (i-- && el && el->next)
// 	{
// 		if ((*f)(el, el->next)) {
// 			content_tmp = el->content;
// 			content_size_tmp = el->content_size;
// 			el->content = el->next->content;
// 			el->content_size = el->next->content_size;
// 			el->next->content = content_tmp;
// 			el->next->content_size = content_size_tmp;
// 			el = start;
//
//
// 			el2 = start;
// 			while (el2) {
// 				ft_printf("%x \n", el2->next);
// 				// ft_printf("%s ", ((t_symbol *)(el2->content))->name);
// 				el2 = el2->next;
// 			}
// 			ft_printf("\n\n");
// 		} else {
// 			el = el->next;
// 		}
// 	}
// }

void handle_file(t_env *env, t_file *file) {
	uint32_t magic;
	uint32_t filetype;
	// FILE IS PROBABLY DEFINED HERE (BECAUSE FILE IS THE  EXECUTABLE AND NOT ARCHIVE)
	magic = *(uint32_t *)(file->start);
	filetype = ((t_mach_header *)(file->start))->filetype; // 64bits

	// Handle ar
	// Handle fat
	// Handle not supported
	if (magic == MH_MAGIC || magic == MH_CIGAM || magic == MH_MAGIC_64 || magic == MH_CIGAM_64) { // Check cigam is working
		parse_mach_file(env, file, magic);
		if (env->bin == BIN_NM) {
			// ft_lstsort(file->mysyms, sort_mysyms_alpha);


			t_list *tmp = file->mysyms;
			while (tmp) {
				tmp = tmp->next;
			}


		}
	} else
		ft_printf("Not a valid file"); // Do a better msg ???
	// Handle 64
}
