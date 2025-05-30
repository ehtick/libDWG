﻿/**
 * libDWG - A C++ library for reading and writing DWG and DXF files in CAD.
 *
 * This file is part of libDWG.
 *
 * libDWG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libDWG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * For more information, visit the project's homepage or contact the author.
 */

#pragma once

namespace dwg {

/// Some object types have fixed values, this enum contains the values for these objects.
enum class ObjectType : short
{
    UNLISTED = -999,
    INVALID = -1,
    UNDEFINED = 0,

    TEXT = 1,
    ATTRIB = 2,
    ATTDEF = 3,
    BLOCK = 4,
    ENDBLK = 5,
    SEQEND = 6,
    INSERT = 7,
    MINSERT = 8,

    UNKNOW_9 = 9,

    VERTEX_2D = 0x0A,
    VERTEX_3D = 0x0B,
    VERTEX_MESH = 0x0C,
    VERTEX_PFACE = 0x0D,
    VERTEX_PFACE_FACE = 0x0E,
    POLYLINE_2D = 0x0F,
    POLYLINE_3D = 0x10,
    ARC = 0x11,
    CIRCLE = 0x12,
    LINE = 0x13,
    DIMENSION_ORDINATE = 0x14,
    DIMENSION_LINEAR = 0x15,
    DIMENSION_ALIGNED = 0x16,
    DIMENSION_ANG_3_Pt = 0x17,
    DIMENSION_ANG_2_Ln = 0x18,
    DIMENSION_RADIUS = 0x19,
    DIMENSION_DIAMETER = 0x1A,
    POINT = 0x1B,
    FACE3D = 0x1C,
    POLYLINE_PFACE = 0x1D,
    POLYLINE_MESH = 0x1E,
    SOLID = 0x1F,
    TRACE = 0x20,
    SHAPE = 0x21,
    VIEWPORT = 0x22,
    ELLIPSE = 0x23,
    SPLINE = 0x24,
    REGION = 0x25,
    SOLID3D = 0x26,
    BODY = 0x27,
    RAY = 0x28,
    XLINE = 0x29,
    DICTIONARY = 0x2A,
    OLEFRAME = 0x2B,
    MTEXT = 0x2C,
    LEADER = 0x2D,
    TOLERANCE = 0x2E,
    MLINE = 0x2F,
    BLOCK_CONTROL_OBJ = 0x30,
    BLOCK_HEADER = 0x31,
    LAYER_CONTROL_OBJ = 0x32,
    LAYER = 0x33,
    STYLE_CONTROL_OBJ = 0x34,
    STYLE = 0x35,

    UNKNOW_36 = 0x36,
    UNKNOW_37 = 0x37,

    LTYPE_CONTROL_OBJ = 0x38,
    LTYPE = 0x39,

    UNKNOW_3A = 0x3A,
    UNKNOW_3B = 0x3B,

    VIEW_CONTROL_OBJ = 0x3C,
    VIEW = 0x3D,
    UCS_CONTROL_OBJ = 0x3E,
    UCS = 0x3F,
    VPORT_CONTROL_OBJ = 0x40,
    VPORT = 0x41,
    APPID_CONTROL_OBJ = 0x42,
    APPID = 0x43,
    DIMSTYLE_CONTROL_OBJ = 0x44,
    DIMSTYLE = 0x45,
    VP_ENT_HDR_CTRL_OBJ = 0x46,
    VP_ENT_HDR = 0x47,
    GROUP = 0x48,
    MLINESTYLE = 0x49,
    OLE2FRAME = 0x4A,
    DUMMY = 0x4B,
    LONG_TRANSACTION = 0x4C,
    LWPOLYLINE = 0x4D,
    HATCH = 0x4E,
    XRECORD = 0x4F,
    ACDBPLACEHOLDER = 0x50,
    VBA_PROJECT = 0x51,
    LAYOUT = 0x52,
    ACAD_PROXY_ENTITY = 0x1f2,
    ACAD_PROXY_OBJECT = 0x1f3,
};

}// namespace dwg