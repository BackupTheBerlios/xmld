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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xmlddef.h"

/*
 * Composes a message in the form:
 *
 * <argument> <value>
 *
 * arguments are taken from args and values are taken from vals
 * such that each value is associated with the argument with the
 * same index in args. The generated message is never longer than
 * len--arguments that go past it are ignored. This method returns 
 * the composed message.
 */
char *protoimpl_compose_msg(char **args, char **vals, int num, int len) {
}

/*
 * Parses a message in the form above and stores its arguments in the
 * string array args and their values in the corresponding indeces
 * in the vals array, returns the number of arguments processed and 
 * proccesses only a maximum of num arguments. Note that args and
 * vals are pre-allocated and may not be chaned by the function.
 */
int protoimpl_parse_msg(char *msg, const char **args, const char **vals, int num) {
}

/*
 * Reads a sequence of a header + message from to fd - returns the message itself
 * and puts the header status in the location pointed by stat.
 */
char *protoimpl_read_sequence(int fd, XMLDStatus *stat) {
}

/*
 * Writes a sequence of a header + message to fd - returns success/failure and 
 * writes a message whose contents are the of msg and sets the header's status
 * to the provided stat argument.
 */
XMLDStatus protoimpl_write_sequence(int fd, char *msg, XMLDStatus stat) {
}
