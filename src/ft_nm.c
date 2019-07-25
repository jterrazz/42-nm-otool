/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 11:21:42 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	usage(char const *bin)
{
	ft_printf("USAGE: %s [options] <input files>\n", bin);
	ft_printf("\nOPTIONS:\n\nGeneral options:\n\n");
	ft_printf("  %-25s %s\n", "-n --numeric-sort", "Sort symbols by address");
	ft_printf("  %-25s %s\n", "-r --reverse-sort", "Sort in reverse order");
	ft_printf("  %-25s %s\n", "-g --extern-only",
		"Show only external symbols");
	ft_printf("  %-25s %s\n", "-p --no-sort",
		"Show symbols in order encountered");
	ft_printf("  %-25s %s\n", "-u --undefined-only",
		"Show only undefined symbols");
	ft_printf("  %-25s %s\n", "-U --defined-only", "Show only defined symbols");
	ft_printf("  %-25s %s\n", "-j --simple-output",
		"Print just the symbol's name");
	ft_printf("  %-25s %s\n", "-a --debug-syms",
		"Show all symbols, even debugger only");
	ft_printf("  %-25s %s\n", "-h --help", "Display available options");
}

int			main(int argc, char const *argv[])
{
	t_env	env;
	int		i;

	i = 0;
	if (cmd_init(&env, argc, argv, BIN_NM) < 0)
		return (EXIT_FAILURE);
	if (env.flags & FLAG_HELP)
		usage(argv[0]);
	else
	{
		if (!env.nfiles)
		{
			if (cmd_start(&env, DEFAULT_FILE))
				return (EXIT_FAILURE);
		}
		while (i < env.nfiles)
		{
			if (env.nfiles > 1)
				ft_printf("\n%s:\n", env.filenames[i]);
			if (cmd_start(&env, env.filenames[i]))
				return (EXIT_FAILURE);
			i++;
		}
	}
	cmd_end(&env);
	return (EXIT_SUCCESS);
}
