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
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "../xmlddef.h"
#include "../mutils.h"
#include "../dutils.h"
#include "../sutils.h"
#include "../xmld_list.h"
#include "../xmld_col.h"
#include "../xmld_row.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "../xmld_expr.h"
#include "../xmld_func.h"
#include "../xmld_aggr_table.h"
#include "../xmld_cond.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "../xmld_resource.h"
#include "../xmld_response.h"
#include "../xmld_request.h"
#include "../xmld_connection.h"
#include "../xmld_work.h"
#include "../xmld_engine.h"
#include "../cfg.h"
#include "../mfigure.h"
#include "../fmanager.h"
#include "engine_xmld.h"
#include "element_op.h"
#include "format_manip.h"

/* init function */
void engine_xmld_init() {
 /* 
  * init is useless in engine_xmld.
  * but it might be useful for persistent things which may appear later (?)
  */
}

/* prepare function 
 * level: see xmld_engine.h
 * */
XMLDStatus engine_xmld_prepare(XMLDWork *work, XMLDFile *file, int level) {
 char *full_name=XMLDFile_get_full_name(file, work);
 char *mime=mfigure_get_mime(full_name);
 
 if (strcmp(mime, "text/xml") != 0) { /* A XML-only engine! */
  xmld_errno=XMLD_EINVALFILE;
  free(mime);
  free(full_name);
  return XMLD_FAILURE;
 }
 
 free(mime);
 if (BIT_ISSET(level, XMLD_ACCESS_EX)) {
  file->data=(void *) fmanager_get_ex_fd(full_name);
 }
 else {
  file->data=(void *) fmanager_get_sh_fd(full_name);
 }
 
 if (file->data == NULL) {
  xmld_errno = XMLD_ENOFILE;
  free(full_name);
  return XMLD_FAILURE;
 }
 
 file->level=0;
 if (BIT_ISSET(level, XMLD_ACCESS_FORMAT)) {	 
  file->store=(void *) engine_xmld_load_format_file(full_name, BIT_ISSET(level, XMLD_ACCESS_FORMAT_EX));
  if (file->store == NULL) {
   xmld_errno = XMLD_ENOFORMAT;
   return XMLD_FAILURE;
  }  
 }
 else {
  file->store = NULL;
 }
 free(full_name);
 return XMLD_SUCCESS;
}
/* cleanup function */
void engine_xmld_cleanup(XMLDWork *work, XMLDFile *file) {
 if (file->store != NULL) {
  fmanager_unlock_fd((FILE *) file->store);
  fclose((FILE *) file->store);
 }
 fmanager_unlock_fd((FILE *) file->data);
 fclose((FILE *) file->data);
}

/* destroy function */
void engine_xmld_destroy() {
}

/* walk function
 * return values:
 * XMLD_WALK_UP  : shallower by one
 * XMLD_WALK_END : end of document
 * XMLD_WALK_DOWN: deeper by one
 */ 
int engine_xmld_walk(XMLDWork *work, XMLDFile *file) {
 short token;
 char buf;
 char *tokens[2]={"<", "/>"};
  
 while (1) {
  token=dmstrstr((FILE *) file->data, tokens, 2);
  if (token == -1) {
   return XMLD_WALK_END;
  }
  else if (token == 0) {
   buf=getc((FILE *) file->data);
   if (buf == '/') {
    file->level--;
    if (file->level == 0) {
     return XMLD_WALK_END;
    }
    else {
     return XMLD_WALK_UP;
    }
   }
   else {
    fseek((FILE *) file->data, -1, SEEK_CUR);
    file->level++;
    return XMLD_WALK_DOWN;
   } 
  }
  else if (token == 1) {
   file->level--;
   if (file->level == 0) {
    return XMLD_WALK_END;
   }
   else {
    return XMLD_WALK_UP;
   }
  }
 }
}

