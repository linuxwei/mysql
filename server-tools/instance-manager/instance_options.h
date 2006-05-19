#ifndef INCLUDES_MYSQL_INSTANCE_MANAGER_INSTANCE_OPTIONS_H
#define INCLUDES_MYSQL_INSTANCE_MANAGER_INSTANCE_OPTIONS_H
/* Copyright (C) 2004 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <my_global.h>
#include <my_sys.h>

#include "parse.h"
#include "portability.h" /* for pid_t on Win32 */

#if defined(__GNUC__) && defined(USE_PRAGMA_INTERFACE)
#pragma interface
#endif


/*
  This class contains options of an instance and methods to operate them.

  We do not provide this class with the means of synchronization as it is
  supposed that options for instances are all loaded at once during the
  instance_map initilization and we do not change them later. This way we
  don't have to synchronize between threads.
*/

class Instance_options
{
public:
  /* The operation is used to check if the option is IM-specific or not. */
   static bool is_option_im_specific(const char *option_name);

public:
  Instance_options();
  ~Instance_options();
  /* fills in argv */
  int complete_initialization(const char *default_path);

  bool set_option(Named_value *option);
  void unset_option(const char *option_name);

  inline int get_num_options() const;
  inline Named_value get_option(int idx) const;

public:
  int init(const LEX_STRING *instance_name_arg);
  pid_t get_pid();
  int get_pid_filename(char *result);
  int unlink_pidfile();
  void print_argv();

public:
  /*
    We need this value to be greater or equal then FN_REFLEN found in
    my_global.h to use my_load_path()
  */
  enum { MAX_PATH_LEN= 512 };
  enum { MAX_NUMBER_OF_DEFAULT_OPTIONS= 2 };
  char pid_file_with_path[MAX_PATH_LEN];
  char **argv;
  /*
    Here we cache the version string, obtained from mysqld --version.
    In the case when mysqld binary is not found we get NULL here.
  */
  const char *mysqld_version;
  /* We need the some options, so we store them as a separate pointers */
  const char *mysqld_socket;
  const char *mysqld_datadir;
  const char *mysqld_pid_file;
  const char *mysqld_port;
  uint mysqld_port_val;
  LEX_STRING instance_name;
  LEX_STRING mysqld_path;
  const char *nonguarded;
  const char *shutdown_delay;
  uint shutdown_delay_val;
  /* log enums are defined in parse.h */
  char *logs[3];

private:
  int fill_log_options();
  int fill_instance_version();
  int add_to_argv(const char *option);
  int get_default_option(char *result, size_t result_len,
                         const char *option_name);

  void update_var(const char *option_name, const char *option_value);
  int find_option(const char *option_name);

private:
  uint filled_default_options;
  MEM_ROOT alloc;

  Named_value_arr options;
};


inline int Instance_options::get_num_options() const
{
  return options.get_size();
}


inline Named_value Instance_options::get_option(int idx) const
{
  return options.get_element(idx);
}

#endif /* INCLUDES_MYSQL_INSTANCE_MANAGER_INSTANCE_OPTIONS_H */
