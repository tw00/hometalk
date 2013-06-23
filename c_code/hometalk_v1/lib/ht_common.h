/*  
    Copyright (C) 2012 Thomas Weustenfeld <thomas.weustenfeld@tu-dortmund.de>

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef HT_COMMON_H
#define HT_COMMON_H

#include <stdio.h>

typedef char BYTE; // IMMER?
typedef unsigned char UBYTE;
typedef unsigned long QUAD_BYTE; // IMMER?
typedef unsigned short DBYTE;

typedef struct 
{
    DBYTE addr;    // little endian
    DBYTE device;  // little endian
    UBYTE cmd[4];
    DBYTE value;
    UBYTE seqnum;
} hometalkCommand;


#define HT_FRAME_LENGTH 16

#endif
