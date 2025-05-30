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

#include <dwg/io/template/CadTemplate_p.h>
#include <dwg/tables/Layer.h>

namespace dwg {

class CadLayerTemplate : public CadTemplateT<Layer *>
{
public:
    CadLayerTemplate();
    CadLayerTemplate(Layer *l);
    void build(CadDocumentBuilder *builder) override;

    unsigned long long layerControlHandle() const;
    void setLayerControlHandle(unsigned long long);

    unsigned long long plotStyleHandle() const;
    void setPlotStyleHandle(unsigned long long);

    unsigned long long materialHandle() const;
    void setMaterialHandle(unsigned long long);

    std::optional<unsigned long long> lineTypeHandle() const;
    void setLineTypeHandle(unsigned long long);

    std::string lineTypeName() const;
    void setLineTypeName(const std::string &);

    std::string trueColorName() const;
    void setTrueColorName(const std::string &);

private:
    unsigned long long _layerControlHandle;
    unsigned long long _plotStyleHandle;
    unsigned long long _materialHandle;
    std::optional<unsigned long long> _lineTypeHandle;
    std::string _lineTypeName;
    std::string _trueColorName;
};

}// namespace dwg