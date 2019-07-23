# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 18:00:29 by jterrazz          #+#    #+#              #
#    Updated: 2019/07/23 16:16:12 by jterrazz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC_PATH = inc
LIB_PATH = libs
BUILD_PATH = obj
SRC_PATH = src

SOURCES += cmd/init.c cmd/start.c cmd/end.c
SOURCES += handle/archive.c handle/fat.c handle/binary.c handle/macho.c
SOURCES += parse/macho_segment.c parse/macho_symtab.c parse/macho.c
SOURCES += shared/ft_hexdump.c shared/print_mysyms.c shared/overflow.c
SOURCES += shared/ft_bswap.c shared/ft_bswap_helper.c shared/file.c

SOURCES1 = ft_nm.c $(SOURCES)
SOURCES2 = ft_otool.c $(SOURCES)

LIB_SOURCES = $(LIB_PATH)/libft/libft.a $(LIB_PATH)/ft_printf/libftprintf.a

OBJECTS1 = $(SOURCES1:%.c=$(BUILD_PATH)/%.o)
OBJECTS2 = $(SOURCES2:%.c=$(BUILD_PATH)/%.o)

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

all: $(NAME)

$(NAME): $(NAME1) $(NAME2)

$(NAME1): libs $(OBJECTS1)
	@$(CC) $(FLAGS_CC) -o $@ $(OBJECTS1) $(LIB_SOURCES)

$(NAME2): libs $(OBJECTS2)
	@$(CC) $(FLAGS_CC) -o $@ $(OBJECTS2) $(LIB_SOURCES)

libs:
	@make -s -C $(LIB_PATH)/ft_printf
	@make -s -C $(LIB_PATH)/libft

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
