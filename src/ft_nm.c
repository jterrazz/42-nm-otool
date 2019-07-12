/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 15:24:51 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./shared.h"
// Nm default to a.out
// otool must have one file
// Probably use cmd_start
int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	if ((i = cmd_init(&env, argc, argv, BIN_NM)) == -1)
		return EXIT_FAILURE;

	if (i == argc)
		cmd_start(&env, "a.out");

	while (i < argc) {
		cmd_start(&env, argv[i]);
		i++;
	}

	// free(env->...)
	return (EXIT_SUCCESS);
}
