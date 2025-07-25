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
#include <dwg/attributes/DxfCodeValueAttribute_p.h>
#include <dwg/attributes/DxfSubClassAttribute_p.h>
#include <dwg/entities/DimensionLinear.h>

namespace dwg {

DimensionLinear::DimensionLinear() : DimensionAligned(DimensionType::Linear) {}

DimensionLinear::~DimensionLinear() {}

ObjectType DimensionLinear::objectType() const
{
    return ObjectType::DIMENSION_LINEAR;
}

std::string DimensionLinear::objectName() const
{
    return DxfFileToken::EntityDimension;
}

std::string DimensionLinear::subclassMarker() const
{
    return DxfSubclassMarker::LinearDimension;
}

double DimensionLinear::rotation() const
{
    return _rotation;
}

void DimensionLinear::setRotation(double value)
{
    _rotation = value;
}

double DimensionLinear::measurement() const
{
    return 0.0;
}

double DimensionLinear::offset() const
{
    return 0.0;
}
void DimensionLinear::setOffset(double value) {}

void DimensionLinear::updateBlock() {}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<DimensionLinear>("DimensionLinear")(metadata("DxfName", DxfFileToken::EntityDimension),
                                                             metadata("DxfSubClass", DxfSubClassAttribute(DxfSubclassMarker::LinearDimension)))
            .constructor()
            .property("rotation", &DimensionLinear::rotation, &DimensionLinear::setRotation)(metadata("DxfCodeValue", DxfCodeValueAttribute(DxfReferenceType::IsAngle, {50})));
}

}// namespace dwg