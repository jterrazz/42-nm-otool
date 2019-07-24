/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 04:26:58 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void usage(char const *bin)
{
	ft_printf("Usage: %s <object file>\n", bin);
}

int main(int argc, char const *argv[])
{
	t_env env;
	int i;

	i = 0;
	if (cmd_init(&env, argc, argv, BIN_OTOOL) == -1)
		return (EXIT_FAILURE);

	if (env.flags & FLAG_HELP)
		usage(argv[0]);
	else {
		if (!env.nfiles) {
			ft_printf("error: %s: at least one file must be specified\n", argv[0]);
			return (EXIT_FAILURE);
		}
		while (i < env.nfiles) {
			if (cmd_start(&env, env.filenames[i]))
				return EXIT_FAILURE;
			i++;
		}
	}

	cmd_end(&env);
	return (EXIT_SUCCESS);
}
