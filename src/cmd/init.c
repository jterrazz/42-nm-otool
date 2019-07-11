/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 11:08:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 00:12:09 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shared.h"

/*
** Returns the index of the first not flag argument
*/

int cmd_init(t_env *env, int argc, char const *argv[], t_bin bin) {
	ft_bzero(env, sizeof(t_env));
	env->argc = argc;
	env->argv = argv;
	env->bin = bin;
	env->cputype = CPU_TYPE_X86_64;

	return 1;
}
