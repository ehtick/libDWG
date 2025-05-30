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

#include <dwg/entities/Entity.h>

namespace dwg {

class TextStyle;

class LIBDWG_API Shape : public Entity
{
public:
    Shape();
    ~Shape();

    virtual ObjectType objectType() const override;
    virtual std::string objectName() const override;
    virtual std::string subclassMarker() const override;

    double thickness() const;
    void setThickness(double);

    XYZ insertionPoint() const;
    void setInsertionPoint(const XYZ &);

    double size() const;
    void setSize(double);

    TextStyle *shapeStyle() const;
    void setShapeStyle(TextStyle *);

    double rotation() const;
    void setRotation(double);

    double relativeXScale() const;
    void setRelativeXScale(double);

    double obliqueAngle() const;
    void setObliqueAngle(double);

    XYZ normal() const;
    void setNormal(const XYZ &);

    unsigned short shapeIndex() const;
    void setShapeIndex(unsigned short);

private:
    double _thickness = 0.0;
    XYZ _insertionPoint = XYZ::Zero;
    double _size = 1.0;
    TextStyle *_shapeStyle = nullptr;
    double _rotation = 0;
    double _relativeXScale = 1;
    double _obliqueAngle = 0;
    XYZ _normal = XYZ::AxisZ;
    unsigned short _shapeIndex = 0;
};

}// namespace dwg