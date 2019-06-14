/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mysyms.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 15:13:32 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/14 13:04:42 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static int sort_mysyms_alpha(t_list *lst1, t_list *lst2) {
	t_symbol *sym1;
	t_symbol *sym2;

	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
		return 1;
	sym1 = lst1->content;
	sym2 = lst2->content;
	if (!ft_strcmp(sym1->name, sym2->name)) {
		return sym1->value >= sym2->value;
	}
	return (ft_strcmp(sym1->name, sym2->name) > 0);
}

static void lst_swap(t_list *lst1, t_list *lst2)
{
	void *content_tmp;
	size_t content_size_tmp;

	content_tmp = lst1->content;
	content_size_tmp = lst1->content_size;
	lst1->content = lst2->content;
	lst1->content_size = lst2->content_size;
	lst2->content = content_tmp;
	lst2->content_size = content_size_tmp;
}

static void ft_lstsort(t_list *lst, int (*f)(t_list *lst1, t_list *lst2)) {
	t_list *to_replace;
	t_list *el;
	t_list *min;

	to_replace = lst;
	while (to_replace && to_replace->next) {
		min = NULL;
		el = to_replace;
		while (el) {
			if (!min || (*f)(min, el)) {
				min = el;
			}
			el = el->next;
		}
		if (min && to_replace != min)
			lst_swap(to_replace, min);
		to_replace = to_replace->next;
	}
}
#include <stdio.h>
void print_mysyms(t_file *file)
{
	t_list *symlst;
	t_symbol *sym;
	uint8_t left_padding;

	left_padding = (file->arch == ARCH_32) ? 8 : 16;
	symlst = file->mysyms;
	ft_lstsort(symlst, sort_mysyms_alpha);
	// ft_printf("Will print mysyms\n");

	while (symlst) {
		sym = symlst->content;
		if (sym->type_p == '-') // Maybe do something with that
			;
		else if (sym->type_p == 'I') {
			ft_printf("%*c %c %s (indirect for %s)\n", left_padding, ' ', sym->type_p, sym->name, sym->name);
		} else if (sym->type_p != 'U')
			ft_printf("%0*llx %c %s\n", left_padding, sym->value, sym->type_p, sym->name);
		else
			ft_printf("%*c %c %s\n", left_padding, ' ', sym->type_p, sym->name);
		symlst = symlst->next;
	}
}
