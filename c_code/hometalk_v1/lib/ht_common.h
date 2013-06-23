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

typedef enum { 
    CMD,
    eCMD,
    FLOW,
    PSI
} HtFrameType;

typedef enum {
    false = 0,
    true
} BOOL;

#define HT_HEADER_CONTROLFIELD_FRT (1 << 0)  /* frame type */
#define HT_HEADER_CONTROLFIELD_RES (1 << 1)  /* RESERVED = 1 */
#define HT_HEADER_CONTROLFIELD_EXT (1 << 2)  /* Exteded frame bit */
#define HT_HEADER_CONTROLFIELD_ROU (1 << 3)  /* Routing bit */

typedef char HBYTE; // IMMER?
typedef char BYTE; // IMMER?
typedef unsigned char UBYTE;
typedef unsigned long QUAD_BYTE; // IMMER?
typedef unsigned short DBYTE;

#define HT_HEADER_MAGIC_BYTE 0xAB

#define HT_CMD_FRAME_LENGTH 11
#define HT_EXCMD_FRAME_LENGTH 17
#define HT_FLOW_FRAME_LENGTH 6
#define HT_PSI_FRAME_LENGTH 6

/*typedef struct 
{
    DBYTE header;
    DBYTE routing_addr;    
    BYTE  func;
    UBYTE cmd[4];
    DBYTE crc;
} hometalkCMDwithRouting;*/

typedef struct 
{
    DBYTE header;
    BYTE  addr;    
    BYTE  func;
    UBYTE cmd[4];
    DBYTE crc;
} hometalkCMD;

/*typedef struct 
{
    DBYTE header;
    QUAD_BYTE addr;    
    BYTE  func;
    UBYTE cmd[8];
    DBYTE crc;
} hometalkExCMD;*/

typedef struct 
{
    DBYTE header;
    BYTE addr;    
    UBYTE payload[8];
    DBYTE crc;
} hometalkPSI;

typedef struct 
{
    DBYTE header;
    BYTE sender;    
    DBYTE crc;
} hometalkFLOW;


/*************** OLD ************************/
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
