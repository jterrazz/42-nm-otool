/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 18:02:41 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "ft_printf.h"

static void usage(char const *argv[])
{
	ft_printf("Usage: %s <object file>\n", argv[0]);
}

int main(int argc, char const *argv[])
{
	t_env env;
	int i;

	i = 0;
	if (cmd_init(&env, argc, argv, BIN_OTOOL) == -1)
		return (EXIT_FAILURE);

	if (env.flags & FLAG_HELP)
		usage(argv);
	else {
		if (!env.nfiles) {
			ft_printf("error: %s: at least one file must be specified\n", argv[0]);
			return (EXIT_FAILURE);
		}
		while (i < env.nfiles) {
			cmd_start(&env, env.filenames[i]);
			i++;
		}
	}

	cmd_end(&env);
	return (EXIT_SUCCESS);
}
