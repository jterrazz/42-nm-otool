/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:58:59 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:48:04 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** Print the hex value of each byte from the `start` pointer, and for `size` bytes.
** `start_address` is the printed address from the value at *start.
*/

void ft_hexdump(void *start, uint64_t size, uint64_t start_address, t_arch arch)
{
	uint8_t i;

	while (size)
	{
		i = size > 16 ? 16 : size;
		ft_printf("%0*llx%s", (arch == ARCH_32) ? 8 : 16, start_address, "\t");

		while (size && i-- > 0)
		{
			ft_printf("%02x ", * (uint8_t *) start);
			start += 1;
			size -= 1;
		}
		ft_printf("\n");
		start_address += 16;
	}
}
