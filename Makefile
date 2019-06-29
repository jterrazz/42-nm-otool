# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 18:00:29 by jterrazz          #+#    #+#              #
#    Updated: 2019/06/16 23:45:28 by jterrazz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Progress bar in makefile + replace in global libs
INC_PATH = inc
LIB_PATH = libs
BUILD_PATH = obj
SRC_PATH = src

SOURCES += cmd/env.c cmd/process_file.c
SOURCES += handle/file.c handle/archive.c handle/fat.c
SOURCES += parse/mach_segment.c parse/mach_symtab.c parse/mach.c
SOURCES += print/ft_hexdump.c print/mysyms.c
SOURCES += utils/ft_bswap.c utils/ft_bswap_helper.c

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
