/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 00:12:22 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./shared.h"

int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	i = cmd_init(&env, argc, argv, BIN_OTOOL);

	while (i < argc) {
		cmd_start(&env, argv[i]);
		i++;
	}

	// free(env->...)
	return (EXIT_SUCCESS);
}
