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

#include <dwg/entities/HatchBoundaryPath.h>
#include <dwg/io/template/CadEntityTemplate_p.h>

namespace dwg {

class Hatch;
class CadBoundaryPathTemplate
{
    HatchBoundaryPath *_path = nullptr;
    std::vector<unsigned long long> _handles;

public:
    CadBoundaryPathTemplate();
    ~CadBoundaryPathTemplate();

    HatchBoundaryPath *path() const;
    void setPath(HatchBoundaryPath *path);

    std::vector<unsigned long long> handles() const;
    std::vector<unsigned long long> &handles();
    void setHandles(const std::vector<unsigned long long> &handles);

    void build(CadDocumentBuilder *builder);
};

class CadHatchTemplate : public CadEntityTemplate
{
    std::vector<CadBoundaryPathTemplate *> _pathTempaltes;
    std::string _hatchPatternName;

public:
    CadHatchTemplate();
    CadHatchTemplate(Hatch *hatch);
    ~CadHatchTemplate();

    std::string hatchPatternName() const;
    void setHatchPatternName(const std::string &name);

    std::vector<CadBoundaryPathTemplate *> pathTempaltes() const;
    std::vector<CadBoundaryPathTemplate *> &pathTempaltes();

    void build(CadDocumentBuilder *builder) override;
};

}// namespace dwg