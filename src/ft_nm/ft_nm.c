/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 15:07:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/05/29 23:32:48 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h> // For uint32_t2
#include "ft_printf.h"

uint32_t ft_nm(int fd) {
	struct stat buf;

	if (fstat(fd, &buf) < 0) {
		ft_printf("fstat error");
		return 1;
	}

	ft_printf("");

	return 0;
}

int main(int argc, char const *argv[]) {
	int i;
	int fd;

	i = 0;
	if (argc < 2) {
		//yo
		ft_printf("no args\n");
		return 1;
	}
	while (i < argc) {
		if ((fd = open(argv[i], O_RDONLY)) < 0) {
			ft_printf("open error\n");
		} else if (ft_nm(fd) < 0) {
			ft_printf("nm error\n");
			return 1;
		}
		i++;
	}
	return 0;
}
