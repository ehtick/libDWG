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

#include <dwg/objects/NonGraphicalObject.h>
#include <map>

namespace dwg {

class Entity;
class CadDictionary;

class LIBDWG_API Group : public NonGraphicalObject
{
public:
    Group();
    ~Group();

    ObjectType objectType() const override;
    std::string objectName() const override;
    std::string subclassMarker() const override;

    std::string description() const;
    void setDescription(const std::string &);

    bool isUnnamed() const;
    void setIsUnnamed(bool);

    bool selectable() const;
    void setSelectable(bool value);

    std::vector<Entity *> entities() const;
    std::vector<Entity *> &entities();

    void assignDocument(CadDocument *doc) override;
    void unassignDocument() override;

private:
    std::string _description;
    bool _isUnnamed;
    bool _selectable;
    std::vector<Entity *> _entities;
};

}// namespace dwg