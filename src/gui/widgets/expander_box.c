/*
 * Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
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

#include "gui/widgets/expander_box.h"
#include "utils/resources.h"

#include <gtk/gtk.h>

G_DEFINE_TYPE_WITH_PRIVATE (
  ExpanderBoxWidget,
  expander_box_widget,
  GTK_TYPE_BOX)

#define GET_PRIVATE(s) \
  ExpanderBoxWidgetPrivate * prv = \
     expander_box_widget_get_private (s);

void
on_clicked (
  GtkButton *button,
  ExpanderBoxWidget * self)
{
  GET_PRIVATE (self);

  gtk_revealer_set_reveal_child (
    prv->revealer,
    !gtk_revealer_get_reveal_child (prv->revealer));
}

/**
 * Gets the private.
 */
ExpanderBoxWidgetPrivate *
expander_box_widget_get_private (
  ExpanderBoxWidget * self)
{
  return expander_box_widget_get_instance_private (
    self);
}

/**
 * Sets the label to show.
 */
void
expander_box_widget_set_label (
  ExpanderBoxWidget * self,
  const char *        label)
{
  GET_PRIVATE (self);

  gtk_label_set_text (
    prv->btn_label, label);
}

/**
 * Sets the icon name to show.
 */
void
expander_box_widget_set_icon_name (
  ExpanderBoxWidget * self,
  const char *        icon_name)
{
  GET_PRIVATE (self);

  gtk_image_set_from_icon_name (
    prv->btn_img,
    icon_name,
    GTK_ICON_SIZE_BUTTON);
}

static void
expander_box_widget_class_init (
  ExpanderBoxWidgetClass * _klass)
{
  GtkWidgetClass * klass = GTK_WIDGET_CLASS (_klass);
  resources_set_class_template (
    klass, "expander_box.ui");
  gtk_widget_class_set_css_name (
    klass, "expander-box");

  gtk_widget_class_bind_template_child_private (
    klass,
    ExpanderBoxWidget,
    button);
  gtk_widget_class_bind_template_child_private (
    klass,
    ExpanderBoxWidget,
    revealer);
  gtk_widget_class_bind_template_child_private (
    klass,
    ExpanderBoxWidget,
    content);
}

static void
expander_box_widget_init (ExpanderBoxWidget * self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  GET_PRIVATE (self);

  prv->btn_label =
    GTK_LABEL (gtk_label_new ("Label"));
  gtk_widget_set_hexpand (
    GTK_WIDGET (prv->btn_label), 1);
  gtk_widget_set_halign (
    GTK_WIDGET (prv->btn_label),
    GTK_ALIGN_START);
  prv->btn_img =
    GTK_IMAGE (
      gtk_image_new_from_icon_name (
        "z-plugins", GTK_ICON_SIZE_BUTTON));
  GtkWidget * box =
    gtk_box_new (GTK_ORIENTATION_HORIZONTAL,
                 2);
  gtk_container_add (
    GTK_CONTAINER (box),
    GTK_WIDGET (prv->btn_label));
  gtk_container_add (
    GTK_CONTAINER (box),
    GTK_WIDGET (
      gtk_separator_new (GTK_ORIENTATION_VERTICAL)));
  gtk_container_add (
    GTK_CONTAINER (box),
    GTK_WIDGET (prv->btn_img));
  gtk_container_add (
    GTK_CONTAINER (prv->button),
    GTK_WIDGET (box));

  gtk_widget_show_all (GTK_WIDGET (self));

  g_signal_connect (
    G_OBJECT (prv->button), "clicked",
    G_CALLBACK (on_clicked), self);
}