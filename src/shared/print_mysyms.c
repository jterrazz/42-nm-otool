/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mysyms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 15:13:32 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 20:30:47 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int sort_mysyms_num(t_list *lst1, t_list *lst2)
{
	t_mysymbol *sym1;
	t_mysymbol *sym2;

	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
		return 0;
	sym1 = lst1->content;
	sym2 = lst2->content;
	if (sym1->value == sym2->value)
		return ft_strcmp(sym1->name, sym2->name) > 0;
	return (sym1->value >= sym2->value);
}

static int sort_mysyms_alpha(t_list *lst1, t_list *lst2)
{
	t_mysymbol *sym1;
	t_mysymbol *sym2;

	if (!lst1 || !lst2 || !lst1->content || !lst2->content)
		return 0;
	sym1 = lst1->content;
	sym2 = lst2->content;
	if (!ft_strcmp(sym1->name, sym2->name))
		return sym1->value >= sym2->value;
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

/*
 A B C D
 B A C D
 B C A D
 B C D A
*/

static void ft_lstreverse(t_list *lst)
{
	t_list *el;
	size_t count;
	size_t i;

	el = lst;
	count = 0;
	while (el) {
		count++;
		el = el->next;
	}
	while(--count > 0) {
		i = 0;
		el = lst;
		while (i++ < count) {
			if (el && el->next)
				lst_swap(el, el->next);
			el = el->next;
		}
	}
}


static void print_mysyms_line(uint8_t left_padding, t_mysymbol *sym, t_bool minimal)
{
	if (minimal)
		ft_printf("%s\n", sym->name);
	else if (sym->type_p == '-')
		ft_printf("%0*llx %c %02d %5s %s\n", left_padding, sym->value, sym->type_p, sym->sect, sym->debug_symbol, sym->name);
	else if (sym->type_p == 'I')
		ft_printf("%*c %c %s (indirect for %s)\n", left_padding, ' ', sym->type_p, sym->name, sym->name);
	else if (sym->type_p != 'U')
		ft_printf("%0*llx %c %s\n", left_padding, sym->value, sym->type_p, sym->name);
	else
		ft_printf("%*c %c %s\n", left_padding, ' ', sym->type_p, sym->name);
}

void print_mysym(t_env *env, t_file *file, t_mysymbol *sym)
{
	if ((!(env->flags & FLAG_A) && sym->type_p == '-')
		|| (env->flags & FLAG_U && sym->type_p != 'U')
		|| (env->flags & FLAG_UU && sym->type_p == 'U')
		|| (env->flags & FLAG_G && (sym->type_p < 'A' || sym->type_p >'Z'))
		|| (!(env->flags & FLAG_A) && sym->type_p == '-')
	)
		;
	else
		print_mysyms_line((file->arch == ARCH_32)
		? 8 : 16, sym, env->flags & FLAG_U || env->flags & FLAG_J);
}

void print_mysyms(t_env *env, t_file *file)
{
	t_list		*symlst;
	t_mysymbol	*sym;
	t_bool		reverse;

	symlst = file->mysyms;
	reverse = env->flags & FLAG_R;
	if (!(env->flags & FLAG_P))
		ft_lstsort(symlst, env->flags & FLAG_N
			? sort_mysyms_num : sort_mysyms_alpha);
	else
		reverse = !reverse;
	if (env->flags & FLAG_R || env->flags & FLAG_P)
		ft_lstreverse(symlst);
	while (symlst)
	{
		sym = symlst->content;
		print_mysym(env, file , sym);
		symlst = symlst->next;
	}
}
