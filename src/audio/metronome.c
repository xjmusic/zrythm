/*
 * Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
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

#include <stdlib.h>

#include "config.h"
#include "audio/encoder.h"
#include "audio/metronome.h"
#include "utils/audio.h"
#include "utils/io.h"
#include "zrythm.h"

#include <gtk/gtk.h>

/**
 * Initializes the Metronome by loading the samples
 * into memory.
 */
void
metronome_init (
  Metronome * self)
{
  /* free if previously initialized */
  if (self->emphasis_path)
    g_free (self->emphasis_path);
  if (self->normal_path)
    g_free (self->normal_path);
  if (self->emphasis)
    free (self->emphasis);
  if (self->normal)
    free (self->normal);

  if (ZRYTHM_TESTING)
    {
      const char * src_root =
        getenv ("G_TEST_SRC_ROOT_DIR");
      g_warn_if_fail (src_root);
      self->emphasis_path =
        g_build_filename (
          src_root, "data", "samples", "klick",
          "square_emphasis.wav", NULL);
      self->normal_path =
        g_build_filename (
          src_root, "data", "samples", "klick",
          "/square_normal.wav", NULL);
    }
  else
    {
#ifdef WINDOWS_RELEASE
      char * install_dir =
        io_get_registry_string_val ("InstallPath");
      self->emphasis_path =
        g_build_filename (
          install_dir, "share", "zrythm", "samples",
          "square_emphasis.wav", NULL);
      self->normal_path =
        g_build_filename (
          install_dir, "share", "zrythm", "samples",
          "square_normal.wav", NULL);
      g_free (install_dir);
#elif defined(MAC_RELEASE)
      char bundle_path[PATH_MAX];
      int ret = io_get_bundle_path (bundle_path);
      g_return_if_fail (ret == 0);
      self->emphasis_path =
        g_strdup_printf (
          "%s/../share/zrythm/samples/"
          "square_emphasis.wav", bundle_path);
      self->normal_path =
        g_strdup_printf (
          "%s/../share/zrythm/samples/"
          "square_normal.wav", bundle_path);
#else
      self->emphasis_path =
        g_build_filename (
          SAMPLES_DIR, "square_emphasis.wav", NULL);
      self->normal_path =
        g_build_filename (
          SAMPLES_DIR, "square_normal.wav", NULL);
#endif

      /* decode */
      AudioEncoder * enc =
        audio_encoder_new_from_file (
          self->emphasis_path);
      if (!enc)
        {
          g_critical (
            "Failed to load samples for metronome "
            "from %s",
            self->emphasis_path);
          return;
        }
      audio_encoder_decode (enc, 0);
      self->emphasis =
        calloc (
          (size_t)
            (enc->num_out_frames * enc->channels),
          sizeof (float));
      self->emphasis_size = enc->num_out_frames;
      self->emphasis_channels = enc->channels;
      g_return_if_fail (enc->channels > 0);
      for (int i = 0;
           i < enc->num_out_frames * enc->channels;
           i++)
        {
          self->emphasis[i] = enc->out_frames[i];
        }
      audio_encoder_free (enc);

      enc =
        audio_encoder_new_from_file (
          self->normal_path);
      audio_encoder_decode (
        enc, 0);
      self->normal =
        calloc (
          (size_t)
            (enc->num_out_frames * enc->channels),
          sizeof (float));
      self->normal_size = enc->num_out_frames;
      self->normal_channels = enc->channels;
      g_return_if_fail (enc->channels > 0);
      for (int i = 0;
           i < enc->num_out_frames * enc->channels;
           i++)
        {
          self->normal[i] = enc->out_frames[i];
        }
      audio_encoder_free (enc);
    }
}

/**
 * Fills the given frame buffer with metronome audio
 * based on the current position.
 *
 * @param buf The frame buffer.
 * @param g_start_frame The global start position in
 *   frames.
 * @param nframes Number of frames to fill. These must
 *   not exceed the buffer size.
 */
/*void*/
/*metronome_fill_buffer (*/
  /*Metronome * self,*/
  /*float *     buf,*/
  /*const long  g_start_frame,*/
  /*const int   nframes)*/
/*{*/
/*}*/
