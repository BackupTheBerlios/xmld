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
#define ENGINE_CAP_NODE_CREATE 10
#define ENGINE_CAP_NODE_ALTER 12
#define ENGINE_CAP_NODE_REMOVE 14

struct XMLDEngine {
 
 /* Basic functions */

 int engine_cap;
 void (*init) (void);
 XMLDBool (*is_valid_mime) (char *);
 XMLDStatus (*prepare) (XMLDFile *, int); 
 void (*cleanup) (XMLDFile *);
 void (*destroy) (void);
 int (*walk) (XMLDFile *);

 /* Data source reading API */
 
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

 /* Iterative labeled node value modification API */
 
 XMLDBool (*set_current_label) (XMLDFile *, char *);
 /* Require alteration capability bit for the next two functions */
 XMLDBool (*set_current_labeled_value_type) (XMLDFile *, char *);
 XMLDBool (*set_current_labeled_value_length) (XMLDFile *, int);
 XMLDBool (*set_current_labeled_value) (XMLDFile *, char *);
 
 /* Data source modification API */

 XMLDBool (*set_labeled_node_value) (XMLDFile *, char *, char *);
 XMLDBool (*set_absolute_node_value) (XMLDFile *, char *);
 XMLDBool (*set_node_name) (XMLDFile *, char *); 

 /* Data source creation API */

 XMLDFile *(*create_source) (void);
 XMLDBool (*create_node) (XMLDFile *, char *);
 XMLDBool (*create_labeled_node_value) (XMLDFile *, char *, char *);

 /* Data source alteration API */

 XMLDBool (*set_labeled_node_value_type) (XMLDFile *, char *);
 XMLDBool (*set_labeled_node_value_length) (XMLDFile *, char *);
 XMLDBool (*set_absolute_node_value_type) (XMLDFile *);
 XMLDBool (*set_absoulte_node_value_length) (XMLDFile *);
 
 /* Data source removal API */

 XMLDBool (*remove_source) (XMLDFile *);
 XMLDBool (*remove_node) (XMLDFile *);
 XMLDBool (*remove_labeled_node_value) (XMLDFile *, char *);
 XMLDBool (*remove_absolute_node_value) (XMLDFile *);
};

#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */

XMLDEngine *XMLDEngine_create(char *);
void XMLDEngine_free(XMLDEngine *);

#endif /* __XMLD_ENGINE_H */
