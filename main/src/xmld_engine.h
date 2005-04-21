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
 
#ifndef __XMLD_ENGINE_H
#define __XMLD_ENGINE_H

/* Access level bitmask */
#define XMLD_ACCESS_READ  0  /* Read Access                  */
#define XMLD_ACCESS_WRITE 1  /* Write Access                 */
#define XMLD_ACCESS_ALTER 2  /* Alteration of Document Shape */

/* walk return values */
#define XMLD_WALK_UP -1  /* One level shallower */
#define XMLD_WALK_END 0  /* End of document     */
#define XMLD_WALK_DOWN 1 /* One level deeper    */

/* Engine Capabilities Bitmask */
#define ENGINE_CAP_MULTI_LEVEL 1
#define ENGINE_CAP_NODE_NAME 2
#define ENGINE_CAP_NODE_ABSOLUTE_VALUE 4
#define ENGINE_CAP_LABEL_ITERATION 6
#define ENGINE_CAP_NODE_MODIFY 8

struct XMLDEngine {
 char *name;
 int engine_cap;
 void (*init) (void);
 XMLDBool (*is_valid_mime) (char *);
 XMLDStatus (*prepare) (XMLDFile *, int); 
 void (*cleanup) (XMLDFile *);
 void (*destroy) (void);
 int (*walk) (XMLDFile *);
 char *(*get_labeled_node_value_type) (XMLDFile *, char *);
 int (*get_labeled_node_value_length) (XMLDFile *, char *);
 char *(*get_labeled_node_value) (XMLDFile *, char *);
 char *(*get_absolute_node_value_type) (XMLDFile *);
 int (*get_absoulte_node_value_length) (XMLDFile *);
 char *(*get_absolute_node_value) (XMLDFile *);
 char *(*get_node_name) (XMLDFile *);

 /* Iterative labeled node value selection API */
 
 void (*reset_node) (XMLDFile *);
 XMLDBool (*next_label) (XMLDFile *);
 char *(*get_current_label) (XMLDFile *);
 char *(*get_current_labeled_value_type) (XMLDFile *);
 int (*get_current_labeled_value_length) (XMLDFile *);
 char *(*get_current_labeled_value) (XMLDFile *);

 /* Data source modification API */

 XMLDBool (*set_labeled_node_value) (XMLDFile *, char *, char *);
 XMLDBool (*set_absolute_node_value) (XMLDFile *, char *);
 XMLDBool (*set_node_name) (XMLDFile *, char *); 
};

#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */

XMLDEngine *XMLDEngine_create(char *);
void XMLDEngine_free(XMLDEngine *);
void XMLDEngine_free_content(void *);

#endif /* __XMLD_ENGINE_H */
