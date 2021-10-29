
#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"

#ECHO TESTS

TEST1="$(echo "cd ..\npwd" | ./minishell | grep -v "mbueno-g@minishell*")"
echo $TEST1
TEST2="$(echo "cd ..\npwd" | /bin/bash)"
echo $TEST2

if [[ "$TEST1" == "$TEST2" ]] ; then
	printf " $BOLDGREEN%s$RESET\n" "✓ "
else
	printf " $BOLDRED%s$RESET\n" "✗ "
fi