/* eval_expr function */
char *engine_xmld_eval_expr(XMLDWork *work, XMLDExpr *expr, int level) {
 char *ret;
 if (expr->type == XMLD_INTEGER) {
  return itostr(expr->nval, 0);
 } 
 else if (expr->type == XMLD_QVAL) {
  ret=(char *) malloc((strlen(expr->qval)+1)*sizeof(char));
  strcpy(ret, expr->qval);
  return ret;
 }
 else if (expr->type == XMLD_FLOAT) {
  return ftostr(expr->fnval, 0);
 }
 else {
  XMLDExpr *tmp=engine_xmld_simplify_expr(work, expr, level);
  ret=engine_xmld_eval_expr(work, tmp, level);
  XMLDExpr_free(tmp);
  return ret;
 }  
}

/* eval_cond function */
XMLDBool engine_xmld_eval_cond(XMLDWork *work, XMLDCond *cond, int level) {
 XMLDBool val;
 if (cond->type == XMLD_CONDITION) {
  XMLDExpr *left, right;
  if (XMLDExpr_is_complex(expr->left)) {
   left=engine_xmld_simplify_expr(work, expr->left, level);
  }
  else {
   left=expr->left;
  }
  if (XMLDExpr_is_complex(expr->right)) {
   right=engine_xmld_simplify_expr(work, expr->right, level);
  }
  else {
   right=expr->right;
  }
  XMLDExpr *left=engine_xmld_simplify_expr(work, cond->left, level);
  XMLDExpr *right=engine_xmld_simplify_expr(work, cond->right, level);
  
  if (left == NULL || right == NULL) {
   return XMLD_FALSE;
  }
  
  switch(cond->op) {
   case XMLD_COND_OP_EQUAL:
   break;
   case XMLD_COND_OP_L:
   break;
   case XMLD_COND_OP_G:
   break;
   case XMLD_COND_OP_NE:
   break;
   case XMLD_COND_OP_LE:
   break;
   case XMLD_COND_OP_GE:
   break;
   case XMLD_COND_OP_LIKE:
   break;
   case XMLD_COND_OP_BET:
    val = XMLD_TRUE;
   break;
   case XMLD_COND_OP_NBET:
    val = XMLD_TRUE;
   break;
  }
 
  if (XMLDExpr_is_complex(expr->left)) {
   XMLDExpr_free(left);
  }
  if (XMLDExpr_is_complex(expr->right)) {
   XMLDExpr_free(right);
  }	  
 }
 else if (cond->type == XMLD_CONDITION_GRP) {
  if (cond->cop == XMLD_COND_GRP_AND) {
   val=(engine_xmld_eval_cond(work, cond->cleft, level) && engine_xmld_eval_cond(work, cond->cright, level));
  }
  else if (cond->cop == XMLD_COND_GRP_OR) {
   val=(engine_xmld_eval_cond(work, cond->cleft, level) || engine_xmld_eval_cond(work, cond->cright, level));
  }
 }
else if (cond->type == XMLD_CONDITION_VOID) {
  val = XMLD_TRUE;
 }
 else if (cond->type == XMLD_CONDITION_LIST) {
  val=engine_xmld_eval_cond(work, (XMLDCond *) XMLDList_first(cond->conds), level);
 }
 
 if (cond->negate == XMLD_TRUE) {
  return !val;
 }
 else {
  return val;
 }
}

/*
 * Simplifies a XMLDExpr to a basic type.
 */
