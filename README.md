# minishell

*As beautiful as a shell 🐚*

### Table of Contents

* [Introduction](#introduction)
	* [What the Shell?](#what-the-shell)
* [Our Implementation of Minishell](#our-implementation-of-minishell)
	* [Lexer & Expander](#lexer-&-expander)
	* [Parser](#parser)
	* [Executor](#executor)
* [Extras](#extras)
* [Roadblocks](#roadblocks)
* [Installation](#installation)
* [Demos](#demos)
* [References](#references)
* [Summary](#summary)


## Introduction

This project is all about recreating your very own (mini)shell, taking bash as reference. This was our first group project, and I was honored to do it with [@mbueno-g](https://github.com/mbueno-g) :)


#### What the Shell?

As we just said, we are asked to implement our own shell, but what is a shell to begin with? If we think (of for example) Linux as a nut or a seashell, the kernel/seed is the core of the nut and has to be surrounded by a cover or shell. Likewise, the shell we are implementing works as a command interpreter communicating with the OS kernel in a secure way, and allows us to perform a number tasks from a command line, namely execute commands, create or delete files or directories, or read and write content of files, among (many) other thins.

## Our Implementation of Minishell

The general idea for this shell is reading a string of commands in a prompt using [readline](https://www.man7.org/linux/man-pages/man3/readline.3.html). Before anything, it is highly recommended to take a deep dive into the [bash manual](https://www.gnu.org/software/bash/manual/bash.html), as it goes over every detail we had to have in mind when doing this project. ``Minishell`` involves heavy parsing of the string read by ``readline``, thus it is crucial to divide the code of the project into different parts: the ``lexer``, the ``expander``, the ``parser``, and lastly the ``executor``.

* ``1.0: Basic stuff working``

![minishell](https://user-images.githubusercontent.com/40824677/141175675-41b940ba-7080-4f1f-add5-edaf79ed6b8d.gif)


* ``2.0: Pipes working, plus some exit statuses``

![minishell](https://user-images.githubusercontent.com/40824677/141684153-e2748818-8a01-4cf8-88a6-5ed2624e2ce6.gif)


``3.0: Mostly done, yay :)``

![Peek-2021-11-24-12-25](https://user-images.githubusercontent.com/40824677/143232233-4e385114-441f-4e3e-b3ba-477bc75454e1.gif)


Available in six different colors:

![Screenshot from 2021-11-24 12-59-57](https://user-images.githubusercontent.com/40824677/143234581-0ff3d00d-18af-43d6-be44-15eef9583c1d.png)
![Screenshot from 2021-11-24 13-01-09](https://user-images.githubusercontent.com/40824677/143234739-bed5e503-e4ab-4016-a5e3-82e29f75f11f.png)
![Screenshot from 2021-11-24 13-05-36](https://user-images.githubusercontent.com/40824677/143235316-82cb41a2-996b-480c-ac4b-237233162741.png)
![Screenshot from 2021-11-24 13-03-11](https://user-images.githubusercontent.com/40824677/143235017-292489d2-6695-4cc5-b333-5c13fb32eeb1.png)
![Screenshot from 2021-11-24 13-07-13](https://user-images.githubusercontent.com/40824677/143235567-d19bb199-a51f-4c9e-b251-a0f8b9599f75.png)
![Screenshot from 2021-11-24 13-08-12](https://user-images.githubusercontent.com/40824677/143235730-e2d7a8ff-d398-4ff0-9a60-b0929ad38152.png)

Note: ``red`` color is reserved for the ``root`` user

![Screenshot from 2021-11-24 13-29-43](https://user-images.githubusercontent.com/40824677/143238700-8878c4f3-4763-4c8f-976e-aae049c9ed57.png)

![Screenshot from 2021-11-24 13-32-04](https://user-images.githubusercontent.com/40824677/143238979-4b8688f7-18ad-4c95-a380-496e5fc2ab17.png)

