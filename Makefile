# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/22 13:38:18 by aperez-b          #+#    #+#              #
#    Updated: 2021/11/30 16:14:22 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color Aliases
DEFAULT = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

UNAME = $(shell uname -s)

# Properties for MacOS
ECHO = echo
CDEBUG = -fsanitize=address
CHECKER = tests/checker_Mac
ifeq ($(UNAME), Linux)
	#Properties for Linux
	ECHO = echo -e
	LEAKS = valgrind --leak-check=full --track-fds=yes --trace-children=yes -s -q 
endif

# Make variables
AR = ar rcs
CFLAGS = -Wall -Wextra -Werror -MD -g3 
RM = rm -f
CC = gcc
SRC_DIR = src
SRCB_DIR = srcb
OBJ_DIR = obj
OBJB_DIR = objb
LIBFT = libft/bin/libft.a
NAME = minishell

SRC = main.c builtins.c ft_strtrim_all.c exec.c			\
	  fill_node.c get_params.c ft_cmdtrim.c				\
	  expand.c heredoc.c error.c env.c custom_cmd.c		\
	  get_next_line.c get_next_line_utils.c prompt.c	\
	  ft_cmdsubsplit.c signal.c parse_args.c get_cmd.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

OBJ_LFT = $(addprefix $(OBJ_LFT_DIR)/, $(SRC_LFT:.c=.o))

all: $(NAME)

$(NAME): create_dirs compile_libft $(OBJ)
	@$(CC) -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include $(CFLAGS) $(CDEBUG) $(OBJ) $(LIBFT) -lreadline -o $@
	@$(ECHO) "$(GREEN)$(NAME) is up to date!$(DEFAULT)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(ECHO) "Compiling $(BLUE)$<$(DEFAULT)..."
	@$(CC) -I ~/.brew/opt/readline/include $(CFLAGS) $(CDEBUG) -c $< -o $@

compile_libft:
	@if [ ! -d "libft" ]; then \
		git clone https://github.com/madebypixel02/libft.git; \
	fi
	@make -C libft

create_dirs:
	@mkdir -p $(OBJ_DIR)

compare: all
	@cd tests && ./compare.sh && cd ..

test: all
	@cd tests && ./test.sh && cd ..

run: all
	@$(LEAKS)./$(NAME)

clean:
	@$(ECHO) "$(CYAN)Cleaning up object files in $(NAME)...$(DEFAULT)"
	@if [ -d "libft" ]; then \
		make clean -C libft/; \
	fi
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(ECHO) "$(CYAN)Removed $(NAME)$(DEFAULT)"
	@$(RM) $(NAME)

norminette:
	@if [ -d "libft" ]; then \
		make norminette -C libft/; \
	fi
	@$(ECHO) "$(CYAN)\nChecking norm for $(NAME)...$(DEFAULT)"
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR) inc/

re: fclean all
	@$(ECHO) "$(YELLOW)Cleaned and Rebuilt Everything for $(NAME)!$(DEFAULT)"

git:
	git add .
	git commit
	git push

-include $(OBJ_DIR)/*.d
-include $(OBJB_DIR)/*.d

.PHONY: all clean fclean bonus norminette create_dirs test git re
