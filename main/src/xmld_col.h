#ifndef HAVE_XMLD_COL_H
#define HAVE_XMLD_COL_H

struct XMLDCol {
 char *val;
};

void XMLDCol_free_content(XMLDCol *)
void XMLDCol_free(XMLDCol *);

typedef struct XMLDCol XMLDCol;

#endif /* HAVE_XMLD_COL_H */
