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

#include <dwg/CadDocument.h>
#include <dwg/header/CadHeader.h>
#include <dwg/io/dwg/writers/DwgObjectWriter_p.h>
#include <dwg/tables/AppId.h>
#include <dwg/tables/Layer.h>
#include <dwg/tables/BlockRecord.h>
#include <dwg/tables/DimensionStyle.h>
#include <dwg/tables/UCS.h>
#include <dwg/tables/TextStyle.h>
#include <dwg/tables/VPort.h>
#include <dwg/tables/View.h>
#include <dwg/tables/LineType.h>
#include <dwg/io/dwg/fileheaders/DwgSectionDefinition_p.h>
#include <dwg/utils/EndianConverter.h>
#include <dwg/objects/CadDictionary.h>
#include <dwg/io/dwg/writers/IDwgStreamWriter_p.h>
#include <dwg/blocks/Block.h>
#include <dwg/blocks/BlockEnd.h>

namespace dwg {

DwgObjectWriter::DwgObjectWriter(std::ostream *stream, CadDocument *document, Encoding encoding, bool writeXRecords,
                                 bool writeXData)
    : DwgSectionIO(document->header()->version())
{
}
DwgObjectWriter::~DwgObjectWriter() {}

std::string DwgObjectWriter::sectionName() const
{
    return DwgSectionDefinition::AcDbObjects;
}

void DwgObjectWriter::write()
{
    //For R18 and later the section data (right after the page header) starts with a
    //RL value of 0x0dca (meaning unknown).
    if (R2004Plus)
    {
        std::vector<unsigned char> arr = LittleEndianConverter::instance()->bytes((int) 0xDCA);
        _stream->write(reinterpret_cast<const char *>(arr.data()), arr.size());
    }

    _objects.push(_document->rootDictionary());

    writeBlockControl();
    writeTable(_document->layers());
    writeTable(_document.TextStyles);
    writeLTypeControlObject();
    writeTable(_document.Views);
    writeTable(_document.UCSs);
    writeTable(_document.VPorts);
    writeTable(_document.AppIds);
    //For some reason the dimension must be writen the last
    writeTable(_document.DimensionStyles);

    writeBlockEntities();
    writeObjects();
}

std::map<unsigned long long, long long> DwgObjectWriter::handleMap() const
{
    return std::map<unsigned long long, long long>();
}

bool DwgObjectWriter::writeXRecords() const
{
    return false;
}

bool DwgObjectWriter::writeXData() const
{
    return false;
}

void DwgObjectWriter::writeLTypeControlObject() {}

void DwgObjectWriter::writeBlockControl() {}

void DwgObjectWriter::writeTable() {}

void DwgObjectWriter::writeEntries() {}

void DwgObjectWriter::writeBlockEntities() {}

void DwgObjectWriter::writeAppId(AppId *app)
{
    writeCommonNonEntityData(app);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(app->name());

    writeXrefDependantBit(app);

    //Unknown RC 71 Undoc'd 71-group; doesn't even appear in DXF or an entget if it's 0.
    _writer->writeByte(0);

    //External reference block handle(hard pointer)	??
    _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);

    registerObject(app);
}

void DwgObjectWriter::writeBlockRecord(BlockRecord *blkRecord)
{
    writeBlockHeader(blkRecord);
}

