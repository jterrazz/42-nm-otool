/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 11:08:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/12 20:35:08 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shared.h"

/*
** Returns the index of the last flag found or 0
*/

int cmd_init_env(t_env *env, int argc, char const *argv[], t_bin bin) {
	// ft_bzero(env);
	env->argc = argc;
	env->argv = argv;
	env->bin = bin;
	env->cputype = CPU_TYPE_X86_64;

	return 1;
}
