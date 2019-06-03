# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 18:00:29 by jterrazz          #+#    #+#              #
#    Updated: 2019/05/30 14:00:38 by jterrazz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# FILES             														   #
# **************************************************************************** #

PATH_INC = inc
PATH_LIB = libs
PATH_OBJ = obj
PATH_SRC = src

SOURCES += ft_nm/ft_nm.c

OBJECTS = $(SOURCES:%.c=$(PATH_OBJ)/%.o)

# **************************************************************************** #
# VARIABLES         														   #
# **************************************************************************** #

NAME = ft_nm

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror
FLAGS_LCC = -Wall -Wextra -Werror

LIBS = libs/ft_printf/libftprintf.a # libs/libft/libft.a

# **************************************************************************** #
# COMMANDS  		    													   #
# **************************************************************************** #

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJECTS)
	@make -s -C lib/ft_printf
	@echo "Make ft_printf \033[33mok\033[0m"
	# @make -s -C lib/libft
	# @echo "Make libft \033[33mok\033[0m"
	$(CC) $(FLAGS_LCC) -o $@ $^ $(LIBS)
	@echo "Compilation successful"

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS_CC) -c -o $@ $< -I $(PATH_INC)

clean:
	@make clean -C lib/ft_printf
	# @make clean -C lib/libft
	@rm -rf $(PATH_OBJ)
	@echo "Clean \033[33mok\033[0m"

fclean: clean
	@make fclean -C lib/ft_printf
	# @make fclean -C lib/libft
	@rm -f $(NAME) $(LIB_NAME)
	@echo "Fclean \033[33mok\033[0m"

re: fclean $(NAME)
