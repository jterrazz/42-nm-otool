/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mysyms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 15:13:32 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 15:43:23 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	print_mysyms_line(uint8_t left_padding,
	t_mysymbol *sym, t_bool minimal)
{
	if (minimal)
		ft_printf("%s\n", sym->name);
	else if (sym->type_p == '-')
		ft_printf("%0*llx %c %02d %5s %s\n", left_padding, sym->value,
			sym->type_p, sym->sect, sym->debug_symbol, sym->name);
	else if (sym->type_p == 'I')
		ft_printf("%*c %c %s (indirect for %s)\n", left_padding, ' ',
			sym->type_p, sym->name, sym->name);
	else if (sym->type_p != 'U')
		ft_printf("%0*llx %c %s\n", left_padding, sym->value, sym->type_p,
			sym->name);
	else
		ft_printf("%*c %c %s\n", left_padding, ' ', sym->type_p, sym->name);
}

void		print_mysym(t_env *env, t_file *file, t_mysymbol *sym)
{
	if ((!(env->flags & FLAG_A) && sym->type_p == '-')
		|| (env->flags & FLAG_U && sym->type_p != 'U')
		|| (env->flags & FLAG_UU && sym->type_p == 'U')
		|| (env->flags & FLAG_G && (sym->type_p < 'A' || sym->type_p > 'Z'))
		|| (!(env->flags & FLAG_A) && sym->type_p == '-'))
		;
	else
		print_mysyms_line((file->arch == ARCH_32)
		? 8 : 16, sym, env->flags & FLAG_U || env->flags & FLAG_J);
}

void		print_mysyms(t_env *env, t_file *file)
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
		print_mysym(env, file, sym);
		symlst = symlst->next;
	}
}
