# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbueno-g <mbueno-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/07 16:58:45 by mbueno-g          #+#    #+#              #
#    Updated: 2021/11/24 13:26:59 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"
CYAN="\033[36m"
TOTAL=0
CORRECT=0

clear
exec_test()
{
	echo "$@" | ../minishell | grep -v "$USER@minishell*" > test1
	echo "$@" | /bin/bash > test2 
	test1="test1"
	test2="test2"
	if (cmp -s "$test1" "$test2") ; then
		printf $BOLDGREEN"All good! %s %s$RESET\n" "✓" "$@"
		CORRECT=$(( $CORRECT + 1 ))
	else
		printf $BOLDRED"\nError! %s %s$RESET\n\n" "✗" "$@"
		printf $BOLDRED"Your output : \n%.20s\n$(cat -e test1)\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $BOLDGREEN"Expected output : \n%.20s\n$(cat -e test2)\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		echo
	fi
	TOTAL=$(( $TOTAL + 1 ))
}

print_result()
{
	if [ $CORRECT -eq $TOTAL ]; then
		printf "\n\n$BOLDGREEN"
	else
		printf "\n\n$BOLDRED"
		printf "%s: %d/%d passed tests (%0.2f%%)\n$RESET" "$builtin" "$CORRECT" "$TOTAL" "$(($CORRECT * 100 / $TOTAL))"
	fi
}

read -p "Enter builtin: " builtin
while [ "$builtin" != "all" ] && [ "$builtin" != "cd" ] && [ "$builtin" != "echo" ] && [ "$builtin" != "exit" ] && [ "$builtin" != "redir" ] && [ "$builtin" != "export" ]
do
	printf "\nUse a valid builtin! (all, cd, echo exit, redir, export)\n"
	read -p "Enter builtin: " builtin
done

# CD
if [ "$builtin" = "cd" ] || [ "$builtin" = "all" ] ; then
	exec_test "cd .. \npwd"
	exec_test "cd \npwd"
	exec_test "cd src/ \n pwd"
	print_result
fi

#ECHO
if [ "$builtin" = "echo" ] || [ "$builtin" = "all" ] ; then
	exec_test "echo \$PATH"
	exec_test "echo checking if spaces      work properly"
	exec_test "echo \"checking if spaces       work properly\""
	exec_test "echo 'checking if spaces   work properly'"
	exec_test "echo \$water"
	exec_test "echo \"\$water\""
	exec_test "echo '\$water'"
	exec_test "echo \$PWD"
	exec_test "echo \$PWD"
	exec_test "echo    \$PWD   "
	exec_test "echo \"   \$PWD   \""
	exec_test "echo \"\$PWD\""
	exec_test "echo \"'\$PWD'\""
	exec_test "echo '\"\$PWD\"'"
	exec_test "echo \"\"\$PWD\"\""
	exec_test "echo \$ PWD"
	exec_test "echo hola \"'\$PWD' sfsdf\""
	exec_test "echo \"   \$PWD\$PD EE\""
	exec_test "echo    \"'\$HOLA'\$PD EE\$\""
	exec_test "echo \"'\$PD'\""
	exec_test "echo '\$P'\"W'D'\""
	exec_test "echo \"\$~\""
	exec_test "> hola echo hello"
	exec_test "echo hola \"'adi \$PWD   os' \$PWD\" 'ddj'abc >> \"hello\" < main.c"
	exec_test "echo $\"PWD\""
	exec_test "echo \$''"
	exec_test "echo \$\"\""
	exec_test "echo \$"
	exec_test "echo \"\"\$"
	exec_test "echo \$?"
	print_result
fi

# EXIT
if [ "$builtin" = "exit" ] || [ "$builtin" = "all" ] ; then
	exec_test "exit 42"
	exec_test "exit 42 42"
	exec_test "exit 424435325234562346234534134"
	exec_test "exit -42"
	exec_test "exit wrong"
	exec_test "exit | echo no"
	print_result
fi

# REDIRECTION
if [ "$builtin" = "redir" ] || [ "$builtin" = "all" ] ; then
	exec_test "echo test > ls \ncat ls"
	exec_test "echo test > ls >> ls >> ls \necho test >> ls \ncat ls"
	exec_test "> lol echo test lol \ncat lol"
	exec_test ">lol echo > test>lol>test>>lol>test mdr >lol test >test \ncat test"
	exec_test "cat < ls"
	exec_test "cat < ls > ls"
	exec_test "cat << hola \ngjkl \n\$PWD \nhola"
	exec_test "cat << 'hola' \ngjkl \n\$PWD \nhola"
	exec_test "cat << ho'la' \ngjkl \n\$PWD \nhola"
	print_result
fi

# ENV EXPANSION
if [ "$builtin" = "env" ] || [ "$builtin" = "all" ] ; then
	exec_test "env | grep water"
	exec_test "env | grep PWD"
	exec_test "unset PWD \nenv | grep PWD"
	exec_test "export ="
	exec_test "export 1TEST= \n \$ENV_SHOW"
	exec_test "export TEST \n \$EXPORT_SHOW"
	exec_test "export \"\"=\"\" \n \$ENV_SHOW"
	exec_test "export TES=T=\"\" \n \$ENV_SHOW"
	exec_test "export TE+S=T=\"\" \n \$ENV_SHOW"
	exec_test "export TEST=LOL \n echo \$TEST \n \$ENV_SHOW"
	exec_test "export TEST=LOL \n echo \$TEST\$TEST\$TEST=lol\$TEST"
	exec_test "export TEST=LOL \nexport TEST+=LOL \n echo $\TEST \n \$ENV_SHOW"
	exec_test "\$ENV_SHOW"
	exec_test "\$EXPORT_SHOW"
	exec_test "export TEST=\"ls       -l     - a\" \n echo \$TEST \n \$LS \n  \$ENV_SHOW"
	print_result
fi

if [ "$builtin" = "all" ]; then
	if [ $CORRECT -eq $TOTAL ]; then
		printf "\n\n$BOLDGREEN"
	else
		printf "\n\n$BOLDRED"
		printf "%TOTAL: %d/%d passed tests (%0.2f%%)\n$RESET" "$builtin" "$CORRECT" "$TOTAL" "$(($CORRECT * 100 / $TOTAL))"
	fi
fi

if [ $CORRECT -eq $TOTAL ]; then
	./starstruck
else
	./sad
fi

rm -f test1 test2
