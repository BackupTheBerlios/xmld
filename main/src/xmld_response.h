#ifndef HAVE_XMLD_RESPONSE_H
#define HAVE_XMLD_RESPONSE_H

struct XMLDResponse {
 XMLDRow *rows;
 XMLDRow *last_row; /* the last row in _rows_ */
 XMLDAggrTable *tables; /* the list of aggregate expressions -> columns association */
 XMLDAggrTable *last_table; /* the last table in _tables_ */
 XMLDAggrTable *curr_table; /* the current table in _tables_ */
};

typedef struct XMLDResponse XMLDResponse;

void XMLDResponse_init(XMLDResponse *);
void XMLDResponse_add_row(XMLDResponse *);
void XMLDResponse_fill_col(XMLDResponse *, char *); /*
						     * Note the given string isn't copied.
						     */
void XMLDResponse_add_col(XMLDResponse *);
void XMLDResponse_add_aggr(XMLDResponse *, XMLDExpr *);
void XMLDResponse_aggr_reset(XMLDResponse *); /* Resets _curr_table_ */
XMLDExpr *XMLDResponse_curr_aggr(XMLDResponse *); /* return the current aggr expr */
void XMLDResponse_fill_aggr(XMLDResponse *, char *);
void XMLDResponse_aggr_next(XMLDResponse *);
void XMLDResponse_free_content(XMLDResponse *);

#endif /* HAVE_XMLD_RESPONSE_H */
