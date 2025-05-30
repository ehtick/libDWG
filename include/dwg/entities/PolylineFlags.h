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

/// Defines the polyline flags.
enum class PolylineFlag
{
    /// Default, open polyline.
    Default = 0,
    /// This is a closed polyline (or a polygon mesh closed in the M direction).
    ClosedPolylineOrClosedPolygonMeshInM = 1,
    /// Curve-fit vertexes have been added.
    CurveFit = 2,
    /// Spline-fit vertexes have been added.
    SplineFit = 4,
    /// This is a 3D polyline.
    Polyline3D = 8,
    /// This is a 3D polygon mesh.
    PolygonMesh = 16,
    /// The polygon mesh is closed in the N direction.
    ClosedPolygonMeshInN = 32,
    /// The polyline is a polyface mesh.
    PolyfaceMesh = 64,
    /// The line type pattern is generated continuously around the vertexes of this polyline.
    ContinuousLinetypePattern = 128
};
Q_DECLARE_FLAGS(PolylineFlags, PolylineFlag);

}// namespace dwg