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
#include <dwg/entities/Ellipse.h>

namespace dwg {

Ellipse::Ellipse()
    : _normal(XYZ::AxisZ), _center(XYZ::Zero), _endPoint(XYZ::AxisX), _radiusRatio(0.0), _startParameter(0.0),
      _endParameter(2 * M_PI), _thickness(0.0)
{
}

Ellipse::~Ellipse() {}

ObjectType Ellipse::objectType() const
{
    return ObjectType::ELLIPSE;
}

std::string Ellipse::objectName() const
{
    return DxfFileToken::EntityEllipse;
}

std::string Ellipse::subclassMarker() const
{
    return DxfSubclassMarker::Ellipse;
}

XYZ Ellipse::normal() const
{
    return _normal;
}

XYZ Ellipse::center() const
{
    return _center;
}

XYZ Ellipse::endPoint() const
{
    return _endPoint;
}

double Ellipse::thickness() const
{
    return _thickness;
}

double Ellipse::radiusRatio() const
{
    return _radiusRatio;
}

double Ellipse::startParameter() const
{
    return _startParameter;
}

double Ellipse::endParameter() const
{
    return _endParameter;
}

void Ellipse::setNormal(const XYZ &normal)
{
    _normal = normal;
}

void Ellipse::setCenter(const XYZ &center)
{
    _center = center;
}

void Ellipse::setEndPoint(const XYZ &endPoint)
{
    _endPoint = endPoint;
}

void Ellipse::setThickness(double thickness)
{
    _thickness = thickness;
}

void Ellipse::setRadiusRatio(double radiusRatio)
{
    _radiusRatio = radiusRatio;
}

void Ellipse::setStartParameter(double startParam)
{
    _startParameter = startParam;
}

void Ellipse::setEndParameter(double endParam)
{
    _endParameter = endParam;
}

}// namespace dwg