/*
 * utils/ui.c - GTK/GDK utils
 *
 * Copyright (C) 2018 Alexandros Theodotou
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "utils/ui.h"

void
ui_set_cursor (GtkWidget * widget, char * name)
{
  GdkWindow * win = gtk_widget_get_parent_window (widget);
  GdkCursor * cursor = gdk_cursor_new_from_name (
                                gdk_display_get_default (),
                                name);
  gdk_window_set_cursor(win, cursor);
}

/**
 * Shows error popup.
 */
void
ui_show_error_message (GtkWindow * parent_window,
                       const char * message)
{
  GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  GtkWidget * dialog = gtk_message_dialog_new (parent_window,
                                   flags,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_CLOSE,
                                   message);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
