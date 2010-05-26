/* ltdl.c -- system independent dlopen wrapper

   Copyright (C) 1998, 1999, 2000, 2004, 2005, 2006,
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

#define _GNU_SOURCE 1
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "fakeltdl.h"


/* --- MANIFEST CONSTANTS --- */


/* Standard libltdl search path environment variable name  */
#undef  LTDL_SEARCHPATH_VAR
#define LTDL_SEARCHPATH_VAR	"LTDL_LIBRARY_PATH"

static char * user_search_path;

/* Initialize libltdl. */
int
lt_dlinit (void)
{
  return 0;
}

int
lt_dlexit (void)
{
    return 0;
}



/* Libtool-1.5.x interface for loading a new module named FILENAME.  */
lt_dlhandle
lt_dlopen (const char *filename)
{
  return dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
}


/* If FILENAME has an ARCHIVE_EXT or MODULE_EXT extension, try to
   open the FILENAME as passed.  Otherwise try appending ARCHIVE_EXT,
   and if a file is still not found try again with MODULE_EXT appended
   instead.  */
lt_dlhandle
lt_dlopenext (const char *filename)
{
  lt_dlhandle	handle	= 0;
  handle = lt_dlopen(filename);
  if(handle)
    return handle;
  char *full;
  asprintf(&full, "%s.so", filename);
  handle = lt_dlopen(full);
  free(full);
  return handle;
}


static char *
has_library_ext (const char *filename, const char *so_ext)
{
  char *    ext     = 0;

  ext = strrchr (filename, '.');
  if (ext && !strcmp(ext, so_ext))
    return ext;
  return 0;
}

static int
foreach_dir(const char *path, int (*func) (const char *filename, void *data), void *data)
{
  DIR	*dirp	  = 0;
  int    errors	  = 0;

  dirp = opendir (path);
  if (dirp)
    {
      struct dirent *dp	= 0;

      while ((dp = readdir (dirp)))
	    if (dp->d_name[0] != '.')
	    {
            char *fullname = NULL;
            asprintf(&fullname, "%s/%s", path, dp->d_name);
            char *ext = has_library_ext(fullname, ".so");
            if(ext)
                *ext='\0';
            int rc = func(fullname, data);
            free(fullname);
            if(rc)
                ++errors;
	    }

      closedir (dirp);
    }
    else
        ++errors;

  return errors;
}


static int
foreach_dirinpath(const char *path, int (*func) (const char *filename, void *data), void *data)
{
    int errors = 0;
    char *start;
    char *next;
    char *delim;

    if(!path||!*path)
        return 1;
    char *dup = strdup(path);
    start = dup;
    while(start && *start)
    {
        delim = strchrnul(start, ':');
        if(*delim)
            next = delim + 1;
        else
            next = 0;
        *delim = '\0';
        errors += foreach_dir(start, func, data);
        start = next;
    }
    free(dup);
    return errors;
}


/* Call FUNC for each unique extensionless file in SEARCH_PATH, along
   with DATA.  The filenames passed to FUNC would be suitable for
   passing to lt_dlopenext.  The extensions are stripped so that
   individual modules do not generate several entries (e.g. libfoo.la,
   libfoo.so, libfoo.so.1, libfoo.so.1.0.0).  If SEARCH_PATH is NULL,
   then the same directories that lt_dlopen would search are examined.  */
int
lt_dlforeachfile (const char *search_path,
		  int (*func) (const char *filename, void *data),
		  void *data)
{
    if (search_path)
      /* If a specific path was passed, search only the directories
	 listed in it.  */
        foreach_dirinpath (search_path, func, data);
    if (user_search_path)
      /* Otherwise search the default paths.  */
      foreach_dirinpath (user_search_path, func, data);

    const char *env = getenv(LTDL_SEARCHPATH_VAR);
    if(env)
	    foreach_dirinpath (env, func, data);

    return 0;
}

int
lt_dlclose (lt_dlhandle handle)
{
    return dlclose(handle);
}

void *
lt_dlsym (lt_dlhandle place, const char *symbol)
{
  return dlsym(place, symbol);
}

const char *
lt_dlerror (void)
{
  return dlerror();
}


