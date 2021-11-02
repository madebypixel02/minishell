
#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"

#ECHO TESTS

read -p "Enter your command: " var

TEST1="$(echo "$var" | ./minishell | grep -v "$USER@minishell*")"
TEST2="$(echo "$var" | /bin/bash)"
if [ "$TEST1" != "$TEST2" ]; then
	echo
	printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	echo
fi
