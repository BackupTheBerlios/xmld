
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

#ifndef __EXECUTOR_H
#define __EXECUTOR_H

typedef void ParseTree;
typedef void ExecutorResult;

struct Executor {
 CfgSection *cfg;
 int modcap;

 ExecutorResult *(*parse) (char *);
 ParseTree *(*get_parse_tree) (ExecutorResult *);
 
 ExecutorResult *(*walk) (ParseTree *);
 void *(*get_output) (ExecutorResult *);
 
 void (*destroy_parse_tree) (ParseTree *); 
 void (*destroy) (ExecutorResult *);
 Error *(*get_error) (ExecutorResult *);
}

#endif /* __EXECUTOR_H */