void DwgObjectWriter::writeBlockHeader(BlockRecord *record)
{
    writeCommonNonEntityData(record);

    //Common:
    //Entry name TV 2
    if (record->flags() & BlockTypeFlag::Anonymous)
    {
        //Warning: anonymous blocks do not write the full name, only *{type character}
        _writer->writeVariableText(record->name().substr(0, 2));
    }
    else
    {
        _writer->writeVariableText(record->name());
    }

    writeXrefDependantBit(record);

    //Anonymous B 1 if this is an anonymous block (1 bit)
    _writer->writeBit(record->flags() & BlockTypeFlag::Anonymous);

    //Hasatts B 1 if block contains attdefs (2 bit)
    _writer->writeBit(record->hasAttribute());

    //Blkisxref B 1 if block is xref (4 bit)
    _writer->writeBit(record->flags() & BlockTypeFlag::XRef);

    //Xrefoverlaid B 1 if an overlaid xref (8 bit)
    _writer->writeBit(record->flags() & BlockTypeFlag::XRefOverlay);

    //R2000+:
    if (R2000Plus)
    {
        //Loaded Bit B 0 indicates loaded for an xref
        _writer->writeBit(record->flags() & BlockTypeFlag::XRef);
    }

    //R2004+:
    if ((R2004Plus && !(record->flags() & BlockTypeFlag::XRef)) &&
        !(record->flags() & BlockTypeFlag::XRefOverlay))
    {
        //Owned Object Count BL Number of objects owned by this object.
        _writer->writeBitLong(record->entities().size());
    }

    //Common:
    //Base pt 3BD 10 Base point of block.
    _writer->write3BitDouble(record.BlockEntity.BasePoint);
    //Xref pname TV 1 Xref pathname. That's right: DXF 1 AND 3!
    //3 1 appears in a tblnext/ search elist; 3 appears in an entget.
    _writer->writeVariableText(record.BlockEntity.XrefPath);

    //R2000+:
    if (R2000Plus)
    {
        //Insert Count RC A sequence of zero or more non-zero RC’s, followed by a terminating 0 RC.The total number of these indicates how many insert handles will be present.
        for (var item in _document.Entities.OfType<Insert>().Where(i = > i.Block.Name == record.Name))
        {
            _writer->writeByte(1);
        }

        _writer->writeByte(0);

        //Block Description TV 4 Block description.
        _writer->writeVariableText(record.BlockEntity.Comments);

        //Size of preview data BL Indicates number of bytes of data following.
        _writer->writeBitLong(0);
    }

    //R2007+:
    if (R2007Plus)
    {
        //Insert units BS 70
        _writer->writeBitShort((short) record.Units);
        //Explodable B 280
        _writer->writeBit(record->isExplodable());
        //Block scaling RC 281
        _writer->writeByte((unsigned char) (record.CanScale ? 1 : 0));
    }

    //NULL(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    //BLOCK entity. (hard owner)
    //Block begin object
    _writer->handleReference(DwgReferenceType::HardOwnership, record->blockEntity());

    //R13-R2000:
    if (_version >= ACadVersion::AC1012 && _version <= ACadVersion::AC1015 &&
        !(record->flags() & BlockTypeFlag::XRef) && !(record->flags() & BlockTypeFlag::XRefOverlay))
    {
        if (!record.Entities.empty())
        {
            //first entity in the def. (soft pointer)
            _writer->handleReference(DwgReferenceType::SoftPointer, record->entities().front());
            //last entity in the def. (soft pointer)
            _writer->handleReference(DwgReferenceType::SoftPointer, record->entities().back());
        }
        else
        {
            _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
            _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
        }
    }

    //R2004+:
    if (R2004Plus)
    {
        for (auto item : record->entities())
        {
            //H[ENTITY(hard owner)] Repeats “Owned Object Count” times.
            _writer->handleReference(DwgReferenceType::HardOwnership, item);
        }
    }

    //Common:
    //ENDBLK entity. (hard owner)
    _writer->handleReference(DwgReferenceType::HardOwnership, record->blockEnd());

    //R2000+:
    if (R2000Plus)
    {
        for (var item in _document.Entities.OfType<Insert>().Where(i = > i.Block.Name == record.Name))
        {
            _writer->handleReference(DwgReferenceType::SoftPointer, item);
        }

        //Layout Handle H(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, record->layout());
    }

    registerObject(record);
}

void DwgObjectWriter::writeBlockBegin(Block *block)
{
    writeCommonEntityData(block);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(block.Name);

    registerObject(block);
}

void DwgObjectWriter::writeBlockEnd(BlockEnd *blkEnd)
{
    writeCommonEntityData(blkEnd);

    registerObject(blkEnd);
}

void DwgObjectWriter::writeLayer(Layer *layer)
{
    writeCommonNonEntityData(layer);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(layer->name());

    writeXrefDependantBit(layer);

    //R13-R14 Only:
    if (R13_14Only)
    {
        //Frozen B 70 if frozen (1 bit)
        _writer->writeBit(layer->flags() & LayerFlag::Frozen);
        //On B if on.
        _writer->writeBit(layer->isOn());
        //Frz in new B 70 if frozen by default in new viewports (2 bit)
        _writer->writeBit(layer->flags() & LayerFlag::FrozenNewViewports);
        //Locked B 70 if locked (4 bit)
        _writer->writeBit(layer->flags() & LayerFlag::Locked);
    }

    //R2000+:
    if (R2000Plus)
    {
        //and lineweight (mask with 0x03E0)
        short values = (short) (CadUtils.toIndex(layer->LineWeight) << 5);

        //contains frozen (1 bit),
        if (layer->flags() & LayerFlag::Frozen)
            values |= 0b1;

        //on (2 bit)
        if (!layer->isOn())
            values |= 0b10;

        //frozen by default in new viewports (4 bit)
        if (layer->flags() & LayerFlag::Frozen)
            values |= 0b100;

        //locked (8 bit)
        if (layer->flags() & LayerFlag::Locked)
            values |= 0b1000;

        //plotting flag (16 bit),
        if (layer->plotFlag())
            values |= 0b10000;

        //Values BS 70,290,370
        _writer->writeBitShort(values);
    }

    //Common:
    //Color CMC 62
    _writer->writeCmColor(layer->color());

    //External reference block handle(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, nullptr);

    //R2000+:
    if (R2000Plus)
    {
        //H 390 Plotstyle (hard pointer), by default points to PLACEHOLDER with handle 0x0f.
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    }

    //R2007+:
    if (R2007Plus)
    {
        //H 347 Material
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    }

    //Common:
    //H 6 linetype (hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, layer->lineType()->handle());

    if (R2013Plus)
    {
        //H Unknown handle (hard pointer). Always seems to be NULL.
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    }

    registerObject(layer);
}

