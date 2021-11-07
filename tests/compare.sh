# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbueno-g <mbueno-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/07 16:54:53 by mbueno-g          #+#    #+#              #
#    Updated: 2021/11/07 21:28:25 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"

#ECHO TESTS

read -p "Enter your command: " var

TEST1=$(echo "$var" | ../minishell | grep -v "$USER@minishell*" 2>&1)
TEST2=$(echo "$var" | /bin/bash 2>&1)

if [[ "$TEST1" != "$TEST2" && ("$TEST1" != *"command not"* || "$TEST2" != *"command not"*) && ("$TEST1" != *"No such"* || "$TEST2" != *"No such"*) ]]; then
	printf $BOLDRED"\nError! %s$RESET\n" "✗"
	echo
	printf $BOLDRED"Your output : \n%.20s\n|$TEST1|\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	echo
else
	printf $BOLDGREEN"\nAll good! %s$RESET\n\n" "✓"
fi

printf $BOLDGREEN"Expected output : \n%.20s\n|$TEST2|\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
if [[ "$TEST1" == "$TEST2" || ("$TEST1" == *"command not"* && "$TEST2" == *"command not"*) || ("$TEST1" == *"No such"* && "$TEST2" == *"No such"*) ]]; then
	./starstruck.sh
else
	./sad.sh
fi
