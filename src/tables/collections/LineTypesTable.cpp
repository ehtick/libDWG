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
#include <dwg/tables/LineType.h>
#include <dwg/tables/collections/LineTypesTable.h>

namespace dwg {

LineTypesTable::LineTypesTable() {}

LineTypesTable::LineTypesTable(CadDocument *document) : Table(document) {}

LineTypesTable::~LineTypesTable() {}

ObjectType LineTypesTable::objectType() const
{
    return ObjectType::LTYPE_CONTROL_OBJ;
}

std::string LineTypesTable::objectName() const
{
    return DxfFileToken::TableLinetype;
}

std::vector<std::string> LineTypesTable::defaultEntries() const
{
    return {LineType::ByLayerName, LineType::ByBlockName, LineType::ContinuousName};
}

LineType *LineTypesTable::byLayer() const
{
    return valueT<LineType *>(LineType::ByLayerName);
}

LineType *LineTypesTable::byBlock() const
{
    return valueT<LineType *>(LineType::ByBlockName);
}

LineType *LineTypesTable::continuous() const
{
    return valueT<LineType *>(LineType::ContinuousName);
}

TableEntry *LineTypesTable::createEntry(const std::string &name)
{
    return new LineType(name);
}

}// namespace dwg