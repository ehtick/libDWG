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

#include <dwg/io/CadWriterConfiguration.h>
#include <set>
#include <string>

namespace dwg {

class LIBDWG_API DxfWriterConfiguration : public CadWriterConfiguration
{
public:
    DxfWriterConfiguration();

    ~DxfWriterConfiguration();

    bool writeAllHeaderVariables() const;

    void setWriteAllHeaderVariables(bool value);

    bool writeOptionalValues() const;

    void setWriteOptionalValues(bool value);

    void addHeaderVariable(const std::string &name);

    bool removeHeaderVariable(const std::string &name);

    std::set<std::string> headerVariables() const;

private:
    bool _writeAllHeaderVariables;
    bool _writeOptionalValues;
    std::set<std::string> _headerVariables;
};

}// namespace dwg