XMLDExpr *engine_xmld_simplify_expr(XMLDWork *work, XMLDExpr *expr, int level) {
 XMLDExpr *ret;
 if (expr->type == XMLD_OPERATION) {
  XMLDExpr *left, right;
  if (XMLDExpr_is_complex(expr->left)) {
   left=engine_xmld_simplify_expr(work, expr->left, level);
  }
  else {
   left=expr->left;
  }
  if (XMLDExpr_is_complex(expr->right)) {
   right=engine_xmld_simplify_expr(work, expr->right, level);
  }
  else {
   right=expr->right;
  }
  
  if (left == NULL ||  right == NULL) {
   cfree(left);
   cfree(right);
   return NULL;   
  }

  switch (expr->op) {
   case XMLD_OP_ADD:
   break;
   case XMLD_OP_BNEG:
   break;
   case XMLD_OP_MULTIP:
   break;
   case XMLD_OP_DIV:
   break;
   case XMLD_OP_EXPO;
   break;
   case XMLD_OP_UNEG:
   break;
   case XMLD_OP_AND:
   break;
  }
  
  if (XMLDExpr_is_complex(expr->left)) {
   XMLDExpr_free(left);
  }
  if (XMLDExpr_is_complex(expr->right)) {
   XMLDExpr_free(right);
  }	  
 }
 else if (expr->type == XMLD_IDENTIFIER) {
  if (expr->file == NULL) {
   expr->file = (XMLDFile *) XMLDList_first(work->files);
  }
  if (expr->file->store == NULL) {
   return NULL;
  }
  if (level != 0 && expr->file->level != level) {
   return NULL;
  }
  
  fpos_t pos;
  fgetpos((FILE *) expr->file->data, &pos);
  char *tagname=engine_xmld_get_tagname((FILE *) expr->file->data);
  fsetpos((FILE *) expr->file->data, &pos);
  char *type=engine_xmld_get_element_att_type((FILE *) expr->file->store, level, tagname, expr->ident);
  free(tagname);
  
  if (type == NULL) {
   return NULL;
  }
  if (strcasecmp(type, XMLD_TYPE_CHAR) == 0) {
   ret=XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
  }
  else if (strcasecmp(type, XMLD_TYPE_INT)) {
   ret=XMLDExpr_create();
   ret->type = XMLD_INTEGER;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
   ret->nval=atoi(ret->qval);
   free(ret->qval);
   ret->qval=NULL;
  }
  else if (strcasecmp(type, XMLD_TYPE_FLOAT)) {
   ret=XMLDExpr_create();
   ret->type = XMLD_FLOAT;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
   ret->fnval=atof(ret->qval);
   free(ret->qval);
   ret->qval=NULL;
  }
  else {
   return NULL;
  }
  
  free(type);
 }
 else if (expr->type == XMLD_FUNCTION) {
  ret=(*(expr->func->func)) (expr->arg_list, expr->file);
 }
 else if (expr->type == XMLD_WILDCARD) {
  if (expr->file == NULL) {
   expr->file = (XMLDFile *) XMLDList_first(work->files);
  }
  ret=XMLDExpr_create();
  ret->type=XMLD_QVAL;
  ret->qval=engine_xmld_get_column_value(expr->file, (expr->wildcard == XMLD_WILDCARD_ALL) ? 0 : 1);
 }
 return ret;
}

/*
 * Gets the value of a particular column (attribute/text)
 * relative to the current element in the given file.
 */
