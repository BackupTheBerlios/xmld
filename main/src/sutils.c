/*                                                                *
 * -------------------------------------------------------------- *
 * The OpenXMLD                                                   *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sutils.h"
#define NUMERIC_LENGTH 10

/*
 * : Returns an array of strings extracted
 * from the given string by breaking it into
 * pieces separated by the given delimeter.
 * str: The string from which to extract tokens.
 * delim: The delimeter to use in breaking the given
 * string.
 * returns: An array of the extracted tokens.
 */ 

/*
 * Not important FIXME:
 * handle delim being the first or the last
 * character in str. (this will not occur
 * in config!)
 */ 
char **str_split(char *str, char delim) {
 char *ptr;
 char *lptr=NULL;
 int num=1;
 ptr=strchr(str, (int) delim);

 char **ret; 
 
 do {
  ret=(char **) realloc(ret, ++num*sizeof(char *));
  ret[num-1]=NULL;
  
  if (ptr == NULL) {
   ret[num-2]=(char *) malloc((strlen(str) - ((lptr==NULL) ? 0 : (++lptr - str)) + 1)*sizeof(char));
   strcpy(ret[num-2], (lptr==NULL) ? str : lptr);
   break;
  }
  else {
   ret[num-2]=(char *) malloc((ptr - ((lptr==NULL) ? str : lptr)  + 1)*sizeof(char));
   strncpy(ret[num-2], (lptr==NULL) ? str : lptr, ptr - ((lptr==NULL) ? str : lptr));
   ret[num-2][ptr - ((lptr==NULL) ? str : lptr)]='\0';
  } 
  
  lptr=ptr;
  ptr=strchr(++ptr, (int) delim);
 }
 while (1);
 
 return ret;
}

/* Prepends a string to another */
char *str_prepend(char *dest, char *pre) {
 char *ret=(char *) malloc((strlen(dest)+strlen(pre)+1)*sizeof(char));
 strcpy(ret, pre);
 strcat(ret, dest);
 return ret;
}

/* checks whether str is LIKE patern (SQL) */
short str_like(char *str, char *patern) {
 if (strchr(patern, '%') == NULL) {
  return !strcmp(str, patern);
 }

 char *str_ptr=str;
 char *pat_ptr=patern;
 char *str_tmp;

 while (1) {
  if (*pat_ptr != '%') {
   if (*str_ptr != *pat_ptr) {
    return 0;
   }
  }
  else {
   if (*(pat_ptr + 1) == '\0') {
    return 1;
   }
   else {
    str_ptr=strchr(str_ptr+1, *(pat_ptr+1));
    if (str_ptr == NULL) {
     return 0;
    }
    else {
     while (1) {
      str_tmp=strchr(str_ptr+1, *(pat_ptr+1));
      if (str_tmp == NULL) {
       break;
      }
      else {
       str_ptr=str_tmp;
      } 
     } 
     str_ptr--;
    }
   }
  }
  pat_ptr++;
  str_ptr++;
  
  if (*pat_ptr == '\0' && *str_ptr == '\0') {
   return 1;
  }
  else if ((*pat_ptr == '\0' || *str_ptr == '\0') && *pat_ptr != *str_ptr) {
   return 0;
  }
 } 
}

/* Checks for whether the value of str is between min and max */
short str_between(char *str, char *min, char *max) {
 int val=atol(str);
 return (val >= atol(min) && val <= atol(max));
}

/*
 * Turns a given integer to its string representation.
 * i.e atoi inverse.
 * FIXME: make it snprintf-independent.
 */
char *ltoa(long num) {
 char *ret=(char *) malloc(NUMERIC_LENGTH*sizeof(char));
 int conv=snprintf(ret, NUMERIC_LENGTH, "%li", num);
 if (conv > 0) {
  ret=(char *) realloc(ret, (NUMERIC_LENGTH+conv)*sizeof(char));
  snprintf(ret, NUMERIC_LENGTH+conv, "%li", num);
 }
 else if (strlen(ret)+1 < NUMERIC_LENGTH) {
  ret=(char *) realloc(ret, (strlen(ret)+1)*sizeof(char));
 }
 return ret;
}