void DwgObjectWriter::writeLineType(LineType *ltype)
{
    writeCommonNonEntityData(ltype);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(ltype->Name);

    writeXrefDependantBit(ltype);

    //Description TV 3
    _writer->writeVariableText(ltype->Description);
    //Pattern Len BD 40
    _writer->writeBitDouble(ltype->PatternLen);
    //Alignment RC 72 Always 'A'.
    _writer->writeByte((unsigned char) ltype->Alignment);

    //Numdashes RC 73 The number of repetitions of the 49...74 data.
    _writer->writeByte((unsigned char) ltype->Segments.Count());
    bool isText = false;
    for (LineType.Segment segment in ltype->Segments)
    {
        //Dash length BD 49 Dash or dot specifier.
        _writer->writeBitDouble(segment.Length);
        //Complex shapecode BS 75 Shape number if shapeflag is 2, or index into the string area if shapeflag is 4.
        _writer->writeBitShort(segment.ShapeNumber);

        //X - offset RD 44 (0.0 for a simple dash.)
        //Y - offset RD 45(0.0 for a simple dash.)
        _writer->writeRawDouble(segment.Offset.X);
        _writer->writeRawDouble(segment.Offset.Y);

        //Scale BD 46 (1.0 for a simple dash.)
        _writer->writeBitDouble(segment.Scale);
        //Rotation BD 50 (0.0 for a simple dash.)
        _writer->writeBitDouble(segment.Rotation);
        //Shapeflag BS 74 bit coded:
        _writer->writeBitShort((short) segment.Shapeflag);

        if (segment.Shapeflag.HasFlag(LinetypeShapeFlags.Text))
            isText = true;
    }

    //R2004 and earlier:
    if (_version <= ACadVersion::AC1018)
    {
        //Strings area X 9 256 bytes of text area. The complex dashes that have text use this area via the 75-group indices. It's basically a pile of 0-terminated strings.
        //First unsigned char is always 0 for R13 and data starts at unsigned char 1.
        //In R14 it is not a valid data start from unsigned char 0.
        //(The 9 - group is undocumented.)
        for (int i = 0; i < 256; i++)
        {
            //TODO: Write the line type text area
            _writer->writeByte(0);
        }
    }

    //R2007+:
    if (R2007Plus && isText)
    {
        for (int i = 0; i < 512; i++)
        {
            //TODO: Write the line type text area
            _writer->writeByte(0);
        }
    }

    //Common:
    //External reference block handle(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);

    for (auto &&segment in ltype->Segments)
    {
        //340 shapefile for dash/shape (1 each) (hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, segment.Style);
    }

    registerObject(ltype);
}

void DwgObjectWriter::writeTextStyle(TextStyle *style)
{
    writeCommonNonEntityData(style);

    //Common:
    //Entry name TV 2
    if (style->IsShapeFile)
    {
        _writer->writeVariableText(string.Empty);
    }
    else
    {
        _writer->writeVariableText(style->Name);
    }

    writeXrefDependantBit(style);

    //shape file B 1 if a shape file rather than a font (1 bit)
    _writer->writeBit(style->Flags.HasFlag(StyleFlags.IsShape));

    //Vertical B 1 if vertical (4 bit of flag)
    _writer->writeBit(style->Flags.HasFlag(StyleFlags.VerticalText));
    //Fixed height BD 40
    _writer->writeBitDouble(style->Height);
    //Width factor BD 41
    _writer->writeBitDouble(style->Width);
    //Oblique ang BD 50
    _writer->writeBitDouble(style->ObliqueAngle);
    //Generation RC 71 Generation flags (not bit-pair coded).
    _writer->writeByte((unsigned char) style->MirrorFlag);
    //Last height BD 42
    _writer->writeBitDouble(style->LastHeight);
    //Font name TV 3
    _writer->writeVariableText(style->Filename);
    //Bigfont name TV 4
    _writer->writeVariableText(style->BigFontFilename);

    _writer->handleReference(DwgReferenceType::HardPointer, _document.TextStyles);

    registerObject(style);
}

void DwgObjectWriter::writeUCS(UCS *ucs)
{
    writeCommonNonEntityData(ucs);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(ucs->name());

    writeXrefDependantBit(ucs);

    //Origin 3BD 10
    _writer->write3BitDouble(ucs->origin());
    //X - direction 3BD 11
    _writer->write3BitDouble(ucs->XAxis);
    //Y - direction 3BD 12
    _writer->write3BitDouble(ucs->YAxis);

    //R2000+:
    if (R2000Plus)
    {
        //Elevation BD 146
        _writer->writeBitDouble(ucs->elevation());
        //OrthographicViewType BS 79	//dxf docs: 79	Always 0
        _writer->writeBitShort((short) ucs->orthographicViewType());
        //OrthographicType BS 71
        _writer->writeBitShort((short) ucs->orthographicType());
    }

    //Common:
    //Handle refs H ucs control object (soft pointer)
    _writer->handleReference(DwgReferenceType::SoftPointer, _document.UCSs);

    //R2000 +:
    if (R2000Plus)
    {
        //Base UCS Handle H 346 hard pointer
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
        //Named UCS Handle H -hard pointer, not present in DXF
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    }

    registerObject(ucs);
}

