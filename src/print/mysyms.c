/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mysyms.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 15:13:32 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/12 21:28:48 by jterrazz         ###   ########.fr       */
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
	return (ft_strcmp(sym1->name, sym2->name) > 0);
}

static void ft_lstsort(t_list *lst, int (*f)(t_list *lst1, t_list *lst2)) {
	t_list *start;
	t_list *el;
	void *content_tmp;
	size_t content_size_tmp;

	if (!lst)
		return;
	start = lst;
	el = lst;
	while (el && el->next)
	{
		if ((*f)(el, el->next)) {
			content_tmp = el->content;
			content_size_tmp = el->content_size;
			el->content = el->next->content;
			el->content_size = el->next->content_size;
			el->next->content = content_tmp;
			el->next->content_size = content_size_tmp;
			el = start;
		} else {
			el = el->next;
		}
	}
}

void print_mysyms(t_file *file)
{
	t_list *symlst;
	t_symbol *sym;
	uint8_t left_padding;

	left_padding = (file->arch == ARCH_32) ? 8 : 16;
	symlst = file->mysyms;
	ft_lstsort(symlst, sort_mysyms_alpha);

	while (symlst) {
		sym = symlst->content;
		if (sym->type & N_STAB) // Maybe do something with that
			break;
		if (sym->type_p != 'U') // TODO Find better condition ?
			ft_printf("%0*llx %c %s\n", left_padding, sym->value, sym->type_p, sym->name);
		else
			ft_printf("%*c %c %s\n", left_padding, ' ', sym->type_p, sym->name);
		symlst = symlst->next;
	}
}
