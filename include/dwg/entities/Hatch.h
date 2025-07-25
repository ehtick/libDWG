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

#include <dwg/Color.h>
#include <dwg/entities/Entity.h>
#include <dwg/entities/HatchPatternType.h>
#include <dwg/entities/HatchStyleType.h>

namespace dwg {

class HatchPattern;
class HatchBoundaryPath;
class HatchGradientPattern;
class LIBDWG_API Hatch : public Entity
{
    RTTR_ENABLE(Entity)
    
public:
    Hatch();
    ~Hatch();

    virtual ObjectType objectType() const override;
    virtual std::string objectName() const override;
    virtual std::string subclassMarker() const override;

    double elevation() const;
    void setElevation(double elevation);

    XYZ normal() const;
    void setNormal(const XYZ &);

    HatchPattern *pattern() const;
    void setPattern(HatchPattern *);

    bool isSolid() const;
    void setIsSolid(bool);

    bool isAssociative() const;
    void setIsAssociative(bool);

    HatchStyleType style() const;
    void setStyle(HatchStyleType);

    HatchPatternType patternType() const;
    void setPatternType(HatchPatternType);

    double patternAngle() const;
    void setPatternAngle(double);

    double patternScale() const;
    void setPatternScale(double);

    bool isDouble() const;
    void setIsDouble(bool);

    double pixelSize() const;
    void setPixelSize(double);

    std::vector<XY> seedPoints() const;
    void setSeedPoints(const std::vector<XY> &);

    HatchGradientPattern *gradientColor() const;
    void setGradientColor(HatchGradientPattern *);

    std::vector<HatchBoundaryPath *> paths() const;
    void setPaths(const std::vector<HatchBoundaryPath *> &);

private:
    double _elevation;
    XYZ _normal;
    HatchPattern *_pattern;
    bool _isSolid;
    bool _isAssociative;
    HatchStyleType _style;
    HatchPatternType _patternType;
    double _patternAngle;
    double _patternScale;
    bool _isDouble;
    double _pixelSize;
    std::vector<XY> _seedPoints;
    HatchGradientPattern *_gradientColor;
    std::vector<HatchBoundaryPath *> _paths;
};

}// namespace dwg
