#include "xmld_errors.h"

void init_error_array() {
 err_str[0]="The request file couldn't be opened.";
 err_str[1]="The given engine does not exist.";
 err_str[2]="Invalid file type.";
 err_str[3]="Invalid use of an aggregate expression.";
 err_str[4]="An error has occured while parsing the given query.";
 err_str[5]="The given query is not yet implemented.";
 err_str[6]="Couldn't open a description file for the request file.";
}

