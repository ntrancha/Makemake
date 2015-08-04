/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_source.c                                       :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: ntrancha <ntrancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/03 17:00:23 by ntrancha          #+#    #+#             */
/*   Updated: 2015/08/04 05:06:52 by ntrancha         ###   ########.fr       */
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

char        *source(char *line)
{
    int     index;
    int     start;
    char    *file;
    char    *source;

    index = 0;
    while (line[index] != ' ' && line[index] != '\t')
        index++;
    while (line[index] == ' ' || line[index] == '\t' || line[index] == '*')
        index++;
    start = index;
    while (line[index] != '(')
        index++;
    file = ft_strsub(line, start, index - start);
    source = ft_strjoin(file, ".c");
    ft_strdel(&file);
    return (source);
}

void        add_header(char *file, t_list *list)
{
    char    *content;
    char    *line;
    char    *src;
    int     num;

    content = get_header(file);
    num = 0;
    line = ft_strgetline(content, num++);
    while (line)
    {
        if (ft_strcchr(line, ");") != 0 && ft_strcchr(line, "ft_") != 0)
        {
            src = source(line);
            ft_listadd(list, (void*)src);
        }
        ft_strdel(&line);
        line = ft_strgetline(content, num++);
        if (ft_strcmp(line, "#endif") == 0)
            ft_strdel(&line);
    }
    ft_strdel(&content);
}

void        col0(t_list *str, char *content, int size, int max)
{
    ft_listadd(str, ft_strdup("\t\t"));
    ft_listadd(str, content);
    while (size % 4 != 0)
    {
        ft_listadd(str, ft_strdup(" "));
        size++;
    }
    while (size + 4 <= max)
    {
        ft_listadd(str, ft_strdup("\t"));
        size += 4;
    }
}

void        col1(t_list *str, char *content, int size, int max)
{
    ft_listadd(str, content);
    while (size % 4 != 0)
    {
        ft_listadd(str, ft_strdup(" "));
        size++;
    }
    while (size + 4 <= max)
    {
        ft_listadd(str, ft_strdup("\t"));
        size += 4;
    }
    ft_listadd(str, ft_strdup("\\\n"));
}

void        col2(t_list *str, char *content, int size, int max)
{
    ft_listadd(str, ft_strdup("FILE =\t"));
    ft_listadd(str, content);
    while (size % 4 != 0)
    {
        ft_listadd(str, ft_strdup(" "));
        size++;
    }
    while (size + 4 <= max)
    {
        ft_listadd(str, ft_strdup("\t"));
        size += 4;
    }
}

int         add_source(t_node *node, t_list *str, int col, int max)
{
    int     size;
    char    *content;

    size = ft_strlen((char *)node->content);
    content = ft_strdup((char *)node->content);
    if (col == -1)
    {
        col2(str, content, size, max);
        return (0);
    }
    col = (col) ? 0 : 1;
    if (col == 0)
        col0(str, content, size, max);
    else
        col1(str, content, size, max);
    return (col);
}

void        create(t_list *list)
{
    int     max;
    t_node  *node;
    t_list  *str;
    char    *content;
    int     col;

    max = (((ft_liststrlenmax(list) / 4) + 1) * 4);
    node = list->start;
    col = -1;
    str = ft_listcreate();
    while (node)
    {
        col = add_source(node, str, col, max);
        node = node->next;
    }
    content = ft_listtostr(str);
    ft_putendl(content);
    ft_strdel(&content);
    ft_listdel(str, ft_memdel);
}

void        make_the_makefile(t_list *list, t_list *source)
{
    t_node  *node;
    t_node  *next;
    int     test;

    node = list->start;
    test = 0;
    while (node)
    {
        next = node->next;
        if (node->content && ft_strcchr((char *)node->content, "FILE =") != 0)
            test = 1;
        if (test == 2 && node->content && ft_strlen((char *)node->content) < 2)
            test = 3;
        if (test == 1)
            test = 2;
        else if (test == 2)
            ft_listdelnode(list, node, ft_memdel);
        else if (test == 3)
        {
            create(source);
            ft_putstr("\n");
            test = 0;
        }
        else
            ft_putendl((char *)node->content);
        node = next;
    }
}

void        make_header(t_list *source)
{
    char    *content;
    char    *content2;
    char    *line;
    int     num;
    int     size;
    t_list  *list;

    num = 0;
    list = ft_listcreate();
    content = ft_get_file("libft/Makefile");
    content2 = ft_strsub(content, 0, ft_strlen(content) - 1);
    line = ft_strgetline(content2, num++);
    ft_listadd(list, line);
    size = (int)ft_strlen(content2) - ft_strlen(line);
    while (line)
    {
        line = ft_strgetline(content2, num++);
        ft_listadd(list, line);
        if (line)
            size -= (int)ft_strlen(line);
        if (size <= 1)
            line = NULL;
    }
    make_the_makefile(list, source);
    ft_strdel(&content);
    ft_strdel(&content2);
    ft_listdel(list, ft_memdel);
}

int         main(int argc, char **argv)
{
    t_opt   *files;
    t_list  *list;
    char    *file;

    files = ft_optget(argc, argv);
    file = ft_optgetopt_next(files);
    list = ft_listcreate();
    while (file)
    {
        add_header(file, list);
        ft_strdel(&file);
        file = ft_optgetopt_next(files);
    }
    make_header(list);
    ft_listdel(list, ft_memdel);
    ft_optdel(files);
    return (1);
}
