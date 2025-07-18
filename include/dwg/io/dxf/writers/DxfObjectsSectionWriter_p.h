
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

#include <dwg/io/dxf/writers/DxfSectionWriterBase_p.h>

namespace dwg {

class CadObject;
class BookColor;
class CadDictionary;
class DictionaryVariable;
class PlotSettings;
class Scale;
class GeoData;
class Group;
class ImageDefinition;
class Layout;
class MLineStyle;
class MultiLeaderStyle;
class SortEntitiesTable;
class XRecord;
class DxfObjectsSectionWriter : public DxfSectionWriterBase
{
public:
    DxfObjectsSectionWriter(IDxfStreamWriter *writer, CadDocument *document, CadObjectHolder *objectHolder,
                            const DxfWriterConfiguration &configuration);
    ~DxfObjectsSectionWriter();

    std::string sectionName() const override;
    void writeSection() override;

protected:
    void writeObject(CadObject *co);
    void writeBookColor(BookColor *color);
    void writeDictionary(CadDictionary *e);
    void writeDictionaryVariable(DictionaryVariable *dictvar);
    void writePlotSettings(PlotSettings *plot);
    void writeScale(Scale *scale);
    void writeGeoData(GeoData *geodata);
    void writeGroup(Group *group);
    void writeImageDefinition(ImageDefinition *definition);
    void writeLayout(Layout *layout);
    void writeMLineStyle(MLineStyle *style);
    void writeMultiLeaderStyle(MultiLeaderStyle *style);
    void writeSortentsTable(SortEntitiesTable *e);
    void writeXRecord(XRecord *e);
};

}// namespace dwg