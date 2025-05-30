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

#include <dwg/io/dwg/fileheaders/DwgFileHeader_p.h>
#include <dwg/io/dwg/fileheaders/DwgSectionLocatorRecord_p.h>
#include <map>

namespace dwg {

class DwgFileHeaderAC15 : public DwgFileHeader
{
    std::map<int, DwgSectionLocatorRecord> _records;

public:
    static std::vector<unsigned char> EndSentinel;

    DwgFileHeaderAC15();

    DwgFileHeaderAC15(ACadVersion version);

    virtual ~DwgFileHeaderAC15();

    virtual void addSection(const std::string &name) override;

    DwgSectionDescriptor &getDescriptor(const std::string &name) override;

    std::map<int, DwgSectionLocatorRecord> records() const;
    std::map<int, DwgSectionLocatorRecord> &records();
    void setRcords(const std::map<int, DwgSectionLocatorRecord> &);
};


}// namespace dwg