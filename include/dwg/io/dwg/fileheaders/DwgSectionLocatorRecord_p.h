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

#include <optional>

namespace dwg {

class DwgSectionLocatorRecord
{
    std::optional<int> _number;
    long long _seeker;
    long long _size;

public:
    DwgSectionLocatorRecord(std::optional<int> number = std::nullopt, long long seeker = 0, long long size = 0);

    bool isInTheRecord(int position) const;

    std::optional<int> number() const;
    void setNumber(int number);

    long long seeker() const;
    void setSeeker(long long seeker);

    long long size() const;
    void setSize(long long size);
};

}// namespace dwg
