/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/08 11:20:40 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

int main(int argc, char const *argv[]) {
	t_env env;
	int i;

	i = cmd_init_env(&env, argc, argv, BIN_NM);

	while (i < argc) {
		cmd_process_file(&env, argv[i]);
		i++;
	}

	// free(env->...)
	return (EXIT_SUCCESS);
}
