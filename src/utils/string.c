/*
 * Copyright (C) 2018-2020 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string.h>

#include "utils/string.h"

#include <gtk/gtk.h>

#include <pcre.h>

int
string_is_ascii (const char * string)
{
  return g_str_is_ascii (string);
#if 0
  unsigned long i;
  if (!string || strlen (string) == 0)
    return 0;
  for (i = 0; i < strlen (string); i++)
    {
      if (string[i] < 32 ||
          string[i] > 126)
        {
          return 0;
        }
    }
  return 1;
#endif
}

/**
 * Returns the matched string if the string array
 * contains the given substring.
 */
char *
string_array_contains_substr (
  char ** str_array,
  int     num_str,
  const char *  substr)
{
  for (int i = 0; i < num_str; i++)
    {
      if (g_str_match_string (
        substr,
        str_array[i],
        0))
        return str_array[i];
    }

  return NULL;
}

/**
 * Returns if the two strings are equal.
 */
int
string_is_equal (
  const char * str1,
  const char * str2,
  int          ignore_case)
{
  if (ignore_case)
    {
      char * str1_casefolded =
        g_utf8_casefold (
          str1, -1);
      char * str2_casefolded =
        g_utf8_casefold (
          str2, -1);
      int ret =
        !g_strcmp0 (
          str1_casefolded,
          str2_casefolded);
      g_free (str1_casefolded);
      g_free (str2_casefolded);
      return ret;
    }
  else
    {
      return !g_strcmp0 (str1, str2);
    }
}

/**
 * Returns if the given string contains the given
 * substring.
 *
 * @param accept_alternatives Accept ASCII
 *   alternatives.
 */
bool
string_contains_substr (
  const char * str,
  const char * substr,
  const bool   accept_alternatives)
{
  return
    g_str_match_string (
      substr,
      str,
      accept_alternatives);
}

/**
 * Returns a newly allocated string that is a
 * filename version of the given string.
 *
 * Example: "MIDI ZRegion #1" -> "MIDI_Region_1".
 */
char *
string_convert_to_filename (
  const char * str)
{
  /* convert illegal characters to '_' */
  char * new_str = g_strdup (str);
  for (int i = 0; i < (int) strlen (str); i++)
    {
      if (str[i] == '#' ||
          str[i] == '%' ||
          str[i] == '&' ||
          str[i] == '{' ||
          str[i] == '}' ||
          str[i] == '\\' ||
          str[i] == '<' ||
          str[i] == '>' ||
          str[i] == '*' ||
          str[i] == '?' ||
          str[i] == '/' ||
          str[i] == ' ' ||
          str[i] == '$' ||
          str[i] == '!' ||
          str[i] == '\'' ||
          str[i] == '"' ||
          str[i] == ':' ||
          str[i] == '@')
        new_str[i] = '_';
    }
  return new_str;
}

/**
 * Removes any bak, bak1 etc suffixes from the
 * string and returns a newly allocated string.
 */
char *
string_get_substr_before_backup_ext (
  const char * str)
{
  /* get the part without .bak */
  char ** parts =
    g_strsplit (
      str, ".bak", 0);
  char * part = g_strdup (parts[0]);
  g_strfreev (parts);
  return part;
}

char *
string_replace (
  const char * str,
  const char * from,
  const char * to)
{
  char ** split =
    g_strsplit (str, from, -1);
  char * new_str =
    g_strjoinv (to, split);
  g_strfreev (split);
  return new_str;
}

/**
 * Gets the string in the given regex group as an
 * integer.
 *
 * @param def Default.
 *
 * @return The int, or default.
 */
int
string_get_regex_group_as_int (
  const char * str,
  const char * regex,
  int          group,
  int          def)
{
  char * res =
    string_get_regex_group (str, regex, group);
  if (res)
    {
      int res_int = atoi (res);
      g_free (res);
      return res_int;
    }
  else
    return def;
}

