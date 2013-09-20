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

#ifndef HT_DECODE_H
#define HT_DECODE_H

#include <string.h>


#ifdef __cplusplus	/* to make the lib compatible to Arduino */
extern "C"{
#endif


BYTE* ht_encode_frame(hometalkCommand* ht_cmd);

hometalkCommand* ht_decode_frame(BYTE* frame, int length);

QUAD_BYTE ht_cmd_as_quad(hometalkCommand* command);

DBYTE ht_enconde_header(HtFrameType type, BOOL isRoutedFrame, long seq);


UBYTE ht_get_frame_length_by_type(UBYTE buffer);
HtFrameType ht_determine_frame_type(circular_buffer* buffer);

BYTE* ht_encode_frame(hometalkCommand* ht_cmd);

UBYTE ht_encode_cmd(hometalkCMD* cmd, circular_buffer* buffer);
UBYTE ht_encode_flow(hometalkFLOW* flow, circular_buffer* buffer);
UBYTE ht_encode_psi(hometalkPSI* psi, circular_buffer* buffer);
HtDecodeResult ht_decode_cmd(hometalkCMD* cmd, circular_buffer* buffer);
HtDecodeResult ht_decode_flow(hometalkFLOW* flow, circular_buffer* buffer);
HtDecodeResult ht_decode_psi(hometalkPSI* psi, circular_buffer* buffer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
