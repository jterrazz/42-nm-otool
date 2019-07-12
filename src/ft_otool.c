/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 15:25:20 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./shared.h"
#include "ft_printf.h"

int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	if ((i = cmd_init(&env, argc, argv, BIN_OTOOL)) == -1)
		return EXIT_FAILURE;

	if (i == argc) {
		ft_printf("otool: at least one file must be specified\n"); // Replace otool by argv0
		return EXIT_FAILURE;
	}

	while (i < argc) {
		cmd_start(&env, argv[i]);
		i++;
	}

	// free(env->...)
	return (EXIT_SUCCESS);
}
