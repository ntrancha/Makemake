/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dependenC.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntrancha <ntrancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/18 03:48:33 by ntrancha          #+#    #+#             */
/*   Updated: 2015/08/18 05:41:21 by ntrancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"

void        recup(char *proto, t_list *files, char *path)
{
    char    *tmp2;
    char    *file;
    int     start;

    start  = 0;
    while (proto[start] && proto[start] != ' ' && proto[start] != '\t')
        start++;
    while (proto[start] && (proto[start] == ' ' || proto[start] == '\t'))
        start++;
    while (proto[start] && proto[start] == '*')
        start++;
    tmp2 = ft_strsub(proto, start, ft_strlen(proto) - start);
    start  = 0;
    while (tmp2[start] && tmp2[start] != '(')
        start++;
    file = ft_strsub(tmp2, 0, start);
    ft_strdel(&tmp2);
    tmp2 = ft_strjoin(path, "/");
    ft_listadd(files, (void *)ft_strmjoin(tmp2, file, ".c"));
    ft_strdel(&tmp2);
    ft_strdel(&path);
    ft_strdel(&file);
}


void        parse_header(char *content, t_list *files, char *path, char *path_src)
{
    int     start;
    int     max;
    char    *line;

    start = 0;
    max = 0;
    while (content[start])
        if (content[start++] == '\n')
            max++;
    start = 10;
    line = NULL;
    while (start++ < max - 3)
    {
        if (line)
            ft_strdel(&line);
        line = ft_strgetline(content, start);
        if (line && ft_strlen(line) > 5)
            if (line[ft_strlen(line) - 1] == ';' && line[ft_strlen(line) - 2] == ')')
                recup(line, files, ft_strmjoin(path_src, "/", path));
    }
    ft_strdel(&line);
}

char        *header_exist(char *file, char *path)
{
    char    *tmp;
    char    *tmp2;

    if (ft_strcmp(path, ".") == 0)
        tmp = ft_strdup(path);
    else
        tmp = ft_strmjoin(path, "/", file);
    tmp2 = ft_strmjoin(tmp, "/", "includes");
    ft_strdel(&tmp);
    tmp = ft_strmjoin(tmp2, "/", file);
    ft_strdel(&tmp2);
    tmp2 = ft_strjoin(tmp, ".h");
    ft_strdel(&tmp);
    return (tmp2);
}

t_list      *traitement(char *file, t_list *files, char *path, t_list *headers)
{
    char    *header;
    char    *content;

    header = header_exist(file, path);
    if (!header)
        return (NULL); 
    content = ft_get_file(header);
    if (content)
    {
        parse_header(content, files, file, path);
        ft_strdel(&content);
        ft_listadd(headers, (void *)header);
    }
    else
    {
        ft_putstr_fd(file, 2);
        ft_putstr_fd(": Error\n", 2);
        ft_strdel(&header);
    }
    return (NULL);
}

void        make_header(t_list *list, char *path_src)
{
    t_node  *node;
    char    *tmp;

    node = list->start;
    ft_putendl("#ifndef LIBFT_H");
    ft_putendl("# define LIBFT_H\n");
    while (node)
    {
        ft_putstr("# include \"");
        tmp = ft_strsub(node->content, ft_strlen(path_src) + 1, ft_strlen(node->content));
        ft_putstr(tmp);
        ft_strdel(&tmp);
        ft_putendl("\"");
        node = node->next;
    }
    ft_putendl("\n#endif");
}

int         main(int argc, char **argv)
{
    t_opt   *options;
    char    *option;
    char    *path_src;
    t_list  *files;
    t_list  *headers;

    options = ft_optget(argc, argv);
    files = ft_listcreate();
    headers = ft_listcreate();
    option = ft_optgetopt_next(options);
    path_src = ft_strdup("../minilibft/src");
    while (option)
    {
        traitement(option, files, path_src, headers);
        ft_strdel(&option);
        option = ft_optgetopt_next(options);
    }
    make_header(headers, path_src);
    ft_listputstr(files, ft_putendl);
    ft_strdel(&path_src);
    ft_listdel(files, ft_memdel);
    ft_listdel(headers, ft_memdel);
    ft_optdel(options);
}