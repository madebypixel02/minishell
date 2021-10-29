# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test1.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/28 19:34:45 by mbueno-g          #+#    #+#              #
#    Updated: 2021/10/29 11:17:24 by mbueno-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"

#ECHO TESTS

echo "cd --" | ./minishell
TEST1=$(echo "pwd" | ./minishell)
echo $TEST1

cd --
TEST2=$(pwd)
echo $TEST2

if ["$TEST1" == "$TEST2"]; then
	printf " BOLDGREEN%s" "✓ "
else
	printf " $BOLDRED%s" "✗ "
fi
