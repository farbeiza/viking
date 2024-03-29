/*
 * viking -- GPS Data and Topo Analyzer, Explorer, and Manager
 *
 * Copyright (C) 2003-2005, Evan Battaglia <gtoevan@gmx.net>
 * Copyright (C) 2012, Guilhem Bonnefille <guilhem.bonnefille@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "viking.h"

#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <glib.h>
#include <glib/gstdio.h>

const gchar *a_get_viking_dir()
{
  static gchar *viking_dir = NULL;

  // TODO: use g_get_user_config_dir ?

  if (!viking_dir) {
    const gchar *home = g_getenv("HOME");
    if (!home || g_access(home, W_OK))
      home = g_get_home_dir ();
#ifdef HAVE_MKDTEMP
    if (!home || g_access(home, W_OK))
    {
      static gchar temp[] = {"/tmp/vikXXXXXX"};
      home = mkdtemp(temp);
    }
#endif
    if (!home || g_access(home, W_OK))
      /* Fatal error */
      g_critical("Unable to find a base directory");

    /* Build the name of the directory */
#ifdef __APPLE__
    viking_dir = g_build_filename(home, "/Library/Application Support/Viking", NULL);
#else
    viking_dir = g_build_filename(home, ".viking", NULL);
#endif
    if (g_file_test(viking_dir, G_FILE_TEST_EXISTS) == FALSE)
      g_mkdir(viking_dir, 0755);
  }

  return viking_dir;
}

/**
 * a_get_viking_data_home:
 * 
 * Retrieves the XDG compliant user's data directory.
 * 
 * Retuns: the directory (can be NULL). Should be freed with g_free.
 */
gchar *
a_get_viking_data_home()
{
	const gchar *xdg_data_home = g_getenv("XDG_DATA_HOME");
	if (xdg_data_home)
	{
		return g_build_filename(xdg_data_home, PACKAGE);
	}
	else
	{
		return NULL;
	}
}

/**
 * a_get_viking_data_path:
 * 
 * Retrieves the configuration path.
 *
 * Returns: list of directories to scan for data. Should be freed with g_strfreev.
 */
gchar **
a_get_viking_data_path()
{
  const gchar *xdg_data_dirs = g_getenv("XDG_DATA_DIRS");
  if (xdg_data_dirs == NULL)
  {
    /* Default value specified in 
     http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
     */
    xdg_data_dirs = "/usr/local/share/:/usr/share/";
  }
  gchar **data_path = g_strsplit(xdg_data_dirs, ":", 0);
  /* Append the viking dir */
  gchar **path;
  for (path = data_path ; *path != NULL ; path++)
  {
    gchar *dir = *path;
    *path = g_build_filename(dir, PACKAGE, NULL);
    g_free(dir);
  }
  return data_path;
}
