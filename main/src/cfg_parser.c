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
#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include "xmlddef.h"
#include "sutils.h"
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
XMLDStatus cfg_parser_parse() {
 errno=0;
 FILE *conf=fopen("xmld.conf", "r");

 if (errno != 0) {
  return XMLD_FAILURE;
 }
 
 char buf;
 char *curr_data=NULL;
 int data_len=1;
 int mode=XMLD_CFG_MODE_TYPE;
 XMLDBool ign_ws=XMLD_TRUE; /* whether to ignore whitespace */
 
 while (1) {
  buf = (char) fgetc(conf);
  if (buf == EOF) {
   break;
  }
  if (buf == '#' && mode == XMLD_CFG_MODE_TYPE) {
   mode=XMLD_CFG_MODE_END;
   ign_ws=XMLD_FALSE;
   continue;
  }
  else {
   if (ign_ws && (buf == ' ' || buf == '\t' || buf == '\n' || buf == '\r')) {
    continue;
   }
   else {
    if (mode == XMLD_CFG_MODE_TYPE) {
     if (buf != ' ' && buf != '\t') {
      ign_ws=XMLD_FALSE;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-2]=buf;
      curr_data[data_len-1]='\0';
     }
     else {
      cfg_parser_parse_token(curr_data, mode);
      free(curr_data);
      curr_data=NULL;
      data_len=1;
      mode=XMLD_CFG_MODE_DIR;
      ign_ws=XMLD_TRUE;
     }
    }
    else if (mode == 1) {
     if (buf != ' ' && buf != '\t' && buf != '=') {
      ign_ws=XMLD_FALSE;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-2]=buf;
      curr_data[data_len-1]='\0';
     }
     else {
      if (buf == '=') {
       fseek(conf, -1, SEEK_CUR);
      }
      cfg_parser_parse_token(curr_data, mode);
      free(curr_data);
      curr_data=NULL;
      data_len=1;
      ign_ws=XMLD_TRUE;
      mode=XMLD_CFG_MODE_EQ;
     }
    }
    else if (mode == XMLD_CFG_MODE_EQ) {
     if (buf == '=') {
      mode=XMLD_CFG_MODE_VAL;
     }
    }
    else if (mode == XMLD_CFG_MODE_VAL) {
     if (buf == '\n' || buf == '\r') {
      cfg_parser_parse_token(curr_data, mode);
      free(curr_data);
      curr_data=NULL;
      data_len=1;
      ign_ws=XMLD_TRUE;
      mode=XMLD_CFG_MODE_TYPE;
     }
     else {
      ign_ws=XMLD_FALSE;
      curr_data=(char *) realloc(curr_data, (++data_len)*sizeof(char));
      curr_data[data_len-2]=buf;
      curr_data[data_len-1]='\0';
     }
    }
    else if (mode == XMLD_CFG_MODE_END) {
     if (buf == '\n' || buf == '\r') {
      mode=XMLD_CFG_MODE_TYPE;
      ign_ws=XMLD_TRUE;
     }
    }
   }
  } 
 }
 return XMLD_SUCCESS;
}

/*
 * : A small parser to parse tokens extracted by cfg_parser_parse.
 * returns: whether successful.
 */
void cfg_parser_parse_token(char *token, int mode) {
 if (cfg_tree == NULL) {
  cfg_tree=XMLDDirective_create_list();
 }
 XMLDDirective *curr_dir;
 if (mode == XMLD_CFG_MODE_TYPE) {
  curr_dir=XMLDDirective_add_to_list(cfg_tree);
  XMLDList_next(cfg_tree);
  if (strcmp(token, "int") == 0) {
   curr_dir->type=XMLD_DIR_INT;
  }
  else if (strcmp(token, "int*") == 0) {
   curr_dir->type=XMLD_DIR_INTARR;
  }
  else if (strcmp(token, "char*") == 0) {
   curr_dir->type=XMLD_DIR_STR;
  }
  else if (strcmp(token, "char**") == 0) {
   curr_dir->type=XMLD_DIR_STRARR;
  }
  else if (strcmp(token, "char") == 0) {
   curr_dir->type=XMLD_DIR_CHAR;
  }
  else {
   curr_dir->type=XMLD_DIR_ERR;
  }
 }
 else if (mode == XMLD_CFG_MODE_DIR) {
  curr_dir=(XMLDDirective *) XMLDList_curr(cfg_tree);
  if (curr_dir->type == XMLD_DIR_ERR) {
   return;
  }
  else {
   curr_dir->name=(char *) malloc((strlen(token)+1)*sizeof(char));
   strcpy(curr_dir->name, token);
  }
 }
 else if (mode == XMLD_CFG_MODE_VAL) {
  curr_dir=(XMLDDirective *) XMLDList_curr(cfg_tree);
  if (curr_dir->type == XMLD_DIR_ERR) {
   return;
  }
  switch(curr_dir->type) {
   case XMLD_DIR_INT:
    curr_dir->value.int_value=atoi(ltrim(token));
   break; 
   case XMLD_DIR_INTARR:
    char **str_array=str_split(token, ',');
    int num=1;
    while (*str_array != NULL) {
     num++;
     curr_dir->value.int_array_value=(int *) realloc(curr_dir->value.int_array_value, num*sizeof(int));
     curr_dir->value.int_array_value[num-1]=0;
     curr_dir->value.int_array_value[num-2]=atoi(ltrim(*str_array));
     free(*str_array);
     str_array++;
    }
    while (num > 1) {
     num--;
     str_array--;
    }
    free(str_array);
   break;
   case XMLD_DIR_STR:
    curr_dir->value.string_value=(char *) malloc((strlen(token)+1)*sizeof(char));
    strcpy(curr_dir->value.string_value, token);
   break; 
   case XMLD_DIR_STRARR:
    curr_dir->value.string_array_value=str_split(token, ',');
   break;
   case XMLD_DIR_CHAR:
    if (strcmp(token, "\\t") == 0) {
     curr_dir->value.char_value='\t';
    }
    else if (strcmp(token, "\\r") == 0) {
     curr_dir->value.char_value='\r';
    }
    else {
     curr_dir->value.char_value=token[0];
    } 
   break; 
  }
 }
}

/*
 * : Frees up resources used by the parse tree
 * created by cfg_parser_parse, and removes
 * configuration directives from memory.
 * returns: whether successful.
 */
XMLDStatus cfg_parser_clean() {
 XMLDList_free(cfg_tree);
 return XMLD_SUCCESS;
}
