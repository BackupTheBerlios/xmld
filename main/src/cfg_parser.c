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
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include "xmld_list.h"
#include "xmld_directive.h"
#include "cfg_parser.h"

/*
 * : Parses the configuration file and makes
 * configuration directives be available for 
 * retrieval. (this function is a small lexer
 * which uses cfg_parser_parse_token as its
 * parser)
 * returns: whether successful.
 */
short cfg_parser_parse() {
 errno=0;
 FILE *conf=fopen("xmld.conf", "r");

 if (errno != 0) {
  return 0;
 }
 
 char buf;
 char *curr_data=(char *) malloc(sizeof(char));
 *curr_data='\0';
 int data_len=1;
 short mode=0; /* 0  = looking for type
                * 1  = looking for directive
	        * 2  = looking for equal sign
		* 3  = looking for value
		* 4  = looking for end-of-line
	        */
 short ign_ws=1; /* 0 = don't ignore whitespace
	          * 1 = ignore whitespace
	          */
 while (!feof(conf)) {
  fread((void *) &buf, sizeof(char), 1, conf);
  if (buf == '#') {
   mode=4;
   ign_ws=1;
   continue;
  }
  else {
   if (ign_ws == 1 || (buf == ' ' || buf == '\t')) {
    continue;
   }
   else {
    if (mode == 0) {
     if (buf != ' ' && buf != '\t') {
      ign_ws=0;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-1]=buf;
      curr_data[data_len]='\0';
     }
     else {
      cfg_parser_parse_token(curr_data, 0);
      mode=1;
      ign_ws=1;      
     }
    }
    else if (mode == 1) {
     if (buf != ' ' && buf != '\t' && buf != '=') {
      ign_ws=0;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-1]=buf;
      curr_data[data_len]='\0';
     }
     else {
      cfg_parser_parse_token(curr_data, 1);
      ign_ws=1;
      mode=2;      
     }
    }
    else if (mode == 2) {
     if (buf == '=') {
      mode=3;
     }
    }
    else if (mode == 3) {
     else if (buf == ';') {
      cfg_parser_parse_token(curr_data, 3);
      ign_ws=1;
      mode=0;
     }
     else {
      ign_ws=0;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-1]=buf;
      curr_data[data_len]='\0';
     }
    }
    else if (mode == 4) {
     if (buf == '\n') {	     
      mode=0;
     }
    }
   }
  }
 
 }
}

/*
 * : A small parser to parse tokens extracted by cfg_parser_parse.
 */
void cfg_parser_parse_token(char *token, short mode) {
}

/*
 * : Frees up resources used by the parse tree
 * created by cfg_parser_parse, and removes
 * configuration directives from memory.
 * returns: whether successful.
 */
short cfg_parser_clean() {
 return 1;
}
