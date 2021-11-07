
#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"

#ECHO TESTS

read -p "Enter your command: " var

TEST1=$(echo "$var" | ./minishell | grep -v "$USER@minishell*" 2>&1)
TEST2=$(echo "$var" | /bin/bash 2>&1)
if [ "$TEST1" != "$TEST2" ]; then
	printf "\nError! $BOLDRED%s %s$RESET\n" "✗" "$@"
	echo
	printf $BOLDRED"Your output : \n%.20s\n$BOLDRED|$TEST1|\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGReEN|$TEST2|\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	echo
else
	printf "All good! $BOLDGREEN%s %s$RESET\n" "✓" "$@"
fi
