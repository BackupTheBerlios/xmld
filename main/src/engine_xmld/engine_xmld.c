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
#include "../xmld_errors.h"
#include "../mfigure.h"
#include "../fmanager.h"
#include "engine_xmld.h"
#include "element_op.h"
#include "format_manip.h"

/* init function */
void engine_xmld_init() {
 /* 
  * init (in this case) is quiete useless,
  * might be useful for persistent things which may appear later (?)
  */
}

/* prepare function */
short engine_xmld_prepare(XMLDWork *work) {
 char *full_name=XMLDWork_get_full_file(work);
 char *mime=mfigure_get_mime(full_name);
 
 if (strcmp(mime, "text/xml") != 0) { /* A XML-only engine! */
  xmld_errno=XMLD_EINVALFILE;
  free(mime);
  free(full_name);
  return 0;
 }
 
 free(mime);
 if (work->req->type == 4 || work->req->type == 5 || work->req->type == 6
     || work->req->type == 7 || work->req->type == 8 || work->req->type == 9) { /* Operations that need the .format file */
  work->res->data_source=(void *) fmanager_get_ex_fd(full_name);
 }
 else {
  work->res->data_source=(void *) fmanager_get_sh_fd(full_name);
 }
 
 if (work->res->data_source == NULL) {
  xmld_errno = XMLD_ENOFILE;
  free(full_name);
  return 0;
 }
 
 work->res->store=malloc(3*sizeof(int));
 *((int *) work->res->store)=0; /* <-- the current level in the document */
 *((int *) work->res->store+2)=engine_xmld_load_format_file((FILE *) (work->res->store+1), full_name, work->req->type);
 free(full_name);

 return 1;
}

/* cleanup function */
void engine_xmld_cleanup(XMLDWork *work) {
 if (*((int *) work->res->store+2)) {
  fmanager_unlock_fd((FILE *) (work->res->store+1));
  fclose((FILE *) (work->res->store+1));
 }
 free(work->res->store);
 fmanager_unlock_fd((FILE *) (work->res->data_source));
 fclose((FILE *) work->res->data_source);
}

/* destroy function */
void engine_xmld_destroy() {
}

/* walk function */
int engine_xmld_walk(XMLDWork *work) {
 short token;
 char *tokens[3]={"<", "/>", "</"};
 
 while (1) {
  token=dmstrstr((FILE *) work->res->data_source, tokens, 3);
  if (token == -1) {
   return *((int *) work->res->store);
  }
  else if (token == 0) {
   return ++(*((int *) work->res->store));
  }
  else if (token == 1 || token == 2) {
  (*((int *) work->res->store))--;
  }
 }
}

/* eval_expr function */
char *engine_xmld_eval_expr(XMLDWork *work, XMLDExpr *expr) {
 char *ret;
 if (expr->type == 0) { /* expr is of a numeric type */
  return ltoa(expr->nval);
 }
 else if (expr->type == 2) { /* expr is a column name */
  return engine_xmld_get_column_value(work, expr->ident);  
 }
 else if (expr->type == 4) { /* expr is a string value */
  ret=(char *) malloc((strlen(expr->qval)+1)*sizeof(char));
  strcpy(ret, expr->qval);
  return ret;
 }
 else if (expr->type == 5) { /* expr is a wildcard */
  return engine_xmld_get_column_value(work, (expr->wildcard == 0) ? "*" : "@");
 }
 else {
  XMLDExpr *temp_expr=XMLDExpr_create();
  XMLDExpr_copy(expr, temp_expr);
  engine_xmld_simplify_expr(work, temp_expr);
  ret=engine_xmld_eval_expr(work, temp_expr);
  XMLDExpr_free(temp_expr);
  return ret;
 }  
}

/* eval_cond function */
short engine_xmld_eval_cond(XMLDWork *work, XMLDCond *cond) {
 short val;
 char *left_val;
 char *right_val;
 if (cond->type == 0) {
  if (cond->op == 7) {
   left_val=engine_xmld_eval_expr(work, cond->left);
   char *bet_left=engine_xmld_eval_expr(work, cond->right->left);
   char *bet_right=engine_xmld_eval_expr(work, cond->right->right);
   val=str_between(left_val, bet_left, bet_right);
   free(left_val);
   free(bet_left);
   free(bet_right);
  }
  else if (cond->op == 8) {
   left_val=engine_xmld_eval_expr(work, cond->left);
   char *bet_left=engine_xmld_eval_expr(work, cond->right->left);
   char *bet_right=engine_xmld_eval_expr(work, cond->right->right);
   val=!str_between(left_val, bet_left, bet_right);
   free(left_val);
   free(bet_left);
   free(bet_right);
  }
  else {
   left_val=engine_xmld_eval_expr(work, cond->left);
   right_val=engine_xmld_eval_expr(work, cond->right);
   switch(cond->op) {
    case 0:
     if (strcmp(left_val, right_val) == 0) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 1:
     if (atol(left_val) < atol(right_val)) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 2:
     if (atol(left_val) > atol(right_val)) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 3:
     if (atol(left_val) != atol(right_val)) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 4:
     if (atol(left_val) <= atol(right_val)) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 5:
     if (atol(left_val) >= atol(right_val)) {
      val=1;
     }
     else {
      val=0;
     }
    break;
    case 6:
     val=str_like(left_val, right_val);
    break;
   }
   free(left_val);
   free(right_val);
  }
 }
 else if (cond->type == 1) {
  if (cond->cop == 0) {
   val=(engine_xmld_eval_cond(work, cond->cleft) && engine_xmld_eval_cond(work, cond->cright));
  }
  else if (cond->cop == 1) {
   val=(engine_xmld_eval_cond(work, cond->cleft) || engine_xmld_eval_cond(work, cond->cright));
  }
 }
 else if (cond->type == 2) {
  val=1;
 }
 if (cond->negate == 1) {
  return !val;
 }
 else {
  return val;
 }
}

