# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbueno-g <mbueno-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/07 16:54:53 by mbueno-g          #+#    #+#              #
#    Updated: 2021/11/24 13:26:43 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"

#ECHO TESTS

read -p "Enter your command: " var

touch test1 test2

echo "$var" | ../minishell | grep -v "$USER@minishell"* > test1
echo "$var" | /bin/bash > test2

test1="test1"
test2="test2"

if (cmp -s "$test1" "$test2") ; then
	printf $BOLDGREEN"\nAll good! %s$RESET\n\n" "✓"
else
	printf $BOLDRED"\nError! %s$RESET\n" "✗"
	echo
	printf $BOLDRED"Your output : \n%.20s\n$(cat -e test1)\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	echo
fi

printf $BOLDGREEN"Expected output : \n%.20s\n$(cat -e test2)\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"

if (cmp -s "$test1" "$test2") ; then
	./starstruck
else
	printf "$BOLDRED\n----------------------------------------- DIFF -----------------------------------------\n\n"
	diff test1 test2
	printf $RESET
	./sad
fi

rm -f test1 test2
