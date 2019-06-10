/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:45:04 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 17:34:50 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../shared.h"
#include "libft.h"

int cmd_process_file(t_env *env, char const *filename)
{
    struct stat buf;
    t_file file;
    int fd;
    char *ptr;

    ft_bzero(&file, sizeof(t_file));
    if ((fd = open(filename, O_RDONLY)) < 0)
        return FAILURE; // Print error no such file
    file.filename = filename;
    if (fstat(fd, &buf) < 0)
        return FAILURE; // Print error (we can use official errors)
    file.filesize = buf.st_size;
    if ((ptr = mmap(NULL, file.filesize, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return FAILURE; //same
    file.start = ptr;
    handle_file(env, &file, file.start);
    if (munmap(ptr, file.filesize) < 0)
        return FAILURE; // same
    close(fd);
    // free file (with mysects ...)

    return (SUCCESS);
}
