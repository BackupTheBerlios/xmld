#ifndef HAVE_XMLD_TYPES_H
#define HAVE_XMLD_TYPES_H

/* represents a single-level conditional statement */
struct XMLDCond {
 struct XMLDCond *left;
 struct XMLDCond *right;
 int connector; /* connector values:
                 * 0 = terminal conditional
                 * 1 = AND
                 * 2 = OR
                 * 3 = XOR
                 */ 
 char *ident;
 char *val; /* if =0 then use nval (numeric val) instead */
 long nval;
 int op; /* op values:
	  * 0 = '='
	  * 1 = '<'
	  * 2 = '>'
	  * 3 = '<='
	  * 4 = '>='
	  * 5 = LIKE
	  * 6 = '<>'
	  */
 int negate;     /* if =1 then it checks for false instead of true */
};

/* carries the parsed query */
struct XMLDRequest {
 char *file;
 char **retr;
 struct XMLDCond *cond; /* the conditional for each level */
 int num_cond;
};

/* carries the response to be sent */
struct XMLDResponse {
};

/* carries per-request resources */
struct XMLDResource {
};

/* carries connection-specific information */
struct XMLDConnection {
 int fd;
 int sfd;
 char *curr_dir; /* the current directory */
 char *user; /* for authentication in the future */
};

struct XMLDWork {
 struct XMLDRequest *req;
 struct XMLDResponse *resp;
 struct XMLDResource *res;
 struct XMLDConnection *conn;
};

/* FIXME: add functions that free (and create) each of the above struct-
 * ures whensever required, and create a file xmld_types.c to carry
 * the functions' implementations.
 */ 
#endif
