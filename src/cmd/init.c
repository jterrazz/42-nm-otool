/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 11:08:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/13 12:23:22 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shared.h"
#include "ft_printf.h"

// -n -n case
t_flag_detail g_flags[] = {
	{'n', "-numeric-sort", FLAG_N, BIN_NM},
	{'r', "-reverse-sort", FLAG_R, BIN_NM},
	{'g', "-extern-only", FLAG_G, BIN_NM},
	{'p', "-no-sort", FLAG_P, BIN_NM},
	{'u', "-undefined-only", FLAG_U, BIN_NM},
	{'U', "-defined-only", FLAG_UU, BIN_NM},
	{'j', "-simple-output", FLAG_J, BIN_NM},
	{'a', "-debug-syms", FLAG_A, BIN_NM},
	{'h', "-help", FLAG_HELP, BIN_NM | BIN_OTOOL},
	{0, "", 0, 0}
};

static t_flag_detail *get_flag(char *str, t_bin bin)
{
	int i;

	i = 0;
	while (g_flags[i].symbol) {
		if (g_flags[i].binaries & bin && ((g_flags[i].symbol == str[0] && str[0] && !str[1]) || !ft_strcmp(str, g_flags[i].fullname)))
			return g_flags + i;
		i++;
	}
	return NULL;
}

static int init_flags(t_env *env, int argc, char const *argv[], t_bin bin)
{
	int i;
	t_flag_detail *flag;

	i = 1;
	while (i < argc && argv[i][0] == '-') {
		if (!(flag = get_flag((char *) argv[i] + 1, bin))) {
			ft_printf("nmnm: Unknow command line argument %s. Try nmnm -help\n", argv[i]);
			return (-1);
		}
		env->flags = env->flags | flag->value;
		i++;
	}
	return i;
}

/*
** Returns the index of the first not flag argument, -1 for errors
*/

int cmd_init(t_env *env, int argc, char const *argv[], t_bin bin) {
	ft_bzero(env, sizeof(t_env));
	env->argc = argc;
	env->argv = argv;
	env->bin = bin;
	env->cputype = CPU_TYPE_X86_64;

	return init_flags(env, argc, argv, bin);
}
