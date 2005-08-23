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
 
#ifndef __ENGINE_H
#define __ENGINE_H

/* Access level bitmask */
#define ACCESS_READ  0  /* Read Access                  */
#define ACCESS_WRITE 1  /* Write Access                 */
#define ACCESS_ALTER 2  /* Alteration of Document Shape */

/* walk return values */
#define WALK_UP -1  /* One level shallower */
#define WALK_END 0  /* End of document     */
#define WALK_DOWN 1 /* One level deeper    */

/* Engine Capabilities Bitmask */
#define ENGINE_CAP_MULTI_LEVEL 1
#define ENGINE_CAP_NODE_NAME 2
#define ENGINE_CAP_NODE_ABSOLUTE_VALUE 4
#define ENGINE_CAP_LABEL_ITERATION 6
#define ENGINE_CAP_NODE_MODIFY 8
#define ENGINE_CAP_NODE_CREATE 10
#define ENGINE_CAP_NODE_ALTER 12
#define ENGINE_CAP_NODE_REMOVE 14
#define ENGINE_CAP_ERROR_REPORTING 16

struct Engine {
 
 /* Basic functions */

 int engine_cap;
 char *error;
 Status (*init) (void);
 Bool (*is_valid_mime) (char *);
 Status (*prepare) (File *, int); 
 Status (*cleanup) (File *);
 Status (*destroy) (void);
 int (*walk) (File *);

 /* Data source reading API */
 
 char *(*get_labeled_node_value_type) (File *, char *);
 int (*get_labeled_node_value_length) (File *, char *);
 char *(*get_labeled_node_value) (File *, char *);
 char *(*get_absolute_node_value_type) (File *);
 int (*get_absoulte_node_value_length) (File *);
 char *(*get_absolute_node_value) (File *);
 char *(*get_node_name) (File *);

 /* Iterative labeled node value selection API */
 
 Bool (*reset_node) (File *);
 Bool (*next_label) (File *);
 char *(*get_current_label) (File *);
 char *(*get_current_labeled_value_type) (File *);
 int (*get_current_labeled_value_length) (File *);
 char *(*get_current_labeled_value) (File *);

 /* Iterative labeled node value modification API */
 
 Bool (*set_current_label) (File *, char *);
 /* Require alteration capability bit for the next two functions */
 Bool (*set_current_labeled_value_type) (File *, char *);
 Bool (*set_current_labeled_value_length) (File *, int);
 Bool (*set_current_labeled_value) (File *, char *);
 
 /* Data source modification API */

 Bool (*set_labeled_node_value) (File *, char *, char *);
 Bool (*set_absolute_node_value) (File *, char *);
 Bool (*set_node_name) (File *, char *); 

 /* Data source creation API */

 File *(*create_source) (void);
 Bool (*create_node) (File *, char *);
 Bool (*create_labeled_node_value) (File *, char *, char *);

 /* Data source alteration API */

 Bool (*set_labeled_node_value_type) (File *, char *);
 Bool (*set_labeled_node_value_length) (File *, char *);
 Bool (*set_absolute_node_value_type) (File *);
 Bool (*set_absoulte_node_value_length) (File *);
 
 /* Data source removal API */

 Bool (*remove_source) (File *);
 Bool (*remove_node) (File *);
 Bool (*remove_labeled_node_value) (File *, char *);
 Bool (*remove_absolute_node_value) (File *);

 /* Error reporting API */

 char *(*get_error_message) (void);
};

#ifndef ENGINE_TYPE_DEFINED
#define ENGINE_TYPE_DEFINED
 typedef struct Engine Engine;
#endif /* ENGINE_TYPE_DEFINED */

Engine *Engine_create(void);
void Engine_free(Engine *);

#endif /* __ENGINE_H */
