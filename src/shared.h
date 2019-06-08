/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:47:37 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/08 11:20:25 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
#define SHARED_H

#define FAILURE -1
#define SUCCESS 0

#include <stdlib.h> // For uint32_t2

typedef enum { FALSE, TRUE } t_bool;
typedef enum { BIN_NM, BIN_OTOOL } t_bin;

typedef struct s_env {
	int argc;
	char const **argv;
	t_bin bin;
}				t_env;

int cmd_init_env(t_env *env, int argc, char const *argv[], t_bin bin);
int cmd_process_file(t_env *env, char const *filename);

#endif
