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
 
#ifndef HAVE_XMLD_PART_H
#define HAVE_XMLD_PART_H

/* A structure representing a part that needs initialization */
struct XMLDPart {
 xmld_status_t (*init_func) (void); /* initialization function */
 xmld_status_t (*shutdown_func) (void); /* shutdown function */
 short ok; /* whether initilization went fine */
};

typedef struct XMLDPart XMLDPart;
void XMLDPart_create(XMLDPart *, xmld_status_t (*) (void), xmld_status_t (*) (void));
xmld_status_t XMLDPart_start(XMLDPart *);
xmld_status_t XMLDPart_shutdown(XMLDPart *);

#endif /* HAVE_XMLD_PART_H */
 