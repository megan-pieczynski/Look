C=gcc
CFLAGS = -Wall -Werror -pedantic -ansi -O3

look: look.c
   gcc $(CFLAGS) look.c -o Look
   