void DwgObjectWriter::writeView(View *view)
{
    writeCommonNonEntityData(view);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(view->Name);

    writeXrefDependantBit(view);

    //View height BD 40
    _writer->writeBitDouble(view->Height);
    //View width BD 41
    _writer->writeBitDouble(view->Width);
    //View center 2RD 10(Not bit - pair coded.)
    _writer->write2RawDouble(view->Center);
    //Target 3BD 12
    _writer->write3BitDouble(view->Target);
    //View dir 3BD 11 DXF doc suggests from target toward camera.
    _writer->write3BitDouble(view->Direction);
    //Twist angle BD 50 Radians
    _writer->writeBitDouble(view->Angle);
    //Lens length BD 42
    _writer->writeBitDouble(view->LensLength);
    //Front clip BD 43
    _writer->writeBitDouble(view->FrontClipping);
    //Back clip BD 44
    _writer->writeBitDouble(view->BackClipping);

    //View mode X 71 4 bits: 0123
    //Note that only bits 0, 1, 2, and 4 of the 71 can be specified -- not bit 3 (8).
    //0 : 71's bit 0 (1)
    _writer->writeBit(view->ViewMode.HasFlag(ViewModeType.PerspectiveView));
    //1 : 71's bit 1 (2)
    _writer->writeBit(view->ViewMode.HasFlag(ViewModeType.FrontClipping));
    //2 : 71's bit 2 (4)
    _writer->writeBit(view->ViewMode.HasFlag(ViewModeType.BackClipping));
    //3 : OPPOSITE of 71's bit 4 (16)
    _writer->writeBit(view->ViewMode.HasFlag(ViewModeType.FrontClippingZ));

    //R2000+:
    if (R2000Plus)
    {
        //Render Mode RC 281
        _writer->writeByte((unsigned char) view->RenderMode);
    }

    //R2007+:
    if (R2007Plus)
    {
        //Use default lights B ? Default value is true
        _writer->writeBit(true);
        //Default lighting RC ? Default value is 1
        _writer->writeByte(1);
        //Brightness BD ? Default value is 0
        _writer->writeBitDouble(0.0);
        //Contrast BD ? Default value is 0
        _writer->writeBitDouble(0.0);
        //Abient color CMC? Default value is indexed color 250
        _writer->writeCmColor(new Color(250));
    }

    //Common:
    //Pspace flag B 70 Bit 0(1) of the 70 - group.
    _writer->writeBit(view->Flags.HasFlag((StandardFlags) 0b1));

    if (R2000Plus)
    {
        _writer->writeBit(view->IsUcsAssociated);
        if (view->IsUcsAssociated)
        {
            //Origin 3BD 10 This and next 4 R2000 items are present only if 72 value is 1.
            _writer->write3BitDouble(view->UcsOrigin);
            //X-direction 3BD 11
            _writer->write3BitDouble(view->UcsXAxis);
            //Y-direction 3BD 12
            _writer->write3BitDouble(view->UcsYAxis);
            //Elevation BD 146
            _writer->writeBitDouble(view->UcsElevation);
            //OrthographicViewType BS 79
            _writer->writeBitShort((short) view->UcsOrthographicType);
        }
    }

    //Common:
    //Handle refs H view control object (soft pointer)
    _writer->handleReference(DwgReferenceType::SoftPointer, _document.Views);

    //R2007+:
    if (R2007Plus)
    {
        //Camera plottable B 73
        _writer->writeBit(view->IsPlottable);

        //Background handle H 332 soft pointer
        _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
        //Visual style H 348 hard pointer
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
        //Sun H 361 hard owner
        _writer->handleReference(DwgReferenceType::HardOwnership, 0ULL);
    }

    if (R2000Plus && view->IsUcsAssociated)
    {
        //TODO: Implement ucs reference for view
        //Base UCS Handle H 346 hard pointer
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
        //Named UCS Handle H 345 hard pointer
        _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
    }

    //R2007+:
    if (R2007Plus)
    {
        //Live section H 334 soft pointer
        _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
    }

    registerObject(view);
}

