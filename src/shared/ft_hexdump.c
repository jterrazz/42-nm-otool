/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:58:59 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 18:26:15 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// Secure the hexdump
void ft_hexdump(void *start, uint64_t size, uint64_t printed_start, t_arch arch)
{
	uint8_t i;

	while (size) {
		i = size > 16 ? 16 : size;
		ft_printf("%0*llx%s", (arch == ARCH_32) ? 8 : 16, printed_start, "\t");

		while (size && i-- > 0) {
			ft_printf("%02x ", * (uint8_t *) start); // No space for last one ?
			start += 1;
			size -= 1;
		}
		ft_printf("\n");
		printed_start += 16;
	}
}
