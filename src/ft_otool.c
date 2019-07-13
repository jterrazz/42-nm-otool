/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/13 13:45:32 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./shared.h"
#include "ft_printf.h"

static void print_usage(char const *argv[])
{
	ft_printf("Usage: %s <object file>\n", argv[0]);
}

int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	if ((i = cmd_init(&env, argc, argv, BIN_OTOOL)) == -1)
		return (EXIT_FAILURE);

	if (env.flags & FLAG_HELP)
		print_usage(argv);
	else {
		if (i == argc) {
			ft_printf("error: %s: at least one file must be specified\n", argv[0]);
			return (EXIT_FAILURE);
		}

		while (i < argc) {
			cmd_start(&env, argv[i]);
			i++;
		}
	}

	// free(env->...)
	return (EXIT_SUCCESS);
}
