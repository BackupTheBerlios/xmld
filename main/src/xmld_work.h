#ifndef HAVE_XMLD_WORK_H
#define HAVE_XMLD_WORK_H

struct XMLDWork {
 XMLDRequest *req;
 XMLDResponse *resp;
 XMLDConnection *conn;
 XMLDResource *res;
};

typedef struct XMLDWork XMLDWork;

void XMLDWork_free_content(XMLDWork *);
void XMLDWork_free(XMLDWork *);

#endif
