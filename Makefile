# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 18:00:29 by jterrazz          #+#    #+#              #
#    Updated: 2019/06/10 00:40:39 by jterrazz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC_PATH = inc
LIB_PATH = libs
BUILD_PATH = obj
SRC_PATH = src

SOURCES += ft_nm.c
SOURCES += cmd/env.c cmd/process_file.c
SOURCES += handle/file.c
SOURCES += parse/segment.c parse/symtab.c

LIB_SOURCES = $(LIB_PATH)/libft/libft.a $(LIB_PATH)/ft_printf/libftprintf.a

OBJECTS = $(SOURCES:%.c=$(BUILD_PATH)/%.o)

# **************************************************************************** #
# VARIABLES         														   #
# **************************************************************************** #

NAME = ft_nm_otool
NAME1 = ft_nm
NAME2 = ft_otool

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror

# **************************************************************************** #
# COMMANDS  		    													   #
# **************************************************************************** #

.PHONY: all libs clean fclean re

all: libs $(NAME1) $(NAME2)

$(NAME1): $(OBJECTS)
	$(CC) $(FLAGS_CC) -o $@ $^ $(LIB_SOURCES)
	@echo "$(NAME1): Compilation successful"

$(NAME2): $(OBJECTS)
	$(CC) $(FLAGS_CC) -o $@ $^ $(LIB_SOURCES)
	@echo "$(NAME2): Compilation successful"

libs:
	@make -s -C $(LIB_PATH)/ft_printf
	@echo "Make ft_printf \033[33mok\033[0m"
	@make -s -C $(LIB_PATH)/libft
	@echo "Make libft \033[33mok\033[0m"

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS_CC) -c -o $@ $< -I $(INC_PATH)

clean:
	@make clean -C $(LIB_PATH)/ft_printf
	@make clean -C $(LIB_PATH)/libft
	@rm -rf $(BUILD_PATH)
	@echo "Clean \033[33mok\033[0m"

fclean: clean
	@make fclean -C $(LIB_PATH)/ft_printf
	@make fclean -C $(LIB_PATH)/libft
	@rm -f $(NAME1) $(NAME2) $(LIB_NAME)
	@echo "Fclean \033[33mok\033[0m"

re: fclean $(NAME)