char *engine_xmld_get_column_value(XMLDFile *file, char *col_name) {
 fpos_t pos;
 int token;
 fgetpos((FILE *) file->data, &pos);
 char *ret;
 
 if (strcasecmp(col_name, "[text]") == 0) {
  if (engine_xmld_locate_text((FILE *) file->data)) {
   ret=engine_xmld_get_text_value((FILE *) file->data);
  }
  else {
   ret=NULL;
  }
 }
 else if (strcasecmp(col_name, "[tagname]") == 0) {
  ret=engine_xmld_get_tagname((FILE *) file->data);
 }
 else if (strcmp(col_name, "*") == 0) {
  char *atts=engine_xmld_get_column_value(file, "@");
  char *text=engine_xmld_get_column_value(file, "[text]");
  if (text != NULL) {
   ret=(char *) malloc((strlen(atts)+strlen(text)+2)*sizeof(char));
  }
  else {
   ret=(char *) malloc((strlen(atts)+1)*sizeof(char));
  }
gat da7'a  strcpy(ret, atts);
  if (text != NULL) {
   ret[strlen(atts)]=col_sep;
   strcat(ret, text);
   free(text);
  }
  free(atts);
 }
 else if (strcmp(col_name, "@") == 0) {
  ret=(char *) malloc(sizeof(char));
  ret[0]='\0';
  int ret_len=1;
  while (1) {
   token=dmstrchr((FILE *) file->data, " >", 2);
   if (token == 1) { /* Attribute not found */
    if (ret_len == 1) {
     free(ret);
     ret=NULL;
    }
    else {
     ret_len-=1;
     ret=(char *) realloc(ret, ret_len*sizeof(char));
     ret[ret_len-1]='\0';
    } 
    break;
   }
   else if (token == 0) {
    engine_xmld_discard_curr_att_name((FILE *) file->data);
    char *att_value=engine_xmld_get_curr_att_value((FILE *) file->data);
    ret_len+=strlen(att_value)+1;
    ret=(char *) realloc(ret, (ret_len)*sizeof(char));
    strcat(ret, att_value);
    ret[ret_len-2]=col_sep;
    ret[ret_len-1]='\0';
    free(att_value);
   } 
  }
 }
 else {
  if (engine_xmld_locate_att((FILE *) file->data, col_name)) {
   ret=engine_xmld_get_curr_att_value((FILE *) file->data);
  }
  else {
   ret=NULL;
  }
 }
 fsetpos((FILE *) file->data, &pos);
 return ret;
}

/* FIXME: always use the format file */
XMLDStatus engine_xmld_set_column_value(XMLDWork *work, char *col_name, char *value) {
 fpos_t pos;
 fgetpos((FILE *) work->res->data_source, &pos);
 short ret;
 if (*((int *) work->res->store+2)) { /* we have got a format file */
  int stat;
  char *tag_name=engine_xmld_get_tagname((FILE *) work->res->data_source);
  if (strcasecmp(col_name, "[text]") == 0) {
   if (engine_xmld_locate_text((FILE *) work->res->data_source)) {
    int len=engine_xmld_get_element_att_length((FILE *) work->res->store+1, *((int*) work->res->store, "[text]"),
		    tag_name);
    fgetc((FILE *) work->res->data_source); /* Eat the quote up */
    if (strlen(value) >= len) {
     stat=fwrite((void *) value, sizeof(char), len, (FILE *) work->res->data_source);
    }
    else {
     stat=fwrite((void *) value, sizeof(char), strlen(value), (FILE *) work->res->data_source);
    }
    ret = (stat == 0) ? 0 : 1;
   }
   else {
    ret=0;
   }
  }
  else {
   if (engine_xmld_locate_att((FILE *) work->res->data_source, col_name)) {
    int len=engine_xmld_get_element_att_length((FILE *) work->res->store+1, *((int*) work->res->store),
		    tag_name, col_name);
    if (strlen(value) >= len) {
     stat=fwrite((void *) value, sizeof(char), len, (FILE *) work->res->data_source);
    }
    else {
     stat=fwrite((void *) value, sizeof(char), strlen(value), (FILE *) work->res->data_source);
    }
    ret = (stat == 0) ? 0 : 1;
   }
   else {
    ret=0;
   }
  }
  free(tag_name);
 }
 else {
  if (strcasecmp(col_name, "[text]") == 0) {
   if (engine_xmld_locate_text((FILE *) (work->res->data_source))) {
    ret=engine_xmld_set_text_value((FILE *) (work->res->data_source), value);
   }
   else {
    ret=0;
   }
  }
  else {
   if (engine_xmld_locate_att((FILE *) (work->res->data_source), col_name)) {
    ret=engine_xmld_set_curr_att_value((FILE *) (work->res->data_source), value);
   }
   else {
    ret=0;
   }
  }
 } 
 fsetpos((FILE *) work->res->data_source, &pos);
 return ret;
}
