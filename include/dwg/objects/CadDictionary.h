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

#include <dwg/IObservableCadCollection.h>
#include <dwg/objects/DictionaryCloningFlags.h>
#include <dwg/objects/NonGraphicalObject.h>
#include <map>
#include <string>
#include <vector>

namespace dwg {

class LIBDWG_API CadDictionary : public NonGraphicalObject, public IObservableCadCollection
{
    struct StringComparerOrdinalIgnoreCase
    {
        bool operator()(const std::string &lhs, const std::string &rhs) const;
    };

    std::map<std::string, NonGraphicalObject *, StringComparerOrdinalIgnoreCase> _entries;
    bool _hardOwnerFlag;
    DictionaryCloningFlags _clonningFlags;

public:
    static std::string Root;
    static std::string AcadColor;
    static std::string AcadGroup;
    static std::string AcadLayout;
    static std::string AcadMaterial;
    static std::string AcadSortEnts;
    static std::string AcadMLeaderStyle;
    static std::string AcadMLineStyle;
    static std::string AcadTableStyle;
    static std::string AcadPlotSettings;
    static std::string VariableDictionary;
    static std::string AcadPlotStyleName;
    static std::string AcadScaleList;
    static std::string AcadVisualStyle;
    static std::string AcadFieldList;
    static std::string AcadImageDict;

    static std::string GeographicData;

public:
    static CadDictionary *CreateRoot();
    static void CreateDefaultEntries(CadDictionary *root);

    CadDictionary();
    CadDictionary(const std::string &name);
    virtual ~CadDictionary();

    ObjectType objectType() const override;
    std::string objectName() const override;
    std::string subclassMarker() const override;

    bool hardOwnerFlag() const;
    void setHardOwnerFlag(bool);

    DictionaryCloningFlags clonningFlags() const;
    void setClonningFlags(DictionaryCloningFlags);

    CadObject *operator[](const std::string &key) const;
    bool tryGetEntry(const std::string &name, NonGraphicalObject **entry);

    void add(const std::string &key, NonGraphicalObject *value);
    void add(NonGraphicalObject *value);
    bool tryAdd(NonGraphicalObject *value) const;

    bool containsKey(const std::string &key) const;
    bool remove(const std::string &key, NonGraphicalObject **item);
    bool remove(const std::string &key);
    void clear();

    template<typename T>
    bool tryGetEntryT(const std::string &name, T **value)
    {
        auto it = _entries.find(name);
        if (it != _entries.end())
        {
            *value = dynamic_cast<T *>(it->second);
            return true;
        }
        return false;
    }

private:
    CadDictionary *ensureCadDictionaryExist(const std::string &name);
    void onEntryNameChanged(const std::string &olName, const std::string &newName);
};

}// namespace dwg