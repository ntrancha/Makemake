/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makemake.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntrancha <ntrancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/03 17:00:23 by ntrancha          #+#    #+#             */
/*   Updated: 2015/08/03 18:06:09 by ntrancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"

char        *get_header(char *file)
{
    char    *pathfile;
    char    *content;

    pathfile = ft_strmjoin("libft/includes/", file, ".h");
    content = ft_get_file(pathfile);
    ft_strdel(&pathfile);
    return (content);
}

void        add_source(char *line)
{
    int     index;
    int     start;
    char    *file;

    index = 0;
    while (line[index] != ' ' && line[index] != '\t')
        index++;
    while (line[index] == ' ' || line[index] == '\t' || line[index] == '*')
        index++;
    start = index;
    while (line[index] != '(')
        index++;
    file = ft_strsub(line, start, index - start);
    ft_putendl(file);
    ft_strdel(&file);
}

void        add_header(char *file)
{
    char    *content;
    char    *line;
    char    *source;
    int     num;

    content = get_header(file);
    num = 0;
    line = ft_strgetline(content, num++);
    while (line)
    {
        if (ft_strcchr(line, ");") != 0 && ft_strcchr(line, "ft_") != 0)
            add_source(line);
        ft_strdel(&line);
        line = ft_strgetline(content, num++);
        if (ft_strcmp(line, "#endif") == 0)
        {
            ft_strdel(&line);
            //line = NULL;
        }
    }
    ft_strdel(&content);
}

int         main(int argc, char **argv)
{
    t_opt   *files;
    char    *file;

    files = ft_optget(argc, argv);
    file = ft_optgetopt_next(files);
    while (file)
    {
        add_header(file);
        ft_strdel(&file);
        file = ft_optgetopt_next(files);
    }
    ft_optdel(files);
    return (1);
}
