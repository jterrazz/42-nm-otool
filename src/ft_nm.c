/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:51:31 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void usage(char const *argv[])
{
	ft_printf("USAGE: %s [options] <input files>\n", argv[0]);
	ft_printf("\nOPTIONS:\n\nGeneral options:\n\n");
	ft_printf("%-25s %s\n", "-n --numeric-sort", "Sort symbols by address");
	ft_printf("%-25s %s\n", "-r --reverse-sort", "Sort in reverse order");
	ft_printf("%-25s %s\n", "-g --extern-only", "Show only external symbols");
	ft_printf("%-25s %s\n", "-p --no-sort", "Show symbols in order encountered");
	ft_printf("%-25s %s\n", "-u --undefined-only", "Show only undefined symbols");
	ft_printf("%-25s %s\n", "-U --defined-only", "Show only defined symbols");
	ft_printf("%-25s %s\n", "-j --simple-output", "Print just the symbol's name");
	ft_printf("%-25s %s\n", "-a --debug-syms", "Show all symbols, even debugger only");
	ft_printf("%-25s %s\n", "-h --help", "Display available options");
}

int main(int argc, char const *argv[])
{
	t_env env;
	int i;

	i = 0;
	if (cmd_init(&env, argc, argv, BIN_NM) < 0)
		return (EXIT_FAILURE);

	if (env.flags & FLAG_HELP)
		usage(argv);
	else {
		if (!env.nfiles)
			cmd_start(&env, DEFAULT_FILE);
		while (i < env.nfiles) {
			cmd_start(&env, env.filenames[i]);
			i++;
		}
	}
	cmd_end(&env);
	return (EXIT_SUCCESS);
}