/*
 * Simplifies a XMLDExpr to a type directly
 * interpretable by engine_xmld_eval_expr.
 */
void engine_xmld_simplify_expr(XMLDWork * work, XMLDExpr *expr) {
 if (expr->type == 1) {
  XMLDExpr *temp_left=XMLDExpr_create();
  XMLDExpr *temp_right=XMLDExpr_create();
  XMLDExpr_copy(expr->left, temp_left);
  XMLDExpr_copy(expr->right, temp_right);
  engine_xmld_simplify_expr(work, temp_left);
  engine_xmld_simplify_expr(work, temp_right);
  
  /* identifier resolution */
  char *col_value;
  if (temp_left->type == 2) {
   temp_left->type=0;
   col_value=engine_xmld_get_column_value(work, temp_left->ident);
   temp_left->nval=atol(col_value);
   free(col_value);
  }
  if (temp_right->type == 2) {
   temp_right->type=0;
   col_value=engine_xmld_get_column_value(work, temp_right->ident);
   temp_right->nval=atol(col_value);
   free(col_value);
  }
  
  if (temp_left->type == 0) {
   if (expr->op == 0) {
    expr->type = 0;
    expr->nval=temp_left->nval+temp_right->nval;
   }
   else if (expr->op == 1) {
    expr->nval=temp_left->nval-temp_right->nval;
   }
   else if (expr->op == 2) {
    expr->nval=temp_left->nval * temp_right->nval;
   }
   else if (expr->op == 3) {
    if (temp_right->nval == 0) {
     expr->nval=LONG_MAX;
    }
    else {
     expr->nval=(long) (temp_left->nval / temp_right->nval);
    } 
   }
   else if (expr->op == 4) {
    expr->nval=(long) pow(temp_left->nval, temp_right->nval);
   }
   else if (expr->op == 5) {
    expr->nval=(long) (-1 * temp_right->nval);
   }    
  }
  XMLDExpr_free(temp_left);
  XMLDExpr_free(temp_right);
 }
 else if (expr->type == 3) {
  if (expr->func->aggr == 1) {
   XMLDExpr *work_expr=XMLDFunc_add_to_list(expr->arg_list);
   work_expr->nval=(long) work;
  }
  XMLDExpr *func_res=(*(expr->func->func)) (expr->arg_list);
  XMLDExpr_copy(func_res, expr);
  XMLDExpr_free(func_res);
 }
}

/*
 * Gets the value of a particular column (attribute/text)
 * relative to the current element.
 */ 
char *engine_xmld_get_column_value(XMLDWork *work, char *col_name) {
 fpos_t pos;
 int token;
 fgetpos((FILE *) work->res->data_source, &pos);
 char *ret;
 
 if (strcasecmp(col_name, "(text)") == 0) {
  if (engine_xmld_locate_text((FILE *) (work->res->data_source))) {
   ret=engine_xmld_get_text_value((FILE *) (work->res->data_source));
  }
  else {
   ret=NULL;
  }
 }
 else if (strcasecmp(col_name, "(tagname)") == 0) {
  ret=engine_xmld_get_tagname((FILE *) work->res->data_source);
 }
 else if (strcmp(col_name, "*") == 0) {
  char *atts=engine_xmld_get_column_value(work, "@");
  char *text=engine_xmld_get_column_value(work, "(text)");
  ret=(char *) malloc((strlen(atts)+strlen(text)+2)*sizeof(char));
  strcpy(ret, atts);
  ret[strlen(atts)]=col_sep;
  strcat(ret, text);
 }
 else if (strcmp(col_name, "@") == 0) {
  ret=(char *) malloc(sizeof(char));
  ret[0]='\0';
  while (1) {
   token=dmstrchr((FILE *) work->res->data_source, " >", 2);
   if (token == 1) { /* Attribute not found */
    break;
   }
   else if (token == 0) {
    engine_xmld_discard_curr_att_name((FILE *) work->res->data_source);
    char *att_value=engine_xmld_get_curr_att_value((FILE *) work->res->data_source);
    ret=(char *) realloc(ret, (strlen(ret)+strlen(att_value)+2)*sizeof(char));
    if (strlen(ret) > 0) {
     ret[strlen(ret)]=col_sep;
    } 
    strcat(ret, att_value);
    free(att_value);
   } 
  }
 }
 else {
  if (engine_xmld_locate_att((FILE *) (work->res->data_source), col_name)) {
   ret=engine_xmld_get_curr_att_value((FILE *) (work->res->data_source));
  }
  else {
   ret=NULL;
  }
 }
 fsetpos((FILE *) work->res->data_source, &pos);
 return ret;
}

short engine_xmld_set_column_value(XMLDWork *work, char *col_name, char *value) {
 if (strcasecmp(col_name, "(text)") == 0) {
  if (engine_xmld_locate_text((FILE *) (work->res->data_source))) {
   return engine_xmld_set_text_value((FILE *) (work->res->data_source), value);
  }
  else {
   return 0;
  }
 }
 else {
  if (engine_xmld_locate_att((FILE *) (work->res->data_source), col_name)) {
   return engine_xmld_set_curr_att_value((FILE *) (work->res->data_source), value);
  }
  else {
   return 0;
  }
 }
}
