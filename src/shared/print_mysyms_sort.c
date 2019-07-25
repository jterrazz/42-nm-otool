/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mysyms_sort.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 10:34:50 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 10:41:21 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int				sort_mysyms_num(t_list *lst1, t_list *lst2)
{
	t_mysymbol *sym1;
	t_mysymbol *sym2;

	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
		return (0);
	sym1 = lst1->content;
	sym2 = lst2->content;
	if (sym1->value == sym2->value)
		return (ft_strcmp(sym1->name, sym2->name) > 0);
	return (sym1->value >= sym2->value);
}

int				sort_mysyms_alpha(t_list *lst1, t_list *lst2)
{
	t_mysymbol *sym1;
	t_mysymbol *sym2;

	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
		return (0);
	sym1 = lst1->content;
	sym2 = lst2->content;
	if (!ft_strcmp(sym1->name, sym2->name))
		return (sym1->value >= sym2->value);
	return (ft_strcmp(sym1->name, sym2->name) > 0);
}

static void		lst_swap(t_list *lst1, t_list *lst2)
{
	void		*content_tmp;
	size_t		content_size_tmp;

	content_tmp = lst1->content;
	content_size_tmp = lst1->content_size;
	lst1->content = lst2->content;
	lst1->content_size = lst2->content_size;
	lst2->content = content_tmp;
	lst2->content_size = content_size_tmp;
}

void			ft_lstsort(t_list *lst, int (*f)(t_list *lst1, t_list *lst2))
{
	t_list *to_replace;
	t_list *el;
	t_list *min;

	to_replace = lst;
	while (to_replace && to_replace->next)
	{
		min = NULL;
		el = to_replace;
		while (el)
		{
			if (!min || (*f)(min, el))
				min = el;
			el = el->next;
		}
		if (min && to_replace != min)
			lst_swap(to_replace, min);
		to_replace = to_replace->next;
	}
}

/*
** A B C D
** B A C D
** B C A D
** B C D A
*/

void			ft_lstreverse(t_list *lst)
{
	t_list *el;
	size_t count;
	size_t i;

	el = lst;
	count = 0;
	while (el)
	{
		count++;
		el = el->next;
	}
	while (--count > 0)
	{
		i = 0;
		el = lst;
		while (i++ < count)
		{
			if (el && el->next)
				lst_swap(el, el->next);
			el = el->next;
		}
	}
}
