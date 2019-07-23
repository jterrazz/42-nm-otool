/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 08:42:18 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// 'n', "-numeric-sort", FLAG_N, BIN_NM},
// {'r', "-reverse-sort", FLAG_R, BIN_NM},
// {'g', "-extern-only", FLAG_G, BIN_NM},
// {'p', "-no-sort", FLAG_P, BIN_NM},
// {'u', "-undefined-only", FLAG_U, BIN_NM},
// {'U', "-defined-only", FLAG_UU, BIN_NM},
// {'j', "-simple-output", FLAG_J, BIN_NM},
// {'a', "-debug-syms", FLAG_A, BIN_NM},
// {'h', "-help", FLAG_

//nm ./ft_nm -r
//nm ./ft_nm -r -r

static void print_usage(char const *argv[])
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

int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	if ((i = cmd_init(&env, argc, argv, BIN_NM)) < 0)
		return (EXIT_FAILURE);

	if (env.flags & FLAG_HELP)
		print_usage(argv);
	else {
		if (i == argc)
			cmd_start(&env, DEFAULT_FILE);
		while (i < argc) {
			cmd_start(&env, argv[i]);
			i++;
		}
	}
	cmd_end(&env);

	// free(env->...)
	return (EXIT_SUCCESS);
}
