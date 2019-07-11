/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:45:04 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/12 00:10:47 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../shared.h"
#include "libft.h"

// Where init_file = free_file
// Use ft_printf in errors FD ????
#include "ft_printf.h"
int cmd_start(t_env *env, char const *filename)
{
    struct stat buf;
    t_file file;
    int fd;
    char *ptr;

    ft_bzero(&file, sizeof(t_file));
    if ((fd = open(filename, O_RDONLY)) < 0)
        return FAILURE; // Print error no such file
    if (fstat(fd, &buf) < 0)
        return FAILURE; // Print error (we can use official errors)
    if (buf.st_size == 0) {
        ft_printf("%s The file was not recognized as a valid object file\n", filename);
        return FAILURE;
    }
    if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return FAILURE; //same
    init_file(&file, filename, buf.st_size, ptr);
    handle_file(env, &file);
    if (munmap(ptr, buf.st_size) < 0)
        return FAILURE; // same
    close(fd);

    return (SUCCESS);
}
