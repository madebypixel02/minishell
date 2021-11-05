
#!/bin/bash

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
RESET="\033[0m"
CYAN="\033[36m"

exec_test()
{
	TEST1="$(echo $@ | ./minishell | grep -v "$USER@minishell*")"
	TEST2="$(echo $@ | /bin/bash)"

	if [ "$TEST1" = "$TEST2" ] ; then
		printf " $BOLDGREEN%s %s$RESET\n" "✓ " "$@"
	else
		printf " $BOLDRED%s %s$RESET\n" "✗ " "$@"
	fi
	if [ "$TEST1" != "$TEST2" ]; then
		echo
		printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		echo
	fi
}

read -p "Enter builtin: " builtin

# CD
if [ "$builtin" = "cd" ] || [ "$builtin" = "all" ] ; then
	exec_test 'cd .. \npwd'
	exec_test 'cd -- \npwd'
	exec_test 'cd \npwd'
	exec_test 'cd src/ \n pwd'
fi

#ECHO
if [ "$builtin" = "echo" ] || [ "$builtin" = "all" ] ; then
	exec_test 'echo $PATH'
	exec_test 'echo veamos como funcionan      los espacios'
	exec_test 'echo "veamos como funcionan      los espacios"'
	exec_test 'echo $agua' #agua = "hola como     estas"
	exec_test 'echo "$agua"'
	exec_test 'echo $PWD'
	exec_test 'echo '\$PWD''
	exec_test 'echo '   \$PWD   ''
	exec_test 'echo "   $PWD   "'
	exec_test 'echo "$PWD"'
	exec_test 'echo "'\$PWD'"'
	exec_test 'echo '"\$PWD"''
	exec_test 'echo ""$PWD""'
	exec_test 'echo $ PWD'
	exec_test 'echo hola "'\$PWD' sfsdf"'
	exec_test 'echo "   $PWD$PD EE"'
	exec_test 'echo    "'\$HOLA'$PD EE$"'
	exec_test 'echo "'\$PD'"'
	exec_test 'echo '\$P'"W'D'"'
	exec_test 'echo "$~"'
	exec_test '> hola echo hello'
	exec_test 'echo hola \"'adi \$PWD   os' $PWD\" 'ddj'abc >> \"hello\" < main.c'
fi

# EXIT
if [ "$builtin" = "exit" ] || [ "$builtin" = "all" ] ; then
	exec_test 'exit 42'
	exec_test 'exit 42 42'
	exec_test 'exit 424435325234562346234534134'
	exec_test 'exit -42'
	exec_test 'exit wrong'
	exec_test 'exit | echo no'
fi

# REDIRECTION
if [ "$builtin" = "redir" ] || [ "$builtin" = "all" ] ; then
	exec_test 'echo test > ls \ncat ls'
	exec_test 'echo test > ls >> ls >> ls \necho test >> ls \ncat ls'
	exec_test '> lol echo test lol \ncat lol'
	exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test \ncat test'
	exec_test 'cat < ls'
	exec_test 'cat < ls > ls'
	exec_test 'cat << hola \ngjkl \n$PWD \nhola'
	exec_test 'cat << 'hola' \ngjkl \n$PWD \nhola'
	exec_test 'cat << ho'la' \ngjkl \n$PWD \nhola'	
fi

# ENV EXPANSION
if [ "$builtin" = "env" ] || [ "$builtin" = "all" ] ; then
	exec_test 'env | grep agua' #agua = "hola como     estas"
	exec_test 'env | grep PWD'
	exec_test 'unset PWD \nenv | grep PWD'
	exec_test 'export ='
	exec_test 'export 1TEST= \n' $ENV_SHOW
	exec_test 'export TEST \n' $EXPORT_SHOW
	exec_test 'export ""="" \n' $ENV_SHOW
	exec_test 'export TES=T="" \n' $ENV_SHOW
	exec_test 'export TE+S=T="" \n' $ENV_SHOW
	exec_test 'export TEST=LOL \n echo $TEST \n' $ENV_SHOW
	exec_test 'export TEST=LOL \n echo $TEST$TEST$TEST=lol$TEST'
	exec_test 'export TEST=LOL \nexport TEST+=LOL \n echo $TEST \n' $ENV_SHOW
	exec_test $ENV_SHOW
	exec_test $EXPORT_SHOW
	exec_test 'export TEST="ls       -l     - a" \n echo $TEST \n $LS \n  $ENV_SHOW'
fi
