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
#include "mutils.h"
#include "sutils.h"
#include "dutils.h"
#include "cfg.h"
#include "xmld_list.h"
#include "xmld_connection.h"
struct XMLDFile;
#ifndef XMLDFILE_TYPE_DEFINED
#define XMLDFILE_TYPE_DEFINED
 typedef struct XMLDFile XMLDFile;
 typedef XMLDList XMLDFileList;
#endif /* XMLDFILE_TYPE_DEFINED */
struct XMLDInterface;
#ifndef XMLDINTERFACE_TYPE_DEFINED
#define XMLDINTERFACE_TYPE_DEFINED
 typedef struct XMLDInterface XMLDInterface;
 typedef XMLDList XMLDInterfaceList;
#endif /* XMLDINTERFACE_TYPE_DEFINED */
#include "xmld_work.h"
#include "xmld_interface.h"
#include "xmld_engine.h"
#include "xmld_file.h"
#include "engine_list.h"
#include "interface_list.h"
#include "protoimpl.h"
#include "authman.h"
