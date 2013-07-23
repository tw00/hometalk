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
    CMD  = 0,
    eCMD = 1,
    FLOW = 2,
    PSI  = 3,
    ERROR = 15
} HtFrameType;

typedef enum {
	OK = 0,
	CRC_ERROR = 1,
	POINTER_ERROR = 2
} HtDecodeResult;

typedef enum {
    false = 0,
    true
} BOOL;

#define HT_HEADER_CONTROLFIELD_FRT (1 << 3)  /* frame type */
#define HT_HEADER_CONTROLFIELD_RES (1 << 2)  /* RESERVED = 1 */
#define HT_HEADER_CONTROLFIELD_EXT (1 << 1)  /* Exteded frame bit */
#define HT_HEADER_CONTROLFIELD_ROU (1 << 0)  /* Routing bit */

#define HT_MAGIC_NUMBER 0xAAU

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


typedef struct {
	UBYTE seq:4;
	UBYTE ctrlFrt:2;
	UBYTE ctrlRes:1;
	UBYTE ctrlExt:1;
	UBYTE ctrlRou:1;
} hometalkHeader;

typedef struct 
{
	hometalkHeader header;
    DBYTE addr;
    UBYTE func;
    UBYTE cmd[4];
} hometalkCMD;

typedef struct
{
	hometalkHeader header;
    DBYTE sender;
} hometalkFLOW;


typedef struct 
{
	hometalkHeader header;
    DBYTE addr;
    UBYTE ID:4;
    UBYTE res:4;
    UBYTE payload[8];
} hometalkPSI;

/*typedef struct
{
    DBYTE header;
    QUAD_BYTE addr;
    BYTE  func;
    UBYTE cmd[8];
    DBYTE crc;
} hometalkExCMD;*/





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
