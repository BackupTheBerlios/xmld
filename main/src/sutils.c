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
#include "xmlddef.h"
#include "sutils.h"
#define INTEGER_LENGTH 15
#define FLOAT_LENGTH 15

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

 char **ret=NULL; 
 
 do {
  ret=(char **) realloc(ret, ++num*sizeof(char *));
  ret[num-1]=NULL;
  
  if (ptr == NULL) {
   ret[num-2]=(char *) malloc((strlen(str) - ((lptr==NULL) ? 0 : (++lptr - str)) + 1)*sizeof(char));
   strcpy(ret[num-2], (lptr==NULL) ? str : lptr);
   break;
  }
  else {
   ret[num-2]=(char *) malloc((ptr - ((lptr==NULL) ? str : lptr + 1)  + 1)*sizeof(char));
   strncpy(ret[num-2], (lptr==NULL) ? str : lptr + 1, ptr - ((lptr==NULL) ? str : lptr + 1));
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
XMLDBool str_like(char *str, char *patern) {
 if (strchr(patern, '%') == NULL) {
  return !strcmp(str, patern);
 }

 char *str_ptr=str;
 char *pat_ptr=patern;
 char *str_tmp;

 while (1) {
  if (*pat_ptr != '%') {
   if (*str_ptr != *pat_ptr) {
    return XMLD_FALSE;
   }
  }
  else {
   if (*(pat_ptr + 1) == '\0') {
    return XMLD_TRUE;
   }
   else {
    str_ptr=strchr(str_ptr+1, *(pat_ptr+1));
    if (str_ptr == NULL) {
     return XMLD_FALSE;
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
   return XMLD_TRUE;
  }
  else if ((*pat_ptr == '\0' || *str_ptr == '\0') && *pat_ptr != *str_ptr) {
   return XMLD_FALSE;
  }
 } 
}

/* Checks for whether the value of str is between min and max */
XMLDBool str_between(char *str, char *min, char *max) {
 int val=atol(str);
 return (val >= atol(min) && val <= atol(max));
}

/*
 * Turns a given integer to its string representation.
 * i.e atoi inverse.
 */
char *itostr(int num, int len) {
 if (len == 0) {
  len = NUMERIC_LENGTH;
 }
 char *ret=(char *) malloc(len*sizeof(char));
 snprintf(ret, len, "%d", num);
 return ret;
}

/*
 * Turns a given float to its string representation
 * i.e atof inverse.
 */
char *ftostr(float num, int len) {
 if (len == 0) {
  len = FLOAT_LENGTH;
 }
 char *ret=(char *) malloc(len*sizeof(char));
 snprintf(ret, len, "%f", num);
 return ret;
}

/*
 * ltrim: removes white spaces at the beginning of a string.
 */
char *ltrim(char *str) {
 char *ret=str;
 while (*ret == ' ' || *ret == '\t') {
  ret++;
 }
 return ret;
}

/*
 * Replaces each occurence of a character in neddle in the haystack string
 * with the corresponding string in repl.
 */
char *mstrchr_replace(char *haystack, char *neddle, char **repl, int num) {
 char *tmp=(char *) malloc(sizeof(char));
 int tmp_len=1;
 tmp[0]='\0';
 char *curr=haystack;
 int i;
 
 while (*curr != '\0') {
  for (i = 0; i < num; i++) {
   if (*curr == neddle[i]) {
    tmp_len+=strlen(repl[i]);
    tmp = (char *) realloc(tmp, tmp_len * sizeof(char));
    strcat(tmp, repl[i]);
    break;
   }
  }
  if (i == num) {
   tmp=(char *) realloc(tmp, (++tmp_len) * sizeof(char));
   tmp[tmp_len-1]='\0';
   tmp[tmp_len-2]=*curr;
  }
  curr++;
 }
 return tmp; 
}
