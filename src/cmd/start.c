/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:45:04 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 08:06:53 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "nm_otool.h"
#include "libft.h"
#include "ft_printf.h"

// Where init_file = free_file
// Use ft_printf in errors FD ????

int cmd_start(t_env *env, char const *filename)
{
    struct stat buf;
    t_file file;
    int fd;
    char *ptr;

    ft_bzero(&file, sizeof(t_file));
    if ((fd = open(filename, O_RDONLY)) < 0) {
        ft_printf("error: %s: Can't open file %s\n", env->argv[0], filename);
        return FAILURE;
    }
    if (fstat(fd, &buf) < 0) {
        ft_printf("error: %s: Can't fstat file %s\n", env->argv[0], filename);
        return FAILURE;
    }
    if (buf.st_size == 0) {
        ft_printf("%s: is not an object file\n", filename);
        return FAILURE;
    }
    if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
        ft_printf("error: %s: Can't mmap file %s\n", env->argv[0], filename);
        return FAILURE;
    }
    init_file(&file, filename, buf.st_size, ptr);
    handle_binary(env, &file);
    // Free file here
    if (munmap(ptr, buf.st_size) < 0 || close(fd) < 0)
        return FAILURE;
    return (SUCCESS);
}
