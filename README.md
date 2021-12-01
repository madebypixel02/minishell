# minishell

*As beautiful as a shell 🐚*

### Table of Contents

* [Introduction](#introduction)
	* [What the Shell?](#what-the-shell)
* [Our Implementation of Minishell](#our-implementation-of-minishell)
	* [Lexer and Expander](#lexer-and-expander)
	* [Parser](#parser)
	* [Executor](#executor)
	* [Mind Map](#mind-map)
	* [Global Variable](#global-variable)
* [Builtins](#builtins)
* [Prompt](#prompt)
* [Extras](#extras)
	* [User Color](#user-color)
	* [Process ID](#process-id)
	* [Running without Environment](#running-without-environment)
* [Installation](#installation)
	* [Basic Usage](#basic-usage)
	* [Demos](#demos)
* [References](#references)
* [Summary](#summary)


## Introduction

This project is all about recreating your very own (mini)shell, taking bash (Bourne Again SHell) as reference. This was our first group project, and I was honored to do it with [@madebypixel02](https://github.com/madebypixel02) :)


#### What the Shell?

As we just said, we are asked to implement our own shell, but what is a shell to begin with? If we think of (for example) Linux as a nut or a seashell, the kernel/seed is the core of the nut and has to be surrounded by a cover or shell. Likewise, the shell we are implementing works as a command interpreter communicating with the OS kernel in a secure way, and allows us to perform a number tasks from a command line, namely execute commands, create or delete files or directories, or read and write content of files, among (many) other things

## Our Implementation of Minishell

The general idea for this shell is reading a string of commands in a prompt using [readline](https://www.man7.org/linux/man-pages/man3/readline.3.html). Before anything, it is highly recommended to take a deep dive into the [bash manual](https://www.gnu.org/software/bash/manual/bash.html), as it goes over every detail we had to have in mind when doing this project. ``Minishell`` involves heavy parsing of the string read by ``readline``, thus it is crucial to divide the code of the project into different parts: the ``lexer``, the ``expander``, the ``parser``, and lastly the ``executor``


### Lexer and Expander

This first part covers the part of our code in charge of expanding environment variables with ``$`` followed by characters, as well as the expansion of ``~`` to the user's home directory. Here we also split the input string into small chunks or tokens to better handle pipes, redirections, and expansions.

After reading from the ``stdin`` we use a function we named ``cmdtrim`` which separates the string taking spaces and quotes into account. For example:

```
string: echo "hello      there" how are 'you 'doing? $USER |wc -l >outfile
output: {echo, "hello      there", how, are, 'you 'doing?, $USER, |wc, -l, >outfile, NULL}
```
Then, we apply the expander functions on top of every substring of the original string, resulting in something similar to this:

```
output: {echo, "hello      there", how, are, 'you 'doing?, pixel, |wc, -l, >outfile, NULL}
Note: if a variable is not found, the $var part of the string will be replaced by an empty string
```

Lastly, we have another split function called ``cmdsubsplit`` which separates with ``<``, ``|``, or ``>``, but only if those chars are outside of quotes:

```
output: {echo, "hello      there", how, are, 'you 'doing?, pixel, |, wc, -l, >, outfile, NULL}
```

### Parser

The parser is in charge of storing the tokenized string and save it in a useful manner for the executor to use later. Our data structure is managed as follows:

```C
typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	int	e_status;
}		t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	pid_t	pid;
	int	infile;
	int	outfile;
}		t_mini;
```
Here is a short summary of what every variable is used for

| Parameter | Description |
| :-------: | :---------: |
| ``cmds`` | Linked list containing a ``t_mini`` node with all commands separated by pipes |
| ``full_cmd`` | Equivalent of the typical ``argv``, containing the command name and its parameters when needed |
| ``full_path`` | If not a builtin, first available path for the executable denoted by ``argv[0]`` from the ``PATH`` variable |
| ``pid`` | Process id of a child that runs a command |
| ``infile`` | Which file descriptor to read from when running a command (defaults to ``stdin``) |
| ``outfile`` | Which file descriptor to write to when running a command (defaults to ``stdout``) |
| ``envp`` | Up-to-date array containing keys and values for the shell environment |
| ``e_status`` | Exit status of the most-recently-executed command |


After running our lexer and expander, we have a two-dimensional array. Following the previous example, it was the following:

```
{echo, "hello      there", how, are, 'you 'doing?, pixel, |, wc, -l, >, outfile, NULL}
```

Now, our parser starts building the linked list of commands (``t_list *cmds``), which is filled in the following way:

1. Iterate over the two-dimensional array
2. Whenever a redirection is found, check the type of redirection and retrieve a file descriptor containing the info we need as the infile
3. Check that the file descriptor that has been opened is valid (!= -1) and continue
4. If a pipe is found, add a new node to the list of commands
5. In all other cases add whatever words are found to the argument list (``argv``) we call ``full_cmd``

Here's how the variables will look like according to the example we used before:

```
cmds:
	cmd 1:
		infile: 0 (default)
		outfile: 1 (redirected to pipe)
		full_path: NULL (because echo is a builtin)
		full_cmd: {echo, hello there, how, are, you doing?, pixel, NULL}
		pid: (output of getpid())
	cmd 2:
		infile: 0 (contains output of previous command)
		outfile: 3 (fd corresponding to the open file 'outfile')
		full_path: /bin/wc
		full_cmd: {wc, -l, NULL}
		pid: (output of getpid())
envp: (envp from main)
e_status: 0 (if last command exits normally)

```

### Executor

With all our data properly on our structs, the ``executer`` has all the necessary information to execute commands. For this part we use separate processess to execute either our builtins or other commands inside child processes that redirect ``stdin`` and ``stdout`` in the same way we did with our previous [pipex](https://github.com/madebypixel02/pipex) project. If we are given a full path (e.g. ``/bin/ls``) then we do not need to look for the full path of the command and can execute directly with [execve](https://www.man7.org/linux/man-pages/man2/execve.2.html). If we are given a relative path then we use the ``PATH`` environment variable to determine the ``full_path`` of a command. After all commands have started running, we retrieve the exit status of the most recently executed command with the help of [waitpid](https://linux.die.net/man/2/waitpid)

Once all commands have finished running the allocated memory is freed and a new prompt appears to read the next command

### Mind Map

Here is a handy mindmap of our code structure to help you understant everything we mentioned previously

![Concept Map - Frame 5](https://user-images.githubusercontent.com/71781441/144017004-aa68e8d7-5da7-4ece-afc6-b8ab100113df.jpg)
![Concept Map - Frame 4](https://user-images.githubusercontent.com/71781441/144017016-ef2bb606-c301-42c6-88f1-8ed4339d22cd.jpg)

### Global Variable

For this project we could use one global variable. At first it seemed we were never going to need one, but later it became obvious that it is required. Specifically, it has to do with signals. When you use [signal](https://www.man7.org/linux/man-pages/man7/signal.7.html) to capture ``SIGINT`` (from ``Ctrl-C``) and ``SIGQUIT`` (from ``Ctrl-\``) signals, we have to change the error status, and the ``signal`` function has no obvious way of retrieving the updated exit status that shoud change when either of these signals are captured. To work this around, we retrieve two pairs of open file descriptors, send the new exit status through the write end of one of the pipes, and close the other pipe ends.


## Builtins

We were asked to implement some basic builtins with the help of some functions, here is a brief overview of them:

| Builtin | Description | Options | Parameters | Helpful Functions |
| :-----: | :---------: | :-----: | :--------: | :---------------: |
| ``echo`` | Prints arguments separated with a space followed by a new line | ``-n`` | :heavy_check_mark: | [write](https://man7.org/linux/man-pages/man2/write.2.html) |
| ``cd`` | Changes current working directory, updating ``PWD`` and ``OLDPWD`` | :x: | :heavy_check_mark: | [chdir](https://man7.org/linux/man-pages/man2/chdir.2.html) |
|  ``pwd``| Prints current working directory | :x: | :x: | [getcwd](https://www.man7.org/linux/man-pages/man3/getcwd.3.html) |
| ``env`` | Prints environment | :x: | :x: | [write](https://man7.org/linux/man-pages/man2/write.2.html) |
| ``export`` | Adds/replaces variable in environment | :x: | :heavy_check_mark: | :x: |
| ``unset`` | Removes variable from environment | :x: | :heavy_check_mark: | :x: |

## Prompt

As mentioned previously, we use ``readline`` to read the string containing the shell commands. To make it more interactive, ``readline`` receives a string to be used as a prompt. We have heavily tweaked the looks of it to be nice to use. The prompt is structured as follows:

```
$USER@minishell $PWD $
```

Some remarks:

* If there is any problem retrieving the user, it will be replaced with ``guest``
* The ``PWD`` is colored blue and dynamically replaces the ``HOME`` variable with ``~`` when the variable is set. See below for more details
* The ``$`` in the end is printed blue or red depending on the exit status in the struct

![Screenshot from 2021-11-24 13-29-43](https://user-images.githubusercontent.com/40824677/143238700-8878c4f3-4763-4c8f-976e-aae049c9ed57.png)
![Screenshot from 2021-11-24 18-30-37](https://user-images.githubusercontent.com/40824677/143287061-7b87efc3-d5ea-4d65-b2f0-87fe5e96ba17.png)


## Extras

These are a few neat extras that were not explicitly mentioned on the subject of the project but we thought would make the whole experience nicer

### User Color

The ``$USER@minishell`` part of the prompt is available in six different colors (based on the first char of the user's username):

![Screenshot from 2021-11-24 12-59-57](https://user-images.githubusercontent.com/40824677/143234581-0ff3d00d-18af-43d6-be44-15eef9583c1d.png)
![Screenshot from 2021-11-24 13-01-09](https://user-images.githubusercontent.com/40824677/143234739-bed5e503-e4ab-4016-a5e3-82e29f75f11f.png)
![Screenshot from 2021-11-24 13-05-36](https://user-images.githubusercontent.com/40824677/143235316-82cb41a2-996b-480c-ac4b-237233162741.png)
![Screenshot from 2021-11-24 13-03-11](https://user-images.githubusercontent.com/40824677/143235017-292489d2-6695-4cc5-b333-5c13fb32eeb1.png)
![Screenshot from 2021-11-24 13-07-13](https://user-images.githubusercontent.com/40824677/143235567-d19bb199-a51f-4c9e-b251-a0f8b9599f75.png)
![Screenshot from 2021-11-24 13-08-12](https://user-images.githubusercontent.com/40824677/143235730-e2d7a8ff-d398-4ff0-9a60-b0929ad38152.png)

Note: ``red`` color is reserved for the ``root`` user

### Process ID

We were told to only expand variables of the form ``$ + alphanumeric chars``. We implemented expansion of ``$$``, which expands to the program's process id (Uses forbidden function [getpid](https://man7.org/linux/man-pages/man2/getpid.2.html))

![Screenshot from 2021-11-24 18-33-06](https://user-images.githubusercontent.com/40824677/143287427-778538d5-8392-4739-994e-3382f15d803d.png)

### Running without Environment

When running new instances of minishell or minishell withouth environment (``env -i ./minishell``), some environment variables need to be updated manualy, namely the shell level (``SHLVL``) or the ``_`` variable

Here's the env when minishell is launched without an environment:

![Screenshot from 2021-11-24 13-32-04](https://user-images.githubusercontent.com/40824677/143238979-4b8688f7-18ad-4c95-a380-496e5fc2ab17.png)


## Installation

* Prerequisites

Make sure you have these packages installed:

```
gcc make python-norminette readline (valgrind on Linux)

Note for MacOS: to install a recent version of readline, you need to use homebrew: brew install readline
```

* Cloning the Repositories

```shell
git clone https://github.com/madebypixel02/minishell.git
cd minishell
make
```

### Basic Usage

As per the norm, this project compiles an executable called ``minishell``, and it is compiled using the ``.c`` files inside the ``src`` folder

This project includes some nice testers which may help users test their minishell project. There are two variants:

* ``make compare`` Enter a custom command of your choice and check if bash returns the same output as your minishell
* ``make test`` Enter a builtin to check a set of tests with and get a summary of your failed and passed tests

#### Demos

As we developed the project, we recorded some demos of how the project looked. Here is an overview of the most relevant "releases" we made:

* ``v1.0`` Basic stuff working, no pipes or exit status redirection

![minishell](https://user-images.githubusercontent.com/40824677/141175675-41b940ba-7080-4f1f-add5-edaf79ed6b8d.gif)


* ``v2.0`` Pipes working, plus some exit statuses

![minishell](https://user-images.githubusercontent.com/40824677/141684153-e2748818-8a01-4cf8-88a6-5ed2624e2ce6.gif)


* ``v3.0`` Heavily cleaned code, misc fixes, use readline inside a child process, yay :)

![minishell](https://user-images.githubusercontent.com/40824677/143232233-4e385114-441f-4e3e-b3ba-477bc75454e1.gif)


## References

* [minishell - 42 Docs](https://harm-smits.github.io/42docs/projects/minishell)
* [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
* [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)


## Summary

This was our biggest project yet, and it sure was challenging. Co-developing can be tricky, especially the first time you do it on GitHub. We had fun in the process though :)

November 24th, 2021
