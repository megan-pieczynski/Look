/* Megan Pieczynski Final Project
 * June 3rd 2016 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define WORDFILE "/usr/share/dict/words"
#define LINEBUFSIZE 255

/* look error message */
void Usage() {
   fprintf(stderr, "usage: look [-df] [-t char] string [file]\n");
   exit(2);
}

/* function to search words ignoring non alphanumeric characters */
char *NextAlNum(char *str, char *end) {
   while ((!end || str <= end) && *str && *str != '\n' && !isalnum(*str))
      str++;
   return str;
}

/* compare function that returns 0 if a match is found, >0 if search is greater 
 * than line , and <0 if search is less than line */ 
int CheckMatch(char *line, char *search, char *end, int alnum, int ignorecase) {
   if (alnum) {
      search = NextAlNum(search, NULL);
      line = NextAlNum(line, end);
   }
   while (*search) {
      if (line > end || *line == '\n') {
         return -1; 
      }

      if (ignorecase && tolower(*line) != tolower(*search)) {
         return tolower(*line) - tolower(*search); 
      }

      if (!ignorecase && *line != *search) {
         return *line - *search;
      }

      line++;
      search++;
      if (alnum) {
         search = NextAlNum(search, NULL);
         line = NextAlNum(line, end);
      }

   }
   return 0;
}

/* function that iterates backwards to find first line with an
 * occurence of a match */
char *FirstOccurence(char *line, char *search, char *start, char *end) {
   char *temp;

   do {
      temp = line;
      if (line == start)
         break;
      while (line > start)
         if (--line > start && *(line - 1) == '\n')
            break;
   } while (!CheckMatch(line, search, end, 1, 1));

   return temp;
}

/* function to perform a binary search on the file */
char *BinarySearch(char *start, char *end, char *search) {
   char *middle = start + (end - start) / 2;
   int isMatch;

   if (start > end) 
      return NULL;

   while (middle > start && *(middle - 1) != '\n') {
      middle--;
   }

   isMatch = CheckMatch(middle, search, end, 1, 1);

   if (!isMatch) 
      return middle;

   else if (isMatch > 0)
      return BinarySearch(start, middle - 1, search); 

   else 
      return BinarySearch(middle + 1, end, search);
}

/* function that prints all occurences of a match starting with the first line */
int PrintMatches(char *fname, char *search, int alnum, int ignorecase) {
   int fp;
   int matchStatus = 1;
   struct stat fileStat;
   char *start;
   char *end;
   char *match;

   fp = open(fname, O_RDONLY);
   if (fp < 0) {
      fprintf(stderr, "look: ");
      perror(fname);
      exit(2);
   }

   fstat(fp, &fileStat);

   start = (char *)mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, fp, 0);
   end = start + fileStat.st_size - 1;

   match = BinarySearch(start, end, search);
   if (!match)
      return 1;
      
   match = FirstOccurence(match, search, start, end);

   while (match <= end && !CheckMatch(match, search, end, 1, 1)) {
      if ((alnum && ignorecase) ||
       !CheckMatch(match, search, end, alnum, ignorecase)) {
         matchStatus = 0;
         while (match <= end && *match != '\n')
            putchar(*match++);
         putchar('\n');
      }
      else {
         while (match <= end && *match != '\n') 
            match++;
      }
      match++;
   }

   if (close(fp)) {
      fprintf(stderr, "look: ");
      perror(fname);
      exit(2);
   }
 
   return matchStatus;
}

void Terminate(char *str, char termchar) {
   str = strchr(str, termchar);
   if (str) {
      str[1] = '\0';
   }
}

int main(int argc, char *argv[]) {
   int alnum = 0, ignorecase = 0, shouldTerminate = 0, matchStatus = 1;
   char termchar = 0, *search = NULL;
 
   while (*++argv && **argv == '-') {
      while (*++(*argv)) {
         switch (**argv) {
         case 'd':
            alnum = 1;
            break;
         case 'f':
            ignorecase = 1;
            break;
         case 't':
            shouldTerminate = 1;
            if ((*++argv == NULL))
               Usage();
            termchar = **argv;
            if ((*argv)[1]) {
               fprintf(stderr, "look: invalid termination character\n");
               exit(1);
            }
            break;
            
         default:
            fprintf(stderr, "look: invalid option -- '%c'\n", (**argv));
            Usage();
         }
      }
   }

   if (*argv == NULL) {
      Usage();
   }

   search = *argv++;

   if (shouldTerminate) {
      Terminate(search, termchar);
   } 

   if (*argv == NULL) {
      ignorecase = 1;
      alnum = 1;
      matchStatus = PrintMatches(WORDFILE, search, alnum, ignorecase);
   }
   else {
      while (*argv) {
         matchStatus &= PrintMatches(*argv++, search, alnum, ignorecase);
      }
   }

   return matchStatus;
}
