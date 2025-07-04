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

#include <dwg/io/dwg/fileheaders/DwgLocalSectionMap_p.h>
#include <dwg/io/dwg/fileheaders/DwgSectionDescriptor_p.h>
#include <dwg/io/dwg/writers/DwgFileHeaderWriterBase_p.h>
#include <map>

namespace dwg {

class ICompressor;
class DwgFileHeaderAC18;
class DwgFileHeaderWriterAC18 : public DwgFileHeaderWriterBase
{
public:
    DwgFileHeaderWriterAC18(std::fstream *stream, Encoding encoding, CadDocument *document);

    int handleSectionOffset() const override;
    void writeFile() override;
    void addSection(const std::string &name, std::iostream *stream, bool isCompressed,
                    int decompsize = 0x7400) override;


protected:
    int fileHeaderSize() const override;
    virtual void craeteLocalSection(DwgSectionDescriptor descriptor, const std::vector<unsigned char> &buffer,
                                    int decompressedSize, unsigned long long offset, int totalSize, bool isCompressed);
    std::stringstream applyCompression(const std::vector<unsigned char> &buffer, int decompressedSize,
                                       unsigned long long offset, int totalSize, bool isCompressed);

private:
    void writeRecords();
    void writeDescriptors();
    void writeFileMetaData();
    void writeFileHeader(std::stringstream *stream);
    void addSection(DwgLocalSectionMap section);
    DwgLocalSectionMap setSeeker(int map, std::stringstream *stream);
    void compressChecksum(DwgLocalSectionMap &section, std::stringstream *stream);
    void writePageHeaderData(const DwgLocalSectionMap &section, std::iostream *stream);
    void writeDataSection(std::iostream *stream, const DwgSectionDescriptor &descriptor, const DwgLocalSectionMap &map,
                          int size);

    std::vector<DwgLocalSectionMap> _localSectionsMaps;
    std::map<std::string, DwgSectionDescriptor> _descriptors;

protected:
    DwgFileHeaderAC18 *_fileHeader;
    ICompressor *_compressor;
};

}// namespace dwg