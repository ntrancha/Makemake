# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ntrancha <ntrancha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/08/18 20:36:26 by ntrancha          #+#    #+#             *#
#    Updated: 2015/08/18 20:49:33 by ntrancha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = dependenC

$(NAME):
	@rm -rf libft/
	@git clone https://github.com/ntrancha/libft.git
	@cd libft; make install; cd ..
	@if [ -f "libft/libft.a" ] ; \
	then \
		gcc dependenC.c libft/libft.a -Ilibft/includes/ -o $(NAME) ; \
		echo "Success !!!" ; \
	else \
		echo "Error !!!" ; \
	fi;