/**
 * Gets the string in the given regex group.
 *
 * @return The string, or NULL.
 */
char *
string_get_regex_group (
  const char * str,
  const char * regex,
  int          group)
{
  int OVECCOUNT = 60;
  const char *error;
  int   erroffset;
  pcre *re;
  int   rc;
  int   ovector[OVECCOUNT];

  re =
    pcre_compile (
      /* pattern */
      regex,
      /* options */
      0,
      /* error message */
      &error,
      /* error offset */
      &erroffset,
      /* use default character tables */
      0);

  if (!re)
    {
      g_warning (
        "pcre_compile failed (offset: %d), %s",
        erroffset, error);
      return NULL;
    }

  rc =
    pcre_exec (
      re, /* the compiled pattern */
      0,  /* no extra data - pattern was not studied */
      str, /* the string to match */
      strlen (str), /* the length of the string */
      0,   /* start at offset 0 in the subject */
      0,   /* default options */
      ovector, /* output vector for substring information */
      OVECCOUNT); /* number of elements in the output vector */

  if (rc < 0)
    {
      switch (rc)
        {
        case PCRE_ERROR_NOMATCH:
          /*g_message ("String %s didn't match", str);*/
          break;

        default:
          g_message (
            "Error while matching \"%s\": %d",
            str, rc);
          break;
        }
      free (re);
      return NULL;
    }

#if 0
  for (int i = 0; i < rc; i++)
    {
      g_message (
        "%2d: %.*s", i,
        ovector[2 * i + 1] - ovector[2 * i],
        str + ovector[2 * i]);
    }
#endif

  return
    g_strdup_printf (
      "%.*s",
      ovector[2 * group + 1] - ovector[2 * group],
      str + ovector[2 * group]);
}

/**
 * Returns the integer found at the end of a string
 * like "My String 3" -> 3, or -1 if no number is
 * found.
 *
 * See https://www.debuggex.com/cheatsheet/regex/pcre
 * for more info.
 *
 * @param str_without_num A buffer to save the
 *   string without the number (including the space).
 */
int
string_get_int_after_last_space (
  const char * str,
  char *       str_without_num)
{
  int OVECCOUNT = 60;
  const char *error;
  int   erroffset;
  pcre *re;
  int   rc;
  int   i;
  int   ovector[OVECCOUNT];

  const char *regex = "(.*) ([\\d]+)";

  re =
    pcre_compile (
      /* pattern */
      regex,
      /* options */
      0,
      /* error message */
      &error,
      /* error offset */
      &erroffset,
      /* use default character tables */
      0);

  if (!re)
    {
      g_error (
        "pcre_compile failed (offset: %d), %s",
        erroffset, error);
      return -1;
    }

  rc =
    pcre_exec (
      re, /* the compiled pattern */
      0,  /* no extra data - pattern was not studied */
      str, /* the string to match */
      strlen(str), /* the length of the string */
      0,   /* start at offset 0 in the subject */
      0,   /* default options */
      ovector, /* output vector for substring information */
      OVECCOUNT); /* number of elements in the output vector */

  if (rc < 0)
    {
      switch (rc)
        {
        case PCRE_ERROR_NOMATCH:
          g_message ("String %s didn't match", str);
          break;

        default:
          g_message (
            "Error while matching \"%s\": %d",
            str, rc);
          break;
        }
      free (re);
      return -1;
    }

  for (i = 0; i < rc; i++)
    {
      g_message (
        "%2d: %.*s", i,
        ovector[2 * i + 1] - ovector[2 * i],
        str + ovector[2 * i]);
    }

  if (str_without_num)
    {
      i = rc - 2;
      sprintf (
        str_without_num, "%.*s",
        ovector[2 * i + 1] - ovector[2 * i],
        str + ovector[2 * i]);
    }

  i = rc - 1;
  return atoi (str + ovector[i * 2]);
}
