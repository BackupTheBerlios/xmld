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
#include "xmlddef.h"
#include "xmld_sockets.h"
#include "sutils.h"
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_cond.h"
#include "xmld_col.h"
#include "xmld_row.h"
#include "xmld_aggr_table.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_request.h"
#include "xmld_response.h"
#include "xmld_resource.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"
#include "cfg.h"

/*
 * Translates a response structure to a string that 
 * can be sent over a socket.
 */ 
char *resptrans_handle(XMLDWork *work) {
 char *response=(char *) malloc(sizeof(char));
 response[0]='\0';
 int resp_len=1;
 XMLDList_reset(work->resp->rows);
 XMLDRow *curr_row;
 XMLDCol *curr_col;
 
 /* mstrchr_replace vars */
 char repl_tok[4];
 repl_tok[0]=col_sep;
 repl_tok[1]=row_sep;
 repl_tok[2]=down_level;
 repl_tok[3]=up_level;
 char *repl_tok_enc[4];
 repl_tok_enc[0]=col_sep_enc;
 repl_tok_enc[1]=row_sep_enc;
 repl_tok_enc[2]=down_level_enc;
 repl_tok_enc[3]=up_level_enc;
 
 while (XMLDList_next(work->resp->rows)) {
  curr_row=(XMLDRow *) XMLDList_curr(work->resp->rows);
  char attach='\0';
  
  if (curr_row->level_act == XMLD_ROW_DOWN) {
   attach=down_level;
  }
  else if (curr_row->level_act == XMLD_ROW_UP) {
   attach=up_level;
  }
  
  if (attach != '\0') {
   response=(char *) realloc(response, (++resp_len) * sizeof(char));
   response[resp_len-2]=attach;
   response[resp_len-1]='\0';
  }
  
  XMLDList_reset(curr_row->cols);
  while (XMLDList_next(curr_row->cols)) {
   curr_col=(XMLDCol *) XMLDList_curr(curr_row->cols);
   if (curr_col->val != NULL) {
    char *tmp=mstrchr_replace(curr_col->val, repl_tok, repl_tok_enc, 4);
    free(curr_col->val);
    curr_col->val=tmp;
   }
   resp_len+=((curr_col->val != NULL) ? strlen(curr_col->val) : 0)+1;
   response=(char *) realloc(response, resp_len*sizeof(char));
   if (curr_col->val != NULL) {
    strcat(response, curr_col->val);
   } 
   response[resp_len-2]=col_sep;
   response[resp_len-1]='\0';
  }
  response=(char *) realloc(response, (++resp_len) * sizeof(char));
  response[resp_len-2]=row_sep;
  response[resp_len-1]='\0';
 }
 return response;
}
