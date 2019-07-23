/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 11:08:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:59:32 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

t_flag_info g_flags[] = {
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

static t_flag_info *get_flag(char *str, t_bin bin)
{
	int i;

	i = 0;
	while (g_flags[i].shortname) {
		if (g_flags[i].binaries & bin && ((g_flags[i].shortname == str[0] && !str[1]) || !ft_strcmp(str, g_flags[i].fullname)))
			return g_flags + i;
		i++;
	}
	return NULL;
}

static int set_flags(t_env *env, char *argv, t_bin bin)
{
	t_flag_info *flag;

	if (!(flag = get_flag(argv + 1, bin)))
	{
		ft_printf("%s: Unknow command line argument '%s'. Try '%s' -help\n", argv, *argv, argv); // TODO
		return (FAILURE);
	}
	if (env->flags & flag->value)
	{
		ft_printf("%s: for the %s option: may only occur zero or one times!\n", argv, flag->fullname); // TODO
		return (FAILURE);
	}
	env->flags |= flag->value;
	return (SUCCESS);
}

static int set_args(t_env *env, int argc, char const *argv[], t_bin bin)
{
	char **filenames;

	while (--argc && ++argv)
	{
		if ((*argv)[0] == '-')
		{
			if (set_flags(env, (char *)*argv, bin))
				return (FAILURE);
		}
		else
		{
			if (!(filenames = malloc((env->nfiles + 1) * sizeof(char *))))
				return (-1);
			ft_memmove(filenames, env->filenames, env->nfiles * sizeof(char *));
			free(env->filenames);
			filenames[env->nfiles] = (char *) *argv;
			env->filenames = (char const **) filenames;
			env->nfiles++;
		}
	}
	return (SUCCESS);
}

/*
** Initialise a t_env variable with argv values.
** It saves the flags values in env->flags.
** Filenames are saved in the env->filenames list.
** Returns SUCCESS or FAILURE.
*/

int cmd_init(t_env *env, int argc, char const *argv[], t_bin bin) {
	ft_bzero(env, sizeof(t_env));
	env->execname = argv[0];
	env->bin = bin;
	env->cputype = CPU_TYPE_X86_64;

	return set_args(env, argc, argv, bin);
}
