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
#include <string.h>
#include "str_split.h"

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
   ret[num-2]=(char *) malloc(((strlen(str)+1) - ((lptr==NULL) ? 0 : (++lptr - str)) + 1)*sizeof(char));
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
