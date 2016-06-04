README - look.c

What is it?
------------------------------------------
look is a command line utility that prints all lines to standard out
that begin with the given string passed as a commandline argument 

look accepts a file as the last commandline argument and if none 
is provided look uses the file /usr/share/dict/words 

If a file is not specified look compares only alphanumeric characters 
and ignores the case of alphabetic characters

How To Compile "look.c":
------------------------------------------
Compile look using the Makefile provided
   make look

The Makefile compiles look with -ansi -pedantic -Wall -Werror -O3 -o Look

How To Run "look.c":
------------------------------------------
Run look using the following format 
   ./Look [-dfa] [-t termchar] string [file]

Files Included
------------------------------------------
   look.c
   Makefile

Flag Options 
------------------------------------------
   -d       Only alphanumeric characters are compared.
            This flag is turned on if no file is specified
            Ex) ./Look -d hops

            hops
            hop-sack
            hopsack
            hop-sacking
            hopsacking
            hopsacks
            ...

   -f       Ignores the case of alphabetic characters. 
            This flag is turned on if no file is specified
            Ex) ./Look -f piz

            ...
            pizazzy
            pize
            Pizor
            pizz
            pizz.
            ...

   -t       Use this flag along with a specifed termination character.
            The -t flag will print lines with the matching starting 
            string that goes until and including the termination character
            Ex) ./Look -t a pizzas

            pizza
            pizzas
            pizzazz
            pizzazzes

Error Codes
-------------------------------------------
look returns >1 and exits if an error occures

Return Values
-------------------------------------------
look returns 0 if at least one line is found and 1 if no lines are found

Author
-------------------------------------------
Megan Pieczynski June 3rd 2016
Cal Poly San Luis Obispo Computer Science

GitHub Link
-------------------------------------------
