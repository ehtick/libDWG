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

#include <dwg/ACadVersion.h>
#include <dwg/io/dwg/readers/IDwgStreamReader_p.h>
#include <dwg/utils/StreamWrapper.h>

namespace dwg {

class DwgStreamReaderBase : public IDwgStreamReader
{
public:
    DwgStreamReaderBase(std::iostream *stream, bool resetPosition);
    ~DwgStreamReaderBase();

    static IDwgStreamReader *GetStreamHandler(ACadVersion version, std::iostream *stream,
                                              Encoding encoding = Encoding(), bool resetPosition = false);

    Encoding encoding() const override;
    void setEncoding(Encoding value) override;

    std::iostream *stream() override;

    int bitShift() const override;
    long long position() const override;

    void setBitShift(int);
    void setPosition(long long) override;

    bool isEmpty() const;
    void setEmpty(bool);

    unsigned char readByte() override;
    short readShort() override;
    long long setPositionByFlag(long long) override;
    int readInt() override;
    unsigned int readUInt() override;
    double readDouble() override;
    std::vector<unsigned char> readBytes(int length) override;
    bool readBit() override;
    short readBitAsShort() override;
    unsigned char read2Bits() override;
    short readBitShort() override;
    bool readBitShortAsBool() override;
    int readBitLong() override;
    long long readBitLongLong() override;
    double readBitDouble() override;
    XY read2BitDouble() override;
    XYZ read3BitDouble() override;
    char readRawChar() override;
    long long readRawLong() override;
    unsigned long long readRawULong() override;
    XY read2RawDouble() override;
    XYZ read3RawDouble() override;
    unsigned long long readModularChar() override;
    int readSignedModularChar() override;
    int readModularShort() override;
    unsigned long long handleReference() override;
    unsigned long long handleReference(unsigned long long referenceHandle) override;
    unsigned long long handleReference(unsigned long long referenceHandle, DwgReferenceType &reference) override;
    std::string readTextUtf8() override;
    std::string readVariableText() override;
    std::vector<unsigned char> readSentinel() override;
    XY read2BitDoubleWithDefault(const XY &) override;
    XYZ read3BitDoubleWithDefault(const XYZ &) override;
    Color readCmColor() override;
    Color readEnColor(Transparency &, bool &flag) override;
    Color readColorByIndex() override;
    ObjectType readObjectType() override;
    XYZ readBitExtrusion() override;
    double readBitDoubleWithDefault(double def) override;
    double readBitThickness() override;
    DateTime read8BitJulianDate() override;
    DateTime readDateTime() override;
    double readTimeSpan() override;
    long long positionInBits() override;
    void setPositionInBits(long long positon) override;
    void advanceByte() override;
    void advance(int offset) override;
    unsigned short resetShift() override;
    std::string readString(size_t length, Encoding encoding) override;

protected:
    void applyFlagToPosition(long long lastPos, long long &length, long long &strDataSize);
    unsigned char applyShiftToLasByte();
    void applyShiftToArr(int length, std::vector<unsigned char> &arr);
    unsigned char read3bits();
    DateTime julianToDate(int jdata, int miliseconds);

protected:
    std::iostream *_stream;
    StreamWrapper _wrapper;
    unsigned char _lastByte = 0;
    int _bitShift = 0;
    Encoding _encoding;
};

}// namespace dwg