/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
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

typedef void Source;

struct Engine {
 
 /* Basic functions */

 CfgSection *cfg;
 int modcap;
 char *error;
 Status (*init) (void);
 Bool (*is_valid_mime) (char *);
 Status (*prepare) (Source *, int); 
 Status (*cleanup) (Source *);
 Status (*destroy) (void);
 int (*walk) (Source *);

 /* Data source reading API */
 
 char *(*get_labeled_node_value_type) (Source *, char *);
 int (*get_labeled_node_value_length) (Source *, char *);
 char *(*get_labeled_node_value) (Source *, char *);
 char *(*get_absolute_node_value_type) (Source *);
 int (*get_absoulte_node_value_length) (Source *);
 char *(*get_absolute_node_value) (Source *);
 char *(*get_node_name) (Source *);

 /* Iterative labeled node value selection API */
 
 Status (*reset_node) (Source *);
 Status (*next_label) (Source *);
 char *(*get_current_label) (Source *);
 char *(*get_current_labeled_value_type) (Source *);
 int (*get_current_labeled_value_length) (Source *);
 char *(*get_current_labeled_value) (Source *);

 /* Iterative labeled node value modification API */
 
 Status (*set_current_label) (Source *, char *);
 /* Require alteration capability bit for the next two functions */
 Status (*set_current_labeled_value_type) (Source *, char *);
 Status (*set_current_labeled_value_length) (Source *, int);
 Status (*set_current_labeled_value) (Source *, char *);
 
 /* Data source modification API */

 Status (*set_labeled_node_value) (Source *, char *, char *);
 Status (*set_absolute_node_value) (Source *, char *);
 Status (*set_node_name) (Source *, char *); 

 /* Data source creation API */

 Source *(*create_source) (char *);
 Status (*create_node) (Source *, char *);
 Status (*create_labeled_node_value) (Source *, char *, char *);

 /* Data source alteration API */

 Status (*set_labeled_node_value_type) (Source *, char *);
 Status (*set_labeled_node_value_length) (Source *, char *);
 Status (*set_absolute_node_value_type) (Source *);
 Status (*set_absoulte_node_value_length) (Source *);
 
 /* Data source removal API */

 Status (*free_source) (Source *);
 Status (*remove_node) (Source *);
 Status (*remove_labeled_node_value) (Source *, char *);
 Status (*remove_absolute_node_value) (Source *);

 /* Error reporting API */

 Error *(*get_error) (Source *);
};

typedef struct Engine Engine;

#endif /* __ENGINE_H */
