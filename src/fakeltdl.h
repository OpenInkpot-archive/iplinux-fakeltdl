/* ltdl.h -- generic dlopen functions

   Copyright (C) 1998-2000, 2004, 2005,
                 2007, 2008 Free Software Foundation, Inc.
   Written by Thomas Tanner, 1998

   NOTE: The canonical source of this file is maintained with the
   GNU Libtool package.  Report bugs to bug-libtool@gnu.org.

GNU Libltdl is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

As a special exception to the GNU Lesser General Public License,
if you distribute this file as part of a program or library that
is built using GNU Libtool, you may include this file under the
same distribution terms that you use for the rest of that program.

GNU Libltdl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with GNU Libltdl; see the file COPYING.LIB.  If not, a
copy can be downloaded from  http://www.gnu.org/licenses/lgpl.html,
or obtained by writing to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/* Only include this header file once. */
#if !defined(FAKELTDL_H)
#define FAKELTDL_H 1

#if defined(__cplusplus)
extern "C" {
#endif

/* LT_STRLEN can be used safely on NULL pointers.  */
#define LT_STRLEN(s)	(((s) && (s)[0]) ? strlen (s) : 0)

#define LT_SCOPE /* export */


/* --- DYNAMIC MODULE LOADING API --- */


typedef	void *lt_dlhandle;	/* A loaded module.  */

/* Initialisation and finalisation functions for libltdl. */
LT_SCOPE int	    lt_dlinit		(void);
LT_SCOPE int	    lt_dlexit		(void);

/* User module loading advisors.  */
/*
LT_SCOPE int	    lt_dladvise_init	 (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_destroy  (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_ext	 (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_resident (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_local	 (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_global   (lt_dladvise *advise);
LT_SCOPE int	    lt_dladvise_preload	 (lt_dladvise *advise);
*/
/* Portable libltdl versions of the system dlopen() API. */
LT_SCOPE lt_dlhandle lt_dlopen		(const char *filename);
LT_SCOPE lt_dlhandle lt_dlopenext	(const char *filename);
/*
LT_SCOPE lt_dlhandle lt_dlopenadvise	(const char *filename,
					 lt_dladvise advise);
*/
LT_SCOPE void *	    lt_dlsym		(lt_dlhandle handle, const char *name);
LT_SCOPE const char *lt_dlerror		(void);
LT_SCOPE int	    lt_dlclose		(lt_dlhandle handle);

LT_SCOPE int        lt_dlforeachfile(
            const char *search_path,
            int (*func) (const char *filename, void *data),
            void *data);



#define lt_ptr void *

#if defined(__cplusplus)
}
#endif

#endif /*!defined(LTDL_H)*/
