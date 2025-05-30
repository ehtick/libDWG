/**
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

#include <dwg/utils/QFlags.h>

namespace dwg {

/// Flags (bit-coded).
enum class MLineStyleFlag
{
    /// None
    None = 0,
    /// Fill on
    FillOn = 1,
    /// Display miters at the joints (inner vertexes)
    DisplayJoints = 2,
    /// Start square (line) cap
    StartSquareCap = 16,
    /// Start inner arcs cap
    StartInnerArcsCap = 32,
    /// Start round (outer arcs) cap
    StartRoundCap = 64,
    /// End square (line) cap
    EndSquareCap = 256,
    /// End inner arcs cap
    EndInnerArcsCap = 512,
    /// End round (outer arcs) cap
    EndRoundCap = 1024
};
Q_DECLARE_FLAGS(MLineStyleFlags, MLineStyleFlag);

}// namespace dwg