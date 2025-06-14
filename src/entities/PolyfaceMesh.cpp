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

#include <dwg/DxfFileToken_p.h>
#include <dwg/DxfSubclassMarker_p.h>
#include <dwg/entities/PolyfaceMesh.h>
#include <dwg/entities/collection/VertexFaceRecordCollection.h>

namespace dwg {

PolyfaceMesh::PolyfaceMesh() {}

PolyfaceMesh::~PolyfaceMesh() {}

ObjectType PolyfaceMesh::objectType() const
{
    return ObjectType::POLYLINE_PFACE;
}

std::string PolyfaceMesh::objectName() const
{
    return DxfFileToken::EntityPolyline;
}

std::string PolyfaceMesh::subclassMarker() const
{
    return DxfSubclassMarker::PolyfaceMesh;
}

VertexFaceRecordCollection *PolyfaceMesh::faces()
{
    return _faces;
}

void PolyfaceMesh::assignDocument(CadDocument *doc) {}

void PolyfaceMesh::unassignDocument() {}

}// namespace dwg