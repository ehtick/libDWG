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

#include <dwg/CadObject.h>
#include <string>

namespace dwg {

class Entity;
class LIBDWG_API EvaluationExpression : public CadObject
{
public:
    EvaluationExpression();
    virtual ~EvaluationExpression();

    ObjectType objectType() const override;
    std::string subclassMarker() const override;

    int value90() const;
    void setValue90(int value);

    int value98() const;
    void setValue98(int value);

    int value99() const;
    void setValue99(int value);

private:
    int _value90;
    int _value98;
    int _value99;
};

}// namespace dwg