# Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
#
# This file is part of Zrythm
#
# Zrythm is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Zrythm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.

collect_translatables = run_target (
  'collect-translatables',
  command: [
    'collect_translatables.sh',
    ],
  )

# run stoat
if (stoat.found() and
  meson.get_compiler('c').get_id() == 'clang' and
  meson.get_compiler('cpp').get_id() == 'clang')
run_target (
  'run_stoat',
  command: [
    'run_stoat.sh',
    ],
  depends: [
    zrythm_lib,
    ]
  )
endif
