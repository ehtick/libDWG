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

namespace dwg {

enum class DxfCode
{
    Invalid = -9999,
    XDictionary = -6,

    /// \brief APP: persistent reactor chain
    PReactors = -5,
    /// \brief APP: conditional operator (used only with ssget)
    Operator = -4,

    /// \brief APP: extended data (XDATA) sentinel (fixed)
    XDataStart = -3,
    HeaderId = -2,

    /// \brief APP: entity name reference (fixed)
    FirstEntityId = -2,

    /// \brief APP: entity name. The name changes each time a drawing is opened.
    /// It is never saved (fixed)
    End = -1,

    /// \brief Text string indicating the entity type (fixed)
    Start = 0,
    Text = 1,
    XRefPath = 1,

    /// \brief Name (attribute tag, block name, and so on)
    ShapeName = 2,
    BlockName = 2,
    AttributeTag = 2,
    SymbolTableName = 2,
    MlineStyleName = 2,
    SymbolTableRecordName = 2,

    /// \brief Other text or name values
    AttributePrompt = 3,
    DimStyleName = 3,
    LinetypeProse = 3,
    TextFontFile = 3,
    Description = 3,
    DimPostString = 3,

    /// \brief Other text or name values
    TextBigFontFile = 4,
    DimensionAlternativePrefixSuffix = 4,
    CLShapeName = 4,
    SymbolTableRecordComments = 4,

    /// \brief Entity handle; text string of up to 16 hexadecimal digits (fixed)
    Handle = 5,
    DimensionBlock = 5,
    DimBlk1 = 6,
    LinetypeName = 6,
    DimBlk2 = 7,
    TextStyleName = 7,
    LayerName = 8,
    CLShapeText = 9,
    XCoordinate = 10,
    YCoordinate = 20,
    ZCoordinate = 30,
    Elevation = 38,
    Thickness = 39,
    Real = 40,
    ViewportHeight = 40,
    TxtSize = 40,
    TxtStyleXScale = 41,
    ViewWidth = 41,
    ViewportAspect = 41,
    TxtStylePSize = 42,
    ViewLensLength = 42,
    ViewFrontClip = 43,
    ViewBackClip = 44,
    ShapeXOffset = 44,
    ShapeYOffset = 45,
    ViewHeight = 45,
    ShapeScale = 46,
    PixelScale = 47,
    LinetypeScale = 48,
    DashLength = 49,
    MlineOffset = 49,
    LinetypeElement = 49,
    Angle = 50,
    ViewportSnapAngle = 50,
    ViewportTwist = 51,
    Visibility = 60,
    LayerLinetype = 61,
    Color = 62,
    HasSubentities = 66,
    ViewportVisibility = 67,
    ViewportActive = 68,
    ViewportNumber = 69,
    Int16 = 70,
    ViewMode = 71,
    CircleSides = 72,
    ViewportZoom = 73,
    ViewportIcon = 74,
    ViewportSnap = 75,
    ViewportGrid = 76,
    ViewportSnapStyle = 77,
    ViewportSnapPair = 78,
    RegAppFlags = 71,
    TxtStyleFlags = 71,
    LinetypeAlign = 72,
    LinetypePdc = 73,
    Int32 = 90,
    Subclass = 100,
    EmbeddedObjectStart = 101,
    ControlString = 102,
    DimVarHandle = 105,
    UcsOrg = 110,
    UcsOrientationX = 111,
    UcsOrientationY = 112,
    XReal = 140,
    ViewBrightness = 141,
    ViewContrast = 142,
    Int64 = 160,
    XInt16 = 170,
    NormalX = 210,
    NormalY = 220,
    NormalZ = 230,
    XXInt16 = 270,
    Int8 = 280,
    RenderMode = 281,
    Bool = 290,
    XTextString = 300,
    BinaryChunk = 310,
    ArbitraryHandle = 320,
    SoftPointerId = 330,
    HardPointerId = 340,
    SoftOwnershipId = 350,
    HardOwnershipId = 360,
    LineWeight = 370,
    PlotStyleNameType = 380,
    PlotStyleNameId = 390,
    ExtendedInt16 = 400,
    LayoutName = 410,
    ColorRgb = 420,
    ColorName = 430,
    Alpha = 440,
    GradientObjType = 450,
    GradientPatType = 451,
    GradientTintType = 452,
    GradientColCount = 453,
    GradientAngle = 460,
    GradientShift = 461,
    GradientTintVal = 462,
    GradientColVal = 463,
    GradientName = 470,
    Comment = 999,
    ExtendedDataAsciiString = 1000,
    ExtendedDataRegAppName = 1001,
    ExtendedDataControlString = 1002,
    ExtendedDataLayerName = 1003,
    ExtendedDataBinaryChunk = 1004,
    ExtendedDataHandle = 1005,
    ExtendedDataXCoordinate = 1010,
    ExtendedDataYCoordinate = 1020,
    ExtendedDataZCoordinate = 1030,
    ExtendedDataWorldXCoordinate = 1011,
    ExtendedDataWorldYCoordinate = 1021,
    ExtendedDataWorldZCoordinate = 1031,
    ExtendedDataWorldXDisp = 1012,
    ExtendedDataWorldYDisp = 1022,
    ExtendedDataWorldZDisp = 1032,
    ExtendedDataWorldXDir = 1013,
    ExtendedDataWorldYDir = 1023,
    ExtendedDataWorldZDir = 1033,
    ExtendedDataReal = 1040,
    ExtendedDataDist = 1041,
    ExtendedDataScale = 1042,
    ExtendedDataInteger16 = 1070,
    ExtendedDataInteger32 = 1071
};

}// namespace dwg