void DwgObjectWriter::writeDimensionStyle(DimensionStyle *dimStyle)
{
    writeCommonNonEntityData(dimStyle);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(dimStyle->Name);

    writeXrefDependantBit(dimStyle);

    //R13 & R14 Only:
    if (R13_14Only)
    {
        //DIMTOL B 71
        _writer->writeBit(dimStyle->GenerateTolerances);
        //DIMLIM B 72
        _writer->writeBit(dimStyle->LimitsGeneration);
        //DIMTIH B 73
        _writer->writeBit(dimStyle->TextOutsideHorizontal);
        //DIMTOH B 74
        _writer->writeBit(dimStyle->SuppressFirstExtensionLine);
        //DIMSE1 B 75
        _writer->writeBit(dimStyle->SuppressSecondExtensionLine);
        //DIMSE2 B 76
        _writer->writeBit(dimStyle->TextInsideHorizontal);
        //DIMALT B 170
        _writer->writeBit(dimStyle->AlternateUnitDimensioning);
        //DIMTOFL B 172
        _writer->writeBit(dimStyle->TextOutsideExtensions);
        //DIMSAH B 173
        _writer->writeBit(dimStyle->SeparateArrowBlocks);
        //DIMTIX B 174
        _writer->writeBit(dimStyle->TextInsideExtensions);
        //DIMSOXD B 175
        _writer->writeBit(dimStyle->SuppressOutsideExtensions);
        //DIMALTD RC 171
        _writer->writeByte((unsigned char) dimStyle->AlternateUnitDecimalPlaces);
        //DIMZIN RC 78
        _writer->writeByte((unsigned char) dimStyle->ZeroHandling);
        //DIMSD1 B 281
        _writer->writeBit(dimStyle->SuppressFirstDimensionLine);
        //DIMSD2 B 282
        _writer->writeBit(dimStyle->SuppressSecondDimensionLine);
        //DIMTOLJ RC 283
        _writer->writeByte((unsigned char) dimStyle->ToleranceAlignment);
        //DIMJUST RC 280
        _writer->writeByte((unsigned char) dimStyle->TextHorizontalAlignment);
        //DIMFIT RC 287
        _writer->writeByte((unsigned char) dimStyle->DimensionFit);
        //DIMUPT B 288
        _writer->writeBit(dimStyle->CursorUpdate);
        //DIMTZIN RC 284
        _writer->writeByte((unsigned char) dimStyle->ToleranceZeroHandling);
        //DIMALTZ RC 285
        _writer->writeByte((unsigned char) dimStyle->AlternateUnitZeroHandling);
        //DIMALTTZ RC 286
        _writer->writeByte((unsigned char) dimStyle->AlternateUnitToleranceZeroHandling);
        //DIMTAD RC 77
        _writer->writeByte((unsigned char) dimStyle->TextVerticalAlignment);
        //DIMUNIT BS 270
        _writer->writeBitShort(dimStyle->DimensionUnit);
        //DIMAUNIT BS 275
        _writer->writeBitShort((short) dimStyle->AngularUnit);
        //DIMDEC BS 271
        _writer->writeBitShort(dimStyle->DecimalPlaces);
        //DIMTDEC BS 272
        _writer->writeBitShort(dimStyle->ToleranceDecimalPlaces);
        //DIMALTU BS 273
        _writer->writeBitShort((short) dimStyle->AlternateUnitFormat);
        //DIMALTTD BS 274
        _writer->writeBitShort(dimStyle->AlternateUnitToleranceDecimalPlaces);
        //DIMSCALE BD 40
        _writer->writeBitDouble(dimStyle->ScaleFactor);
        //DIMASZ BD 41
        _writer->writeBitDouble(dimStyle->ArrowSize);
        //DIMEXO BD 42
        _writer->writeBitDouble(dimStyle->ExtensionLineOffset);
        //DIMDLI BD 43
        _writer->writeBitDouble(dimStyle->DimensionLineIncrement);
        //DIMEXE BD 44
        _writer->writeBitDouble(dimStyle->ExtensionLineExtension);
        //DIMRND BD 45
        _writer->writeBitDouble(dimStyle->Rounding);
        //DIMDLE BD 46
        _writer->writeBitDouble(dimStyle->DimensionLineExtension);
        //DIMTP BD 47
        _writer->writeBitDouble(dimStyle->PlusTolerance);
        //DIMTM BD 48
        _writer->writeBitDouble(dimStyle->MinusTolerance);
        //DIMTXT BD 140
        _writer->writeBitDouble(dimStyle->TextHeight);
        //DIMCEN BD 141
        _writer->writeBitDouble(dimStyle->CenterMarkSize);
        //DIMTSZ BD 142
        _writer->writeBitDouble(dimStyle->TickSize);
        //DIMALTF BD 143
        _writer->writeBitDouble(dimStyle->AlternateUnitScaleFactor);
        //DIMLFAC BD 144
        _writer->writeBitDouble(dimStyle->LinearScaleFactor);
        //DIMTVP BD 145
        _writer->writeBitDouble(dimStyle->TextVerticalPosition);
        //DIMTFAC BD 146
        _writer->writeBitDouble(dimStyle->ToleranceScaleFactor);
        //DIMGAP BD 147
        _writer->writeBitDouble(dimStyle->DimensionLineGap);

        //DIMPOST T 3
        _writer->writeVariableText(dimStyle->PostFix);
        //DIMAPOST T 4
        _writer->writeVariableText(dimStyle->AlternateDimensioningSuffix);

        //DIMBLK T 5
                _writer->writeVariableText(dimStyle->ArrowBlock?.Name);
                //DIMBLK1 T 6
                _writer->writeVariableText(dimStyle->DimArrow1?.Name);
                //DIMBLK2 T 7
                _writer->writeVariableText(dimStyle->DimArrow2?.Name);

                //DIMCLRD BS 176
                _writer->writeCmColor(dimStyle->DimensionLineColor);
                //DIMCLRE BS 177
                _writer->writeCmColor(dimStyle->ExtensionLineColor);
                //DIMCLRT BS 178
                _writer->writeCmColor(dimStyle->TextColor);
    }

    //R2000+:
    if (R2000Plus)
    {
        //DIMPOST TV 3
        _writer->writeVariableText(dimStyle->PostFix);
        //DIMAPOST TV 4
        _writer->writeVariableText(dimStyle->AlternateDimensioningSuffix);
        //DIMSCALE BD 40
        _writer->writeBitDouble(dimStyle->ScaleFactor);
        //DIMASZ BD 41
        _writer->writeBitDouble(dimStyle->ArrowSize);
        //DIMEXO BD 42
        _writer->writeBitDouble(dimStyle->ExtensionLineOffset);
        //DIMDLI BD 43
        _writer->writeBitDouble(dimStyle->DimensionLineIncrement);
        //DIMEXE BD 44
        _writer->writeBitDouble(dimStyle->ExtensionLineExtension);
        //DIMRND BD 45
        _writer->writeBitDouble(dimStyle->Rounding);
        //DIMDLE BD 46
        _writer->writeBitDouble(dimStyle->DimensionLineExtension);
        //DIMTP BD 47
        _writer->writeBitDouble(dimStyle->PlusTolerance);
        //DIMTM BD 48
        _writer->writeBitDouble(dimStyle->MinusTolerance);
    }

    //R2007+:
    if (R2007Plus)
    {
        //DIMFXL BD 49
        _writer->writeBitDouble(dimStyle->FixedExtensionLineLength);
        //DIMJOGANG BD 50
        _writer->writeBitDouble(dimStyle->JoggedRadiusDimensionTransverseSegmentAngle);
        //DIMTFILL BS 69
        _writer->writeBitShort((short) dimStyle->TextBackgroundFillMode);
        //DIMTFILLCLR CMC 70
        _writer->writeCmColor(dimStyle->TextBackgroundColor);
    }

    //R2000+:
    if (R2000Plus)
    {
        //DIMTOL B 71
        _writer->writeBit(dimStyle->GenerateTolerances);
        //DIMLIM B 72
        _writer->writeBit(dimStyle->LimitsGeneration);
        //DIMTIH B 73
        _writer->writeBit(dimStyle->TextInsideHorizontal);
        //DIMTOH B 74
        _writer->writeBit(dimStyle->TextOutsideHorizontal);
        //DIMSE1 B 75
        _writer->writeBit(dimStyle->SuppressFirstExtensionLine);
        //DIMSE2 B 76
        _writer->writeBit(dimStyle->SuppressSecondExtensionLine);
        //DIMTAD BS 77
        _writer->writeBitShort((short) dimStyle->TextVerticalAlignment);
        //DIMZIN BS 78
        _writer->writeBitShort((short) dimStyle->ZeroHandling);
        //DIMAZIN BS 79
        _writer->writeBitShort((short) dimStyle->AngularZeroHandling);
    }

    //R2007 +:
    if (R2007Plus)
    {
        //DIMARCSYM BS 90
        _writer->writeBitShort((short) dimStyle->ArcLengthSymbolPosition);
    }

    //R2000 +:
    if (R2000Plus)
    {
        //DIMTXT BD 140
        _writer->writeBitDouble(dimStyle->TextHeight);
        //DIMCEN BD 141
        _writer->writeBitDouble(dimStyle->CenterMarkSize);
        //DIMTSZ BD 142
        _writer->writeBitDouble(dimStyle->TickSize);
        //DIMALTF BD 143
        _writer->writeBitDouble(dimStyle->AlternateUnitScaleFactor);
        //DIMLFAC BD 144
        _writer->writeBitDouble(dimStyle->LinearScaleFactor);
        //DIMTVP BD 145
        _writer->writeBitDouble(dimStyle->TextVerticalPosition);
        //DIMTFAC BD 146
        _writer->writeBitDouble(dimStyle->ToleranceScaleFactor);
        //DIMGAP BD 147
        _writer->writeBitDouble(dimStyle->DimensionLineGap);
        //DIMALTRND BD 148
        _writer->writeBitDouble(dimStyle->AlternateUnitRounding);
        //DIMALT B 170
        _writer->writeBit(dimStyle->AlternateUnitDimensioning);
        //DIMALTD BS 171
        _writer->writeBitShort(dimStyle->AlternateUnitDecimalPlaces);
        //DIMTOFL B 172
        _writer->writeBit(dimStyle->TextOutsideExtensions);
        //DIMSAH B 173
        _writer->writeBit(dimStyle->SeparateArrowBlocks);
        //DIMTIX B 174
        _writer->writeBit(dimStyle->TextInsideExtensions);
        //DIMSOXD B 175
        _writer->writeBit(dimStyle->SuppressOutsideExtensions);
        //DIMCLRD BS 176
        _writer->writeCmColor(dimStyle->DimensionLineColor);
        //DIMCLRE BS 177
        _writer->writeCmColor(dimStyle->ExtensionLineColor);
        //DIMCLRT BS 178
        _writer->writeCmColor(dimStyle->TextColor);
        //DIMADEC BS 179
        _writer->writeBitShort(dimStyle->AngularDimensionDecimalPlaces);
        //DIMDEC BS 271
        _writer->writeBitShort(dimStyle->DecimalPlaces);
        //DIMTDEC BS 272
        _writer->writeBitShort(dimStyle->ToleranceDecimalPlaces);
        //DIMALTU BS 273
        _writer->writeBitShort((short) dimStyle->AlternateUnitFormat);
        //DIMALTTD BS 274
        _writer->writeBitShort(dimStyle->AlternateUnitToleranceDecimalPlaces);
        //DIMAUNIT BS 275
        _writer->writeBitShort((short) dimStyle->AngularUnit);
        //DIMFRAC BS 276
        _writer->writeBitShort((short) dimStyle->FractionFormat);
        //DIMLUNIT BS 277
        _writer->writeBitShort((short) dimStyle->LinearUnitFormat);
        //DIMDSEP BS 278
        _writer->writeBitShort((short) dimStyle->DecimalSeparator);
        //DIMTMOVE BS 279
        _writer->writeBitShort((short) dimStyle->TextMovement);
        //DIMJUST BS 280
        _writer->writeBitShort((short) dimStyle->TextHorizontalAlignment);
        //DIMSD1 B 281
        _writer->writeBit(dimStyle->SuppressFirstDimensionLine);
        //DIMSD2 B 282
        _writer->writeBit(dimStyle->SuppressSecondDimensionLine);
        //DIMTOLJ BS 283
        _writer->writeBitShort((short) dimStyle->ToleranceAlignment);
        //DIMTZIN BS 284
        _writer->writeBitShort((short) dimStyle->ToleranceZeroHandling);
        //DIMALTZ BS 285
        _writer->writeBitShort((short) dimStyle->AlternateUnitZeroHandling);
        //DIMALTTZ BS 286
        _writer->writeBitShort((short) dimStyle->AlternateUnitToleranceZeroHandling);
        //DIMUPT B 288
        _writer->writeBit(dimStyle->CursorUpdate);
        //DIMFIT BS 287
        _writer->writeBitShort(3);
    }

    //R2007+:
    if (R2007Plus)
    {
        //DIMFXLON B 290
        _writer->writeBit(dimStyle->IsExtensionLineLengthFixed);
    }

    //R2010+:
    if (R2010Plus)
    {
        //DIMTXTDIRECTION B 295
        _writer->writeBit(dimStyle->TextDirection == TextDirection.RightToLeft);
        //DIMALTMZF BD ?
        _writer->writeBitDouble(dimStyle->AltMzf);
        //DIMALTMZS T ?
        _writer->writeVariableText(dimStyle->AltMzs);
        //DIMMZF BD ?
        _writer->writeBitDouble(dimStyle->Mzf);
        //DIMMZS T ?
        _writer->writeVariableText(dimStyle->Mzs);
    }

    //R2000+:
    if (R2000Plus)
    {
        //DIMLWD BS 371
        _writer->writeBitShort((short) dimStyle->DimensionLineWeight);
        //DIMLWE BS 372
        _writer->writeBitShort((short) dimStyle->ExtensionLineWeight);
    }

    //Common:
    //Unknown B 70 Seems to set the 0 - bit(1) of the 70 - group.
    _writer->writeBit(false);

    //Handle refs H Dimstyle control(soft pointer)
    //[Reactors(soft pointer)]
    //xdicobjhandle(hard owner)

    //External reference block handle(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);

    //340 shapefile(DIMTXSTY)(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->Style);

    //R2000+:
    if (R2000Plus)
    {
        //341 leader block(DIMLDRBLK) (hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->LeaderArrow);
        //342 dimblk(DIMBLK)(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->ArrowBlock);
        //343 dimblk1(DIMBLK1)(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->DimArrow1);
        //344 dimblk2(DIMBLK2)(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->DimArrow2);
    }

    //R2007+:
    if (R2007Plus)
    {
        //345 dimltype(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->LineType);
        //346 dimltex1(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->LineTypeExt1);
        //347 dimltex2(hard pointer)
        _writer->handleReference(DwgReferenceType::HardPointer, dimStyle->LineTypeExt2);
    }

    registerObject(dimStyle);
}

void DwgObjectWriter::writeVPort(VPort *vport)
{
    writeCommonNonEntityData(vport);

    //Common:
    //Entry name TV 2
    _writer->writeVariableText(vport->Name);

    writeXrefDependantBit(vport);

    //View height BD 40
    _writer->writeBitDouble(vport->ViewHeight);
    //Aspect ratio BD 41 The number stored here is actually the aspect ratio times the view height (40),
    //so this number must be divided by the 40-value to produce the aspect ratio that entget gives.
    //(R13 quirk; R12 has just the aspect ratio.)
    _writer->writeBitDouble(vport->AspectRatio * vport->ViewHeight);
    //View Center 2RD 12 DCS. (If it's plan view, add the view target (17) to get the WCS coordinates.
    //Careful! Sometimes you have to SAVE/OPEN to update the .dwg file.) Note that it's WSC in R12.
    _writer->write2RawDouble(vport->Center);
    //View target 3BD 17
    _writer->write3BitDouble(vport->Target);
    //View dir 3BD 16
    _writer->write3BitDouble(vport->Direction);
    //View twist BD 51
    _writer->writeBitDouble(vport->TwistAngle);
    //Lens length BD 42
    _writer->writeBitDouble(vport->LensLength);
    //Front clip BD 43
    _writer->writeBitDouble(vport->FrontClippingPlane);
    //Back clip BD 44
    _writer->writeBitDouble(vport->BackClippingPlane);

    //View mode X 71 4 bits: 0123
    //Note that only bits 0, 1, 2, and 4 are given here; see UCSFOLLOW below for bit 3(8) of the 71.
    //0 : 71's bit 0 (1)
    _writer->writeBit(vport->ViewMode.HasFlag(ViewModeType.PerspectiveView));
    //1 : 71's bit 1 (2)
    _writer->writeBit(vport->ViewMode.HasFlag(ViewModeType.FrontClipping));
    //2 : 71's bit 2 (4)
    _writer->writeBit(vport->ViewMode.HasFlag(ViewModeType.BackClipping));
    //3 : OPPOSITE of 71's bit 4 (16)
    _writer->writeBit(vport->ViewMode.HasFlag(ViewModeType.FrontClippingZ));

    //R2000+:
    if (R2000Plus)
    {
        //Render Mode RC 281
        _writer->writeByte((unsigned char) vport->RenderMode);
    }

    //R2007+:
    if (R2007Plus)
    {
        //Use default lights B 292
        _writer->writeBit(vport->UseDefaultLighting);
        //Default lighting type RC 282
        _writer->writeByte((unsigned char) vport->DefaultLighting);
        //Brightness BD 141
        _writer->writeBitDouble(vport->Brightness);
        //Constrast BD 142
        _writer->writeBitDouble(vport->Contrast);
        //Ambient Color CMC 63
        _writer->writeCmColor(vport->AmbientColor);
    }

    //Common:
    //Lower left 2RD 10 In fractions of screen width and height.
    _writer->write2RawDouble(vport->BottomLeft);
    //Upper right 2RD 11 In fractions of screen width and height.
    _writer->write2RawDouble(vport->TopRight);

    //UCSFOLLOW B 71 UCSFOLLOW. Bit 3 (8) of the 71-group.
    _writer->writeBit(vport->ViewMode.HasFlag(ViewModeType.Follow));

    //Circle zoom BS 72 Circle zoom percent.
    _writer->writeBitShort(vport->circleZoomPercent());

    //Fast zoom B 73
    _writer->writeBit(true);

    //UCSICON X 74 2 bits: 01
    //0 : 74's bit 0 (1)
    _writer->writeBit(vport->ucsIconDisplay() & UscIconType::OnLower);

    //1 : 74's bit 1 (2)
    _writer->writeBit(vport->ucsIconDisplay() & UscIconType::OnOrigin);

    //Grid on/off B 76
    _writer->writeBit(vport->showGrid());
    //Grd spacing 2RD 15
    _writer->write2RawDouble(vport->gridSpacing());
    //Snap on/off B 75
    _writer->writeBit(vport->snapOn());

    //Snap style B 77
    _writer->writeBit(vport->isometricSnap());

    //Snap isopair BS 78
    _writer->writeBitShort(vport->snapIsoPair());
    //Snap rot BD 50
    _writer->writeBitDouble(vport->snapRotation());
    //Snap base 2RD 13
    _writer->write2RawDouble(vport->snapBasePoint());
    //Snp spacing 2RD 14
    _writer->write2RawDouble(vport->snapSpacing());

    //R2000+:
    if (R2000Plus)
    {
        //Unknown B
        _writer->writeBit(false);

        //UCS per Viewport B 71
        _writer->writeBit(true);
        //UCS Origin 3BD 110
        _writer->write3BitDouble(vport->origin());
        //UCS X Axis 3BD 111
        _writer->write3BitDouble(vport->xAxis());
        //UCS Y Axis 3BD 112
        _writer->write3BitDouble(vport->yAxis());
        //UCS Elevation BD 146
        _writer->writeBitDouble(vport->elevation());
        //UCS Orthographic type BS 79
        _writer->writeBitShort((short) vport->orthographicType());
    }

    //R2007+:
    if (R2007Plus)
    {
        //Grid flags BS 60
        _writer->writeBitShort((short) vport->gridFlags());
        //Grid major BS 61
        _writer->writeBitShort(vport->minorGridLinesPerMajorGridLine());
    }

    //Common:
    //External reference block handle(hard pointer)
    _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);

    //R2007+:
    if (R2007Plus)
    {
        //Background handle H 332 soft pointer
        _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
        //Visual Style handle H 348 hard pointer
        _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
        //Sun handle H 361 hard owner
        _writer->handleReference(DwgReferenceType::SoftPointer, 0ULL);
    }

    //R2000+:
    if (R2000Plus)
    {
        if (vport->orthographicType() == OrthographicType::None)
        {
            //Named UCS Handle H 345 hard pointer
            _writer->handleReference(DwgReferenceType::HardPointer, vport->namedUcs());
            //Base UCS Handle H 346 hard pointer
            _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
        }
        else
        {
            //Named UCS Handle H 345 hard pointer
            _writer->handleReference(DwgReferenceType::HardPointer, 0ULL);
            //Base UCS Handle H 346 hard pointer
            _writer->handleReference(DwgReferenceType::HardPointer, vport->baseUcs());
        }
    }

    registerObject(vport);
}

}// namespace dwg