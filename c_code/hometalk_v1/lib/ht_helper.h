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

#ifndef HT_HELPER_H
#define HT_HELPER_H

#ifdef __cplusplus	/* to make the lib compatible to Arduino */
extern "C"{
#endif


int ht_readdata(BYTE* frame, const char* filename);

int ht_writedata(hometalkCommand* cmd, const char* filename);

void ht_debug_data(BYTE* frame);

void ht_debug_frame(hometalkCommand* command);

void ht_debug_header(DBYTE header);

const char* ht_byte_to_binary(BYTE x);

UBYTE ht_gray_decode_ubyte(UBYTE n);

UBYTE ht_gray_encode_ubyte(UBYTE n);

BYTE ht_gray_decode_byte(BYTE n);

BYTE ht_gray_encode_byte(BYTE n);

DBYTE crc16(UBYTE *addr, UBYTE num, DBYTE crc);

unsigned int ter2dec(UBYTE *bin, UBYTE len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
