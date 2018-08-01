# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-huu- <tle-huu-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/11 20:33:21 by tle-huu-          #+#    #+#              #
#    Updated: 2018/08/01 16:30:49 by tle-huu-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test

#CFLAGS += -Wall -Werror -Wextra
#CFLAGS += -fsanitize=address
INCLUDES = includes/

SRCS += srcs/bsemaphore.c
SRCS += srcs/jobqueue.c
SRCS += srcs/thread.c
SRCS += srcs/threadpool.c
SRCS += main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS) $(HEADERS)
	@printf "\r\033[K\033[1;33m[$(NAME)] \033[0;32mLinking...\033[0m\n"
	@gcc $(CFLAGS) -I$(INCLUDES) $(OBJS) -pthread -o $(NAME)
	@printf "\r\033[K\033[1;33m[$(NAME)] \033[0;32mDone !\033[0m\n"

%.o: %.c
	@printf "\r\033[K\033[1;33m[$(NAME)] \033[0;32mBuilding $<\033[0m\n"
	gcc $(CFLAGS) -I$(INCLUDES) -c $< -pthread -o $@

clean:
	@rm -f $(OBJS)
	@printf "\033[1;32m[$(NAME)] \033[1;31mCleaned .o!\033[0m\n"

fclean: clean
	@rm -f $(NAME)
	@printf "\033[1;32m[$(NAME)] \033[1;31mCleaned all!\033[0m\n"

re: fclean all
