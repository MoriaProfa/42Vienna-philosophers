# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: astehl <astehl@student.42vienna.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/05 12:19:16 by astehl            #+#    #+#              #
#    Updated: 2025/07/17 12:50:59 by astehl           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC =  cc
CFLAGS = -Wall -Werror -Wextra -pthread -MMD -MP
FSAN_FLAGS = -fsanitize=thread -g
RM = rm -f

NAME = philo

CFILES = philo.c utils.c handle_args.c init.c flags.c routine.c eat.c end.c
OFILES = $(CFILES:.c=.o)
DFILES = $(CFILES:.c=.d)

all: $(NAME)
-include $(DFILES)

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o $(NAME)

clean:
	@$(RM) $(OFILES) $(DFILES)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

fsan: CFLAGS += $(FSAN_FLAGS)
fsan: re

.PHONY: all clean fclean re fsan
