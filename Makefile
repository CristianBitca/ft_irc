# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbitca <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/27 11:08:05 by cbitca            #+#    #+#              #
#    Updated: 2026/04/29 16:40:24 by cbitca           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP     = c++
CFLAGS  = -Wall -Wextra -Werror -std=c++98

NAME    =  ircserv

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS    = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS    = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re