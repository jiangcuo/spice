/*
  Copyright (C) 2013 Red Hat, Inc.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

typedef struct SpiceMsgMainZeroLen1 {
    uint8_t txt1[5];
    uint8_t sep1;
    uint32_t txt2_len;
    uint8_t *txt2;
    uint8_t *txt3;
    uint32_t n;
    uint16_t txt4_len;
    uint8_t txt4[0];
} SpiceMsgMainZeroLen1;

