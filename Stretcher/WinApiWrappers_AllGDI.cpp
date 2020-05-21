#include "WinApiWrappers_AllGDI.h"
#include "ImportReplacer.h"

extern void SubstituteDC(HDC& hdc);

//Replace the Functions
void ReplaceImports_AllGDI(ImportReplacer &replacer)
{
#define ReplaceImport(dllName, functionName, replacementFunction, pOldFunction) \
    replacer.ReplaceImport((dllName),(functionName),(replacementFunction),(pOldFunction))

	ReplaceImport("Gdi32.dll", "Arc", (FARPROC)Arc_Replacement, (FARPROC*)&Arc_OLD);
	ReplaceImport("Gdi32.dll", "BitBlt", (FARPROC)BitBlt_Replacement, (FARPROC*)&BitBlt_OLD);
	ReplaceImport("Gdi32.dll", "CancelDC", (FARPROC)CancelDC_Replacement, (FARPROC*)&CancelDC_OLD);
	ReplaceImport("Gdi32.dll", "Chord", (FARPROC)Chord_Replacement, (FARPROC*)&Chord_OLD);
	ReplaceImport("Gdi32.dll", "ChoosePixelFormat", (FARPROC)ChoosePixelFormat_Replacement, (FARPROC*)&ChoosePixelFormat_OLD);
	ReplaceImport("Gdi32.dll", "CloseMetaFile", (FARPROC)CloseMetaFile_Replacement, (FARPROC*)&CloseMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "CreateCompatibleBitmap", (FARPROC)CreateCompatibleBitmap_Replacement, (FARPROC*)&CreateCompatibleBitmap_OLD);
	ReplaceImport("Gdi32.dll", "CreateDiscardableBitmap", (FARPROC)CreateDiscardableBitmap_Replacement, (FARPROC*)&CreateDiscardableBitmap_OLD);
	ReplaceImport("Gdi32.dll", "CreateCompatibleDC", (FARPROC)CreateCompatibleDC_Replacement, (FARPROC*)&CreateCompatibleDC_OLD);
	ReplaceImport("Gdi32.dll", "CreateDIBitmap", (FARPROC)CreateDIBitmap_Replacement, (FARPROC*)&CreateDIBitmap_OLD);
	ReplaceImport("Gdi32.dll", "DeleteDC", (FARPROC)DeleteDC_Replacement, (FARPROC*)&DeleteDC_OLD);
	ReplaceImport("Gdi32.dll", "DescribePixelFormat", (FARPROC)DescribePixelFormat_Replacement, (FARPROC*)&DescribePixelFormat_OLD);
	ReplaceImport("Gdi32.dll", "DrawEscape", (FARPROC)DrawEscape_Replacement, (FARPROC*)&DrawEscape_OLD);
	ReplaceImport("Gdi32.dll", "Ellipse", (FARPROC)Ellipse_Replacement, (FARPROC*)&Ellipse_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontFamiliesExA", (FARPROC)EnumFontFamiliesExA_Replacement, (FARPROC*)&EnumFontFamiliesExA_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontFamiliesExW", (FARPROC)EnumFontFamiliesExW_Replacement, (FARPROC*)&EnumFontFamiliesExW_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontFamiliesA", (FARPROC)EnumFontFamiliesA_Replacement, (FARPROC*)&EnumFontFamiliesA_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontFamiliesW", (FARPROC)EnumFontFamiliesW_Replacement, (FARPROC*)&EnumFontFamiliesW_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontsA", (FARPROC)EnumFontsA_Replacement, (FARPROC*)&EnumFontsA_OLD);
	ReplaceImport("Gdi32.dll", "EnumFontsW", (FARPROC)EnumFontsW_Replacement, (FARPROC*)&EnumFontsW_OLD);
	ReplaceImport("Gdi32.dll", "EnumObjects", (FARPROC)EnumObjects_Replacement, (FARPROC*)&EnumObjects_OLD);
	ReplaceImport("Gdi32.dll", "Escape", (FARPROC)Escape_Replacement, (FARPROC*)&Escape_OLD);
	ReplaceImport("Gdi32.dll", "ExtEscape", (FARPROC)ExtEscape_Replacement, (FARPROC*)&ExtEscape_OLD);
	ReplaceImport("Gdi32.dll", "ExcludeClipRect", (FARPROC)ExcludeClipRect_Replacement, (FARPROC*)&ExcludeClipRect_OLD);
	ReplaceImport("Gdi32.dll", "ExtFloodFill", (FARPROC)ExtFloodFill_Replacement, (FARPROC*)&ExtFloodFill_OLD);
	ReplaceImport("Gdi32.dll", "FillRgn", (FARPROC)FillRgn_Replacement, (FARPROC*)&FillRgn_OLD);
	ReplaceImport("Gdi32.dll", "FloodFill", (FARPROC)FloodFill_Replacement, (FARPROC*)&FloodFill_OLD);
	ReplaceImport("Gdi32.dll", "GetROP2", (FARPROC)GetROP2_Replacement, (FARPROC*)&GetROP2_OLD);
	ReplaceImport("Gdi32.dll", "GetAspectRatioFilterEx", (FARPROC)GetAspectRatioFilterEx_Replacement, (FARPROC*)&GetAspectRatioFilterEx_OLD);
	ReplaceImport("Gdi32.dll", "GetBkColor", (FARPROC)GetBkColor_Replacement, (FARPROC*)&GetBkColor_OLD);
	ReplaceImport("Gdi32.dll", "GetDCBrushColor", (FARPROC)GetDCBrushColor_Replacement, (FARPROC*)&GetDCBrushColor_OLD);
	ReplaceImport("Gdi32.dll", "GetDCPenColor", (FARPROC)GetDCPenColor_Replacement, (FARPROC*)&GetDCPenColor_OLD);
	ReplaceImport("Gdi32.dll", "GetBkMode", (FARPROC)GetBkMode_Replacement, (FARPROC*)&GetBkMode_OLD);
	ReplaceImport("Gdi32.dll", "GetBoundsRect", (FARPROC)GetBoundsRect_Replacement, (FARPROC*)&GetBoundsRect_OLD);
	ReplaceImport("Gdi32.dll", "GetBrushOrgEx", (FARPROC)GetBrushOrgEx_Replacement, (FARPROC*)&GetBrushOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidthA", (FARPROC)GetCharWidthA_Replacement, (FARPROC*)&GetCharWidthA_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidthW", (FARPROC)GetCharWidthW_Replacement, (FARPROC*)&GetCharWidthW_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidth32A", (FARPROC)GetCharWidth32A_Replacement, (FARPROC*)&GetCharWidth32A_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidth32W", (FARPROC)GetCharWidth32W_Replacement, (FARPROC*)&GetCharWidth32W_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidthFloatA", (FARPROC)GetCharWidthFloatA_Replacement, (FARPROC*)&GetCharWidthFloatA_OLD);
	ReplaceImport("Gdi32.dll", "GetCharWidthFloatW", (FARPROC)GetCharWidthFloatW_Replacement, (FARPROC*)&GetCharWidthFloatW_OLD);
	ReplaceImport("Gdi32.dll", "GetCharABCWidthsA", (FARPROC)GetCharABCWidthsA_Replacement, (FARPROC*)&GetCharABCWidthsA_OLD);
	ReplaceImport("Gdi32.dll", "GetCharABCWidthsW", (FARPROC)GetCharABCWidthsW_Replacement, (FARPROC*)&GetCharABCWidthsW_OLD);
	ReplaceImport("Gdi32.dll", "GetCharABCWidthsFloatA", (FARPROC)GetCharABCWidthsFloatA_Replacement, (FARPROC*)&GetCharABCWidthsFloatA_OLD);
	ReplaceImport("Gdi32.dll", "GetCharABCWidthsFloatW", (FARPROC)GetCharABCWidthsFloatW_Replacement, (FARPROC*)&GetCharABCWidthsFloatW_OLD);
	ReplaceImport("Gdi32.dll", "GetClipBox", (FARPROC)GetClipBox_Replacement, (FARPROC*)&GetClipBox_OLD);
	ReplaceImport("Gdi32.dll", "GetClipRgn", (FARPROC)GetClipRgn_Replacement, (FARPROC*)&GetClipRgn_OLD);
	ReplaceImport("Gdi32.dll", "GetMetaRgn", (FARPROC)GetMetaRgn_Replacement, (FARPROC*)&GetMetaRgn_OLD);
	ReplaceImport("Gdi32.dll", "GetCurrentObject", (FARPROC)GetCurrentObject_Replacement, (FARPROC*)&GetCurrentObject_OLD);
	ReplaceImport("Gdi32.dll", "GetCurrentPositionEx", (FARPROC)GetCurrentPositionEx_Replacement, (FARPROC*)&GetCurrentPositionEx_OLD);
	ReplaceImport("Gdi32.dll", "GetDeviceCaps", (FARPROC)GetDeviceCaps_Replacement, (FARPROC*)&GetDeviceCaps_OLD);
	ReplaceImport("Gdi32.dll", "GetDIBits", (FARPROC)GetDIBits_Replacement, (FARPROC*)&GetDIBits_OLD);
	ReplaceImport("Gdi32.dll", "GetFontData", (FARPROC)GetFontData_Replacement, (FARPROC*)&GetFontData_OLD);
	ReplaceImport("Gdi32.dll", "GetGlyphOutlineA", (FARPROC)GetGlyphOutlineA_Replacement, (FARPROC*)&GetGlyphOutlineA_OLD);
	ReplaceImport("Gdi32.dll", "GetGlyphOutlineW", (FARPROC)GetGlyphOutlineW_Replacement, (FARPROC*)&GetGlyphOutlineW_OLD);
	ReplaceImport("Gdi32.dll", "GetGraphicsMode", (FARPROC)GetGraphicsMode_Replacement, (FARPROC*)&GetGraphicsMode_OLD);
	ReplaceImport("Gdi32.dll", "GetMapMode", (FARPROC)GetMapMode_Replacement, (FARPROC*)&GetMapMode_OLD);
	ReplaceImport("Gdi32.dll", "GetNearestColor", (FARPROC)GetNearestColor_Replacement, (FARPROC*)&GetNearestColor_OLD);
	ReplaceImport("Gdi32.dll", "GetOutlineTextMetricsA", (FARPROC)GetOutlineTextMetricsA_Replacement, (FARPROC*)&GetOutlineTextMetricsA_OLD);
	ReplaceImport("Gdi32.dll", "GetOutlineTextMetricsW", (FARPROC)GetOutlineTextMetricsW_Replacement, (FARPROC*)&GetOutlineTextMetricsW_OLD);
	ReplaceImport("Gdi32.dll", "GetPixel", (FARPROC)GetPixel_Replacement, (FARPROC*)&GetPixel_OLD);
	ReplaceImport("Gdi32.dll", "GetPixelFormat", (FARPROC)GetPixelFormat_Replacement, (FARPROC*)&GetPixelFormat_OLD);
	ReplaceImport("Gdi32.dll", "GetPolyFillMode", (FARPROC)GetPolyFillMode_Replacement, (FARPROC*)&GetPolyFillMode_OLD);
	ReplaceImport("Gdi32.dll", "GetRandomRgn", (FARPROC)GetRandomRgn_Replacement, (FARPROC*)&GetRandomRgn_OLD);
	ReplaceImport("Gdi32.dll", "GetStretchBltMode", (FARPROC)GetStretchBltMode_Replacement, (FARPROC*)&GetStretchBltMode_OLD);
	ReplaceImport("Gdi32.dll", "GetSystemPaletteEntries", (FARPROC)GetSystemPaletteEntries_Replacement, (FARPROC*)&GetSystemPaletteEntries_OLD);
	ReplaceImport("Gdi32.dll", "GetSystemPaletteUse", (FARPROC)GetSystemPaletteUse_Replacement, (FARPROC*)&GetSystemPaletteUse_OLD);
	ReplaceImport("Gdi32.dll", "GetTextCharacterExtra", (FARPROC)GetTextCharacterExtra_Replacement, (FARPROC*)&GetTextCharacterExtra_OLD);
	ReplaceImport("Gdi32.dll", "GetTextAlign", (FARPROC)GetTextAlign_Replacement, (FARPROC*)&GetTextAlign_OLD);
	ReplaceImport("Gdi32.dll", "GetTextColor", (FARPROC)GetTextColor_Replacement, (FARPROC*)&GetTextColor_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentPointA", (FARPROC)GetTextExtentPointA_Replacement, (FARPROC*)&GetTextExtentPointA_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentPointW", (FARPROC)GetTextExtentPointW_Replacement, (FARPROC*)&GetTextExtentPointW_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentPoint32A", (FARPROC)GetTextExtentPoint32A_Replacement, (FARPROC*)&GetTextExtentPoint32A_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentPoint32W", (FARPROC)GetTextExtentPoint32W_Replacement, (FARPROC*)&GetTextExtentPoint32W_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentExPointA", (FARPROC)GetTextExtentExPointA_Replacement, (FARPROC*)&GetTextExtentExPointA_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentExPointW", (FARPROC)GetTextExtentExPointW_Replacement, (FARPROC*)&GetTextExtentExPointW_OLD);
	ReplaceImport("Gdi32.dll", "GetTextCharset", (FARPROC)GetTextCharset_Replacement, (FARPROC*)&GetTextCharset_OLD);
	ReplaceImport("Gdi32.dll", "GetTextCharsetInfo", (FARPROC)GetTextCharsetInfo_Replacement, (FARPROC*)&GetTextCharsetInfo_OLD);
	ReplaceImport("Gdi32.dll", "GetFontLanguageInfo", (FARPROC)GetFontLanguageInfo_Replacement, (FARPROC*)&GetFontLanguageInfo_OLD);
	ReplaceImport("Gdi32.dll", "GetCharacterPlacementA", (FARPROC)GetCharacterPlacementA_Replacement, (FARPROC*)&GetCharacterPlacementA_OLD);
	ReplaceImport("Gdi32.dll", "GetCharacterPlacementW", (FARPROC)GetCharacterPlacementW_Replacement, (FARPROC*)&GetCharacterPlacementW_OLD);
	ReplaceImport("Gdi32.dll", "GetFontUnicodeRanges", (FARPROC)GetFontUnicodeRanges_Replacement, (FARPROC*)&GetFontUnicodeRanges_OLD);
	ReplaceImport("Gdi32.dll", "GetGlyphIndicesA", (FARPROC)GetGlyphIndicesA_Replacement, (FARPROC*)&GetGlyphIndicesA_OLD);
	ReplaceImport("Gdi32.dll", "GetGlyphIndicesW", (FARPROC)GetGlyphIndicesW_Replacement, (FARPROC*)&GetGlyphIndicesW_OLD);
	ReplaceImport("Gdi32.dll", "GetTextExtentExPointI", (FARPROC)GetTextExtentExPointI_Replacement, (FARPROC*)&GetTextExtentExPointI_OLD);
	ReplaceImport("Gdi32.dll", "GetViewportExtEx", (FARPROC)GetViewportExtEx_Replacement, (FARPROC*)&GetViewportExtEx_OLD);
	ReplaceImport("Gdi32.dll", "GetViewportOrgEx", (FARPROC)GetViewportOrgEx_Replacement, (FARPROC*)&GetViewportOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "GetWindowExtEx", (FARPROC)GetWindowExtEx_Replacement, (FARPROC*)&GetWindowExtEx_OLD);
	ReplaceImport("Gdi32.dll", "GetWindowOrgEx", (FARPROC)GetWindowOrgEx_Replacement, (FARPROC*)&GetWindowOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "IntersectClipRect", (FARPROC)IntersectClipRect_Replacement, (FARPROC*)&IntersectClipRect_OLD);
	ReplaceImport("Gdi32.dll", "InvertRgn", (FARPROC)InvertRgn_Replacement, (FARPROC*)&InvertRgn_OLD);
	ReplaceImport("Gdi32.dll", "LineTo", (FARPROC)LineTo_Replacement, (FARPROC*)&LineTo_OLD);
	ReplaceImport("Gdi32.dll", "MaskBlt", (FARPROC)MaskBlt_Replacement, (FARPROC*)&MaskBlt_OLD);
	ReplaceImport("Gdi32.dll", "PlgBlt", (FARPROC)PlgBlt_Replacement, (FARPROC*)&PlgBlt_OLD);
	ReplaceImport("Gdi32.dll", "OffsetClipRgn", (FARPROC)OffsetClipRgn_Replacement, (FARPROC*)&OffsetClipRgn_OLD);
	ReplaceImport("Gdi32.dll", "PatBlt", (FARPROC)PatBlt_Replacement, (FARPROC*)&PatBlt_OLD);
	ReplaceImport("Gdi32.dll", "Pie", (FARPROC)Pie_Replacement, (FARPROC*)&Pie_OLD);
	ReplaceImport("Gdi32.dll", "PlayMetaFile", (FARPROC)PlayMetaFile_Replacement, (FARPROC*)&PlayMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "PaintRgn", (FARPROC)PaintRgn_Replacement, (FARPROC*)&PaintRgn_OLD);
	ReplaceImport("Gdi32.dll", "PolyPolygon", (FARPROC)PolyPolygon_Replacement, (FARPROC*)&PolyPolygon_OLD);
	ReplaceImport("Gdi32.dll", "PtVisible", (FARPROC)PtVisible_Replacement, (FARPROC*)&PtVisible_OLD);
	ReplaceImport("Gdi32.dll", "RectVisible", (FARPROC)RectVisible_Replacement, (FARPROC*)&RectVisible_OLD);
	ReplaceImport("Gdi32.dll", "Rectangle", (FARPROC)Rectangle_Replacement, (FARPROC*)&Rectangle_OLD);
	ReplaceImport("Gdi32.dll", "RestoreDC", (FARPROC)RestoreDC_Replacement, (FARPROC*)&RestoreDC_OLD);
	ReplaceImport("Gdi32.dll", "ResetDCA", (FARPROC)ResetDCA_Replacement, (FARPROC*)&ResetDCA_OLD);
	ReplaceImport("Gdi32.dll", "ResetDCW", (FARPROC)ResetDCW_Replacement, (FARPROC*)&ResetDCW_OLD);
	ReplaceImport("Gdi32.dll", "RealizePalette", (FARPROC)RealizePalette_Replacement, (FARPROC*)&RealizePalette_OLD);
	ReplaceImport("Gdi32.dll", "RoundRect", (FARPROC)RoundRect_Replacement, (FARPROC*)&RoundRect_OLD);
	ReplaceImport("Gdi32.dll", "SaveDC", (FARPROC)SaveDC_Replacement, (FARPROC*)&SaveDC_OLD);
	ReplaceImport("Gdi32.dll", "SelectClipRgn", (FARPROC)SelectClipRgn_Replacement, (FARPROC*)&SelectClipRgn_OLD);
	ReplaceImport("Gdi32.dll", "ExtSelectClipRgn", (FARPROC)ExtSelectClipRgn_Replacement, (FARPROC*)&ExtSelectClipRgn_OLD);
	ReplaceImport("Gdi32.dll", "SetMetaRgn", (FARPROC)SetMetaRgn_Replacement, (FARPROC*)&SetMetaRgn_OLD);
	ReplaceImport("Gdi32.dll", "SelectObject", (FARPROC)SelectObject_Replacement, (FARPROC*)&SelectObject_OLD);
	ReplaceImport("Gdi32.dll", "SelectPalette", (FARPROC)SelectPalette_Replacement, (FARPROC*)&SelectPalette_OLD);
	ReplaceImport("Gdi32.dll", "SetBkColor", (FARPROC)SetBkColor_Replacement, (FARPROC*)&SetBkColor_OLD);
	ReplaceImport("Gdi32.dll", "SetDCBrushColor", (FARPROC)SetDCBrushColor_Replacement, (FARPROC*)&SetDCBrushColor_OLD);
	ReplaceImport("Gdi32.dll", "SetDCPenColor", (FARPROC)SetDCPenColor_Replacement, (FARPROC*)&SetDCPenColor_OLD);
	ReplaceImport("Gdi32.dll", "SetBkMode", (FARPROC)SetBkMode_Replacement, (FARPROC*)&SetBkMode_OLD);
	ReplaceImport("Gdi32.dll", "SetBoundsRect", (FARPROC)SetBoundsRect_Replacement, (FARPROC*)&SetBoundsRect_OLD);
	ReplaceImport("Gdi32.dll", "SetDIBits", (FARPROC)SetDIBits_Replacement, (FARPROC*)&SetDIBits_OLD);
	ReplaceImport("Gdi32.dll", "SetDIBitsToDevice", (FARPROC)SetDIBitsToDevice_Replacement, (FARPROC*)&SetDIBitsToDevice_OLD);
	ReplaceImport("Gdi32.dll", "SetMapperFlags", (FARPROC)SetMapperFlags_Replacement, (FARPROC*)&SetMapperFlags_OLD);
	ReplaceImport("Gdi32.dll", "SetGraphicsMode", (FARPROC)SetGraphicsMode_Replacement, (FARPROC*)&SetGraphicsMode_OLD);
	ReplaceImport("Gdi32.dll", "SetMapMode", (FARPROC)SetMapMode_Replacement, (FARPROC*)&SetMapMode_OLD);
	ReplaceImport("Gdi32.dll", "SetLayout", (FARPROC)SetLayout_Replacement, (FARPROC*)&SetLayout_OLD);
	ReplaceImport("Gdi32.dll", "GetLayout", (FARPROC)GetLayout_Replacement, (FARPROC*)&GetLayout_OLD);
	ReplaceImport("Gdi32.dll", "SetPixel", (FARPROC)SetPixel_Replacement, (FARPROC*)&SetPixel_OLD);
	ReplaceImport("Gdi32.dll", "SetPixelV", (FARPROC)SetPixelV_Replacement, (FARPROC*)&SetPixelV_OLD);
	ReplaceImport("Gdi32.dll", "SetPixelFormat", (FARPROC)SetPixelFormat_Replacement, (FARPROC*)&SetPixelFormat_OLD);
	ReplaceImport("Gdi32.dll", "SetPolyFillMode", (FARPROC)SetPolyFillMode_Replacement, (FARPROC*)&SetPolyFillMode_OLD);
	ReplaceImport("Gdi32.dll", "StretchBlt", (FARPROC)StretchBlt_Replacement, (FARPROC*)&StretchBlt_OLD);
	ReplaceImport("Gdi32.dll", "StretchDIBits", (FARPROC)StretchDIBits_Replacement, (FARPROC*)&StretchDIBits_OLD);
	ReplaceImport("Gdi32.dll", "SetROP2", (FARPROC)SetROP2_Replacement, (FARPROC*)&SetROP2_OLD);
	ReplaceImport("Gdi32.dll", "SetStretchBltMode", (FARPROC)SetStretchBltMode_Replacement, (FARPROC*)&SetStretchBltMode_OLD);
	ReplaceImport("Gdi32.dll", "SetSystemPaletteUse", (FARPROC)SetSystemPaletteUse_Replacement, (FARPROC*)&SetSystemPaletteUse_OLD);
	ReplaceImport("Gdi32.dll", "SetTextCharacterExtra", (FARPROC)SetTextCharacterExtra_Replacement, (FARPROC*)&SetTextCharacterExtra_OLD);
	ReplaceImport("Gdi32.dll", "SetTextColor", (FARPROC)SetTextColor_Replacement, (FARPROC*)&SetTextColor_OLD);
	ReplaceImport("Gdi32.dll", "SetTextAlign", (FARPROC)SetTextAlign_Replacement, (FARPROC*)&SetTextAlign_OLD);
	ReplaceImport("Gdi32.dll", "SetTextJustification", (FARPROC)SetTextJustification_Replacement, (FARPROC*)&SetTextJustification_OLD);
	ReplaceImport("Gdi32.dll", "UpdateColors", (FARPROC)UpdateColors_Replacement, (FARPROC*)&UpdateColors_OLD);
	ReplaceImport("Gdi32.dll", "AlphaBlend", (FARPROC)AlphaBlend_Replacement, (FARPROC*)&AlphaBlend_OLD);
	ReplaceImport("Gdi32.dll", "TransparentBlt", (FARPROC)TransparentBlt_Replacement, (FARPROC*)&TransparentBlt_OLD);
	ReplaceImport("Gdi32.dll", "GradientFill", (FARPROC)GradientFill_Replacement, (FARPROC*)&GradientFill_OLD);
	ReplaceImport("Gdi32.dll", "PlayMetaFileRecord", (FARPROC)PlayMetaFileRecord_Replacement, (FARPROC*)&PlayMetaFileRecord_OLD);
	ReplaceImport("Gdi32.dll", "EnumMetaFile", (FARPROC)EnumMetaFile_Replacement, (FARPROC*)&EnumMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "CloseEnhMetaFile", (FARPROC)CloseEnhMetaFile_Replacement, (FARPROC*)&CloseEnhMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "CreateEnhMetaFileA", (FARPROC)CreateEnhMetaFileA_Replacement, (FARPROC*)&CreateEnhMetaFileA_OLD);
	ReplaceImport("Gdi32.dll", "CreateEnhMetaFileW", (FARPROC)CreateEnhMetaFileW_Replacement, (FARPROC*)&CreateEnhMetaFileW_OLD);
	ReplaceImport("Gdi32.dll", "EnumEnhMetaFile", (FARPROC)EnumEnhMetaFile_Replacement, (FARPROC*)&EnumEnhMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "GetWinMetaFileBits", (FARPROC)GetWinMetaFileBits_Replacement, (FARPROC*)&GetWinMetaFileBits_OLD);
	ReplaceImport("Gdi32.dll", "PlayEnhMetaFile", (FARPROC)PlayEnhMetaFile_Replacement, (FARPROC*)&PlayEnhMetaFile_OLD);
	ReplaceImport("Gdi32.dll", "PlayEnhMetaFileRecord", (FARPROC)PlayEnhMetaFileRecord_Replacement, (FARPROC*)&PlayEnhMetaFileRecord_OLD);
	ReplaceImport("Gdi32.dll", "SetWinMetaFileBits", (FARPROC)SetWinMetaFileBits_Replacement, (FARPROC*)&SetWinMetaFileBits_OLD);
	ReplaceImport("Gdi32.dll", "GdiComment", (FARPROC)GdiComment_Replacement, (FARPROC*)&GdiComment_OLD);
	ReplaceImport("Gdi32.dll", "GetTextMetricsA", (FARPROC)GetTextMetricsA_Replacement, (FARPROC*)&GetTextMetricsA_OLD);
	ReplaceImport("Gdi32.dll", "GetTextMetricsW", (FARPROC)GetTextMetricsW_Replacement, (FARPROC*)&GetTextMetricsW_OLD);
	ReplaceImport("Gdi32.dll", "AngleArc", (FARPROC)AngleArc_Replacement, (FARPROC*)&AngleArc_OLD);
	ReplaceImport("Gdi32.dll", "PolyPolyline", (FARPROC)PolyPolyline_Replacement, (FARPROC*)&PolyPolyline_OLD);
	ReplaceImport("Gdi32.dll", "GetWorldTransform", (FARPROC)GetWorldTransform_Replacement, (FARPROC*)&GetWorldTransform_OLD);
	ReplaceImport("Gdi32.dll", "SetWorldTransform", (FARPROC)SetWorldTransform_Replacement, (FARPROC*)&SetWorldTransform_OLD);
	ReplaceImport("Gdi32.dll", "ModifyWorldTransform", (FARPROC)ModifyWorldTransform_Replacement, (FARPROC*)&ModifyWorldTransform_OLD);
	ReplaceImport("Gdi32.dll", "CreateDIBSection", (FARPROC)CreateDIBSection_Replacement, (FARPROC*)&CreateDIBSection_OLD);
	ReplaceImport("Gdi32.dll", "GetDIBColorTable", (FARPROC)GetDIBColorTable_Replacement, (FARPROC*)&GetDIBColorTable_OLD);
	ReplaceImport("Gdi32.dll", "SetDIBColorTable", (FARPROC)SetDIBColorTable_Replacement, (FARPROC*)&SetDIBColorTable_OLD);
	ReplaceImport("Gdi32.dll", "SetColorAdjustment", (FARPROC)SetColorAdjustment_Replacement, (FARPROC*)&SetColorAdjustment_OLD);
	ReplaceImport("Gdi32.dll", "GetColorAdjustment", (FARPROC)GetColorAdjustment_Replacement, (FARPROC*)&GetColorAdjustment_OLD);
	ReplaceImport("Gdi32.dll", "CreateHalftonePalette", (FARPROC)CreateHalftonePalette_Replacement, (FARPROC*)&CreateHalftonePalette_OLD);
	ReplaceImport("Gdi32.dll", "StartDocA", (FARPROC)StartDocA_Replacement, (FARPROC*)&StartDocA_OLD);
	ReplaceImport("Gdi32.dll", "StartDocW", (FARPROC)StartDocW_Replacement, (FARPROC*)&StartDocW_OLD);
	ReplaceImport("Gdi32.dll", "EndDoc", (FARPROC)EndDoc_Replacement, (FARPROC*)&EndDoc_OLD);
	ReplaceImport("Gdi32.dll", "StartPage", (FARPROC)StartPage_Replacement, (FARPROC*)&StartPage_OLD);
	ReplaceImport("Gdi32.dll", "EndPage", (FARPROC)EndPage_Replacement, (FARPROC*)&EndPage_OLD);
	ReplaceImport("Gdi32.dll", "AbortDoc", (FARPROC)AbortDoc_Replacement, (FARPROC*)&AbortDoc_OLD);
	ReplaceImport("Gdi32.dll", "SetAbortProc", (FARPROC)SetAbortProc_Replacement, (FARPROC*)&SetAbortProc_OLD);
	ReplaceImport("Gdi32.dll", "AbortPath", (FARPROC)AbortPath_Replacement, (FARPROC*)&AbortPath_OLD);
	ReplaceImport("Gdi32.dll", "ArcTo", (FARPROC)ArcTo_Replacement, (FARPROC*)&ArcTo_OLD);
	ReplaceImport("Gdi32.dll", "BeginPath", (FARPROC)BeginPath_Replacement, (FARPROC*)&BeginPath_OLD);
	ReplaceImport("Gdi32.dll", "CloseFigure", (FARPROC)CloseFigure_Replacement, (FARPROC*)&CloseFigure_OLD);
	ReplaceImport("Gdi32.dll", "EndPath", (FARPROC)EndPath_Replacement, (FARPROC*)&EndPath_OLD);
	ReplaceImport("Gdi32.dll", "FillPath", (FARPROC)FillPath_Replacement, (FARPROC*)&FillPath_OLD);
	ReplaceImport("Gdi32.dll", "FlattenPath", (FARPROC)FlattenPath_Replacement, (FARPROC*)&FlattenPath_OLD);
	ReplaceImport("Gdi32.dll", "GetPath", (FARPROC)GetPath_Replacement, (FARPROC*)&GetPath_OLD);
	ReplaceImport("Gdi32.dll", "PathToRegion", (FARPROC)PathToRegion_Replacement, (FARPROC*)&PathToRegion_OLD);
	ReplaceImport("Gdi32.dll", "PolyDraw", (FARPROC)PolyDraw_Replacement, (FARPROC*)&PolyDraw_OLD);
	ReplaceImport("Gdi32.dll", "SelectClipPath", (FARPROC)SelectClipPath_Replacement, (FARPROC*)&SelectClipPath_OLD);
	ReplaceImport("Gdi32.dll", "SetArcDirection", (FARPROC)SetArcDirection_Replacement, (FARPROC*)&SetArcDirection_OLD);
	ReplaceImport("Gdi32.dll", "SetMiterLimit", (FARPROC)SetMiterLimit_Replacement, (FARPROC*)&SetMiterLimit_OLD);
	ReplaceImport("Gdi32.dll", "StrokeAndFillPath", (FARPROC)StrokeAndFillPath_Replacement, (FARPROC*)&StrokeAndFillPath_OLD);
	ReplaceImport("Gdi32.dll", "StrokePath", (FARPROC)StrokePath_Replacement, (FARPROC*)&StrokePath_OLD);
	ReplaceImport("Gdi32.dll", "WidenPath", (FARPROC)WidenPath_Replacement, (FARPROC*)&WidenPath_OLD);
	ReplaceImport("Gdi32.dll", "GetMiterLimit", (FARPROC)GetMiterLimit_Replacement, (FARPROC*)&GetMiterLimit_OLD);
	ReplaceImport("Gdi32.dll", "GetArcDirection", (FARPROC)GetArcDirection_Replacement, (FARPROC*)&GetArcDirection_OLD);
	ReplaceImport("Gdi32.dll", "MoveToEx", (FARPROC)MoveToEx_Replacement, (FARPROC*)&MoveToEx_OLD);
	ReplaceImport("Gdi32.dll", "TextOutA", (FARPROC)TextOutA_Replacement, (FARPROC*)&TextOutA_OLD);
	ReplaceImport("Gdi32.dll", "TextOutW", (FARPROC)TextOutW_Replacement, (FARPROC*)&TextOutW_OLD);
	ReplaceImport("Gdi32.dll", "ExtTextOutA", (FARPROC)ExtTextOutA_Replacement, (FARPROC*)&ExtTextOutA_OLD);
	ReplaceImport("Gdi32.dll", "ExtTextOutW", (FARPROC)ExtTextOutW_Replacement, (FARPROC*)&ExtTextOutW_OLD);
	ReplaceImport("Gdi32.dll", "PolyTextOutA", (FARPROC)PolyTextOutA_Replacement, (FARPROC*)&PolyTextOutA_OLD);
	ReplaceImport("Gdi32.dll", "PolyTextOutW", (FARPROC)PolyTextOutW_Replacement, (FARPROC*)&PolyTextOutW_OLD);
	ReplaceImport("Gdi32.dll", "DPtoLP", (FARPROC)DPtoLP_Replacement, (FARPROC*)&DPtoLP_OLD);
	ReplaceImport("Gdi32.dll", "LPtoDP", (FARPROC)LPtoDP_Replacement, (FARPROC*)&LPtoDP_OLD);
	ReplaceImport("Gdi32.dll", "Polygon", (FARPROC)Polygon_Replacement, (FARPROC*)&Polygon_OLD);
	ReplaceImport("Gdi32.dll", "Polyline", (FARPROC)Polyline_Replacement, (FARPROC*)&Polyline_OLD);
	ReplaceImport("Gdi32.dll", "PolyBezier", (FARPROC)PolyBezier_Replacement, (FARPROC*)&PolyBezier_OLD);
	ReplaceImport("Gdi32.dll", "PolyBezierTo", (FARPROC)PolyBezierTo_Replacement, (FARPROC*)&PolyBezierTo_OLD);
	ReplaceImport("Gdi32.dll", "PolylineTo", (FARPROC)PolylineTo_Replacement, (FARPROC*)&PolylineTo_OLD);
	ReplaceImport("Gdi32.dll", "SetViewportExtEx", (FARPROC)SetViewportExtEx_Replacement, (FARPROC*)&SetViewportExtEx_OLD);
	ReplaceImport("Gdi32.dll", "SetViewportOrgEx", (FARPROC)SetViewportOrgEx_Replacement, (FARPROC*)&SetViewportOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "SetWindowExtEx", (FARPROC)SetWindowExtEx_Replacement, (FARPROC*)&SetWindowExtEx_OLD);
	ReplaceImport("Gdi32.dll", "SetWindowOrgEx", (FARPROC)SetWindowOrgEx_Replacement, (FARPROC*)&SetWindowOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "OffsetViewportOrgEx", (FARPROC)OffsetViewportOrgEx_Replacement, (FARPROC*)&OffsetViewportOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "OffsetWindowOrgEx", (FARPROC)OffsetWindowOrgEx_Replacement, (FARPROC*)&OffsetWindowOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "ScaleViewportExtEx", (FARPROC)ScaleViewportExtEx_Replacement, (FARPROC*)&ScaleViewportExtEx_OLD);
	ReplaceImport("Gdi32.dll", "ScaleWindowExtEx", (FARPROC)ScaleWindowExtEx_Replacement, (FARPROC*)&ScaleWindowExtEx_OLD);
	ReplaceImport("Gdi32.dll", "SetBrushOrgEx", (FARPROC)SetBrushOrgEx_Replacement, (FARPROC*)&SetBrushOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "GetTextFaceA", (FARPROC)GetTextFaceA_Replacement, (FARPROC*)&GetTextFaceA_OLD);
	ReplaceImport("Gdi32.dll", "GetTextFaceW", (FARPROC)GetTextFaceW_Replacement, (FARPROC*)&GetTextFaceW_OLD);
	ReplaceImport("Gdi32.dll", "GetKerningPairsA", (FARPROC)GetKerningPairsA_Replacement, (FARPROC*)&GetKerningPairsA_OLD);
	ReplaceImport("Gdi32.dll", "GetKerningPairsW", (FARPROC)GetKerningPairsW_Replacement, (FARPROC*)&GetKerningPairsW_OLD);
	ReplaceImport("Gdi32.dll", "GetDCOrgEx", (FARPROC)GetDCOrgEx_Replacement, (FARPROC*)&GetDCOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "FixBrushOrgEx", (FARPROC)FixBrushOrgEx_Replacement, (FARPROC*)&FixBrushOrgEx_OLD);
	ReplaceImport("Gdi32.dll", "SetICMMode", (FARPROC)SetICMMode_Replacement, (FARPROC*)&SetICMMode_OLD);
	ReplaceImport("Gdi32.dll", "CheckColorsInGamut", (FARPROC)CheckColorsInGamut_Replacement, (FARPROC*)&CheckColorsInGamut_OLD);
	ReplaceImport("Gdi32.dll", "GetColorSpace", (FARPROC)GetColorSpace_Replacement, (FARPROC*)&GetColorSpace_OLD);
	ReplaceImport("Gdi32.dll", "SetColorSpace", (FARPROC)SetColorSpace_Replacement, (FARPROC*)&SetColorSpace_OLD);
	ReplaceImport("Gdi32.dll", "GetICMProfileA", (FARPROC)GetICMProfileA_Replacement, (FARPROC*)&GetICMProfileA_OLD);
	ReplaceImport("Gdi32.dll", "GetICMProfileW", (FARPROC)GetICMProfileW_Replacement, (FARPROC*)&GetICMProfileW_OLD);
	ReplaceImport("Gdi32.dll", "SetICMProfileA", (FARPROC)SetICMProfileA_Replacement, (FARPROC*)&SetICMProfileA_OLD);
	ReplaceImport("Gdi32.dll", "SetICMProfileW", (FARPROC)SetICMProfileW_Replacement, (FARPROC*)&SetICMProfileW_OLD);
	ReplaceImport("Gdi32.dll", "GetDeviceGammaRamp", (FARPROC)GetDeviceGammaRamp_Replacement, (FARPROC*)&GetDeviceGammaRamp_OLD);
	ReplaceImport("Gdi32.dll", "SetDeviceGammaRamp", (FARPROC)SetDeviceGammaRamp_Replacement, (FARPROC*)&SetDeviceGammaRamp_OLD);
	ReplaceImport("Gdi32.dll", "ColorMatchToTarget", (FARPROC)ColorMatchToTarget_Replacement, (FARPROC*)&ColorMatchToTarget_OLD);
	ReplaceImport("Gdi32.dll", "EnumICMProfilesA", (FARPROC)EnumICMProfilesA_Replacement, (FARPROC*)&EnumICMProfilesA_OLD);
	ReplaceImport("Gdi32.dll", "EnumICMProfilesW", (FARPROC)EnumICMProfilesW_Replacement, (FARPROC*)&EnumICMProfilesW_OLD);
	ReplaceImport("Gdi32.dll", "ColorCorrectPalette", (FARPROC)ColorCorrectPalette_Replacement, (FARPROC*)&ColorCorrectPalette_OLD);
	ReplaceImport("Gdi32.dll", "wglCreateContext", (FARPROC)wglCreateContext_Replacement, (FARPROC*)&wglCreateContext_OLD);
	ReplaceImport("Gdi32.dll", "wglCreateLayerContext", (FARPROC)wglCreateLayerContext_Replacement, (FARPROC*)&wglCreateLayerContext_OLD);
	ReplaceImport("Gdi32.dll", "wglMakeCurrent", (FARPROC)wglMakeCurrent_Replacement, (FARPROC*)&wglMakeCurrent_OLD);
	ReplaceImport("Gdi32.dll", "wglUseFontBitmapsA", (FARPROC)wglUseFontBitmapsA_Replacement, (FARPROC*)&wglUseFontBitmapsA_OLD);
	ReplaceImport("Gdi32.dll", "wglUseFontBitmapsW", (FARPROC)wglUseFontBitmapsW_Replacement, (FARPROC*)&wglUseFontBitmapsW_OLD);
	ReplaceImport("Gdi32.dll", "SwapBuffers", (FARPROC)SwapBuffers_Replacement, (FARPROC*)&SwapBuffers_OLD);
	ReplaceImport("Gdi32.dll", "wglUseFontOutlinesA", (FARPROC)wglUseFontOutlinesA_Replacement, (FARPROC*)&wglUseFontOutlinesA_OLD);
	ReplaceImport("Gdi32.dll", "wglUseFontOutlinesW", (FARPROC)wglUseFontOutlinesW_Replacement, (FARPROC*)&wglUseFontOutlinesW_OLD);
	ReplaceImport("Gdi32.dll", "wglDescribeLayerPlane", (FARPROC)wglDescribeLayerPlane_Replacement, (FARPROC*)&wglDescribeLayerPlane_OLD);
	ReplaceImport("Gdi32.dll", "wglSetLayerPaletteEntries", (FARPROC)wglSetLayerPaletteEntries_Replacement, (FARPROC*)&wglSetLayerPaletteEntries_OLD);
	ReplaceImport("Gdi32.dll", "wglGetLayerPaletteEntries", (FARPROC)wglGetLayerPaletteEntries_Replacement, (FARPROC*)&wglGetLayerPaletteEntries_OLD);
	ReplaceImport("Gdi32.dll", "wglRealizeLayerPalette", (FARPROC)wglRealizeLayerPalette_Replacement, (FARPROC*)&wglRealizeLayerPalette_OLD);
	ReplaceImport("Gdi32.dll", "wglSwapLayerBuffers", (FARPROC)wglSwapLayerBuffers_Replacement, (FARPROC*)&wglSwapLayerBuffers_OLD);
}

//Import Backups (Definitions)
Arc_FUNC Arc_OLD = NULL;
BitBlt_FUNC BitBlt_OLD = NULL;
CancelDC_FUNC CancelDC_OLD = NULL;
Chord_FUNC Chord_OLD = NULL;
ChoosePixelFormat_FUNC ChoosePixelFormat_OLD = NULL;
CloseMetaFile_FUNC CloseMetaFile_OLD = NULL;
CreateCompatibleBitmap_FUNC CreateCompatibleBitmap_OLD = NULL;
CreateDiscardableBitmap_FUNC CreateDiscardableBitmap_OLD = NULL;
CreateCompatibleDC_FUNC CreateCompatibleDC_OLD = NULL;
CreateDIBitmap_FUNC CreateDIBitmap_OLD = NULL;
DeleteDC_FUNC DeleteDC_OLD = NULL;
DescribePixelFormat_FUNC DescribePixelFormat_OLD = NULL;
DrawEscape_FUNC DrawEscape_OLD = NULL;
Ellipse_FUNC Ellipse_OLD = NULL;
EnumFontFamiliesExA_FUNC EnumFontFamiliesExA_OLD = NULL;
EnumFontFamiliesExW_FUNC EnumFontFamiliesExW_OLD = NULL;
EnumFontFamiliesA_FUNC EnumFontFamiliesA_OLD = NULL;
EnumFontFamiliesW_FUNC EnumFontFamiliesW_OLD = NULL;
EnumFontsA_FUNC EnumFontsA_OLD = NULL;
EnumFontsW_FUNC EnumFontsW_OLD = NULL;
EnumObjects_FUNC EnumObjects_OLD = NULL;
Escape_FUNC Escape_OLD = NULL;
ExtEscape_FUNC ExtEscape_OLD = NULL;
ExcludeClipRect_FUNC ExcludeClipRect_OLD = NULL;
ExtFloodFill_FUNC ExtFloodFill_OLD = NULL;
FillRgn_FUNC FillRgn_OLD = NULL;
FloodFill_FUNC FloodFill_OLD = NULL;
GetROP2_FUNC GetROP2_OLD = NULL;
GetAspectRatioFilterEx_FUNC GetAspectRatioFilterEx_OLD = NULL;
GetBkColor_FUNC GetBkColor_OLD = NULL;
GetDCBrushColor_FUNC GetDCBrushColor_OLD = NULL;
GetDCPenColor_FUNC GetDCPenColor_OLD = NULL;
GetBkMode_FUNC GetBkMode_OLD = NULL;
GetBoundsRect_FUNC GetBoundsRect_OLD = NULL;
GetBrushOrgEx_FUNC GetBrushOrgEx_OLD = NULL;
GetCharWidthA_FUNC GetCharWidthA_OLD = NULL;
GetCharWidthW_FUNC GetCharWidthW_OLD = NULL;
GetCharWidth32A_FUNC GetCharWidth32A_OLD = NULL;
GetCharWidth32W_FUNC GetCharWidth32W_OLD = NULL;
GetCharWidthFloatA_FUNC GetCharWidthFloatA_OLD = NULL;
GetCharWidthFloatW_FUNC GetCharWidthFloatW_OLD = NULL;
GetCharABCWidthsA_FUNC GetCharABCWidthsA_OLD = NULL;
GetCharABCWidthsW_FUNC GetCharABCWidthsW_OLD = NULL;
GetCharABCWidthsFloatA_FUNC GetCharABCWidthsFloatA_OLD = NULL;
GetCharABCWidthsFloatW_FUNC GetCharABCWidthsFloatW_OLD = NULL;
GetClipBox_FUNC GetClipBox_OLD = NULL;
GetClipRgn_FUNC GetClipRgn_OLD = NULL;
GetMetaRgn_FUNC GetMetaRgn_OLD = NULL;
GetCurrentObject_FUNC GetCurrentObject_OLD = NULL;
GetCurrentPositionEx_FUNC GetCurrentPositionEx_OLD = NULL;
GetDeviceCaps_FUNC GetDeviceCaps_OLD = NULL;
GetDIBits_FUNC GetDIBits_OLD = NULL;
GetFontData_FUNC GetFontData_OLD = NULL;
GetGlyphOutlineA_FUNC GetGlyphOutlineA_OLD = NULL;
GetGlyphOutlineW_FUNC GetGlyphOutlineW_OLD = NULL;
GetGraphicsMode_FUNC GetGraphicsMode_OLD = NULL;
GetMapMode_FUNC GetMapMode_OLD = NULL;
GetNearestColor_FUNC GetNearestColor_OLD = NULL;
GetOutlineTextMetricsA_FUNC GetOutlineTextMetricsA_OLD = NULL;
GetOutlineTextMetricsW_FUNC GetOutlineTextMetricsW_OLD = NULL;
GetPixel_FUNC GetPixel_OLD = NULL;
GetPixelFormat_FUNC GetPixelFormat_OLD = NULL;
GetPolyFillMode_FUNC GetPolyFillMode_OLD = NULL;
GetRandomRgn_FUNC GetRandomRgn_OLD = NULL;
GetStretchBltMode_FUNC GetStretchBltMode_OLD = NULL;
GetSystemPaletteEntries_FUNC GetSystemPaletteEntries_OLD = NULL;
GetSystemPaletteUse_FUNC GetSystemPaletteUse_OLD = NULL;
GetTextCharacterExtra_FUNC GetTextCharacterExtra_OLD = NULL;
GetTextAlign_FUNC GetTextAlign_OLD = NULL;
GetTextColor_FUNC GetTextColor_OLD = NULL;
GetTextExtentPointA_FUNC GetTextExtentPointA_OLD = NULL;
GetTextExtentPointW_FUNC GetTextExtentPointW_OLD = NULL;
GetTextExtentPoint32A_FUNC GetTextExtentPoint32A_OLD = NULL;
GetTextExtentPoint32W_FUNC GetTextExtentPoint32W_OLD = NULL;
GetTextExtentExPointA_FUNC GetTextExtentExPointA_OLD = NULL;
GetTextExtentExPointW_FUNC GetTextExtentExPointW_OLD = NULL;
GetTextCharset_FUNC GetTextCharset_OLD = NULL;
GetTextCharsetInfo_FUNC GetTextCharsetInfo_OLD = NULL;
GetFontLanguageInfo_FUNC GetFontLanguageInfo_OLD = NULL;
GetCharacterPlacementA_FUNC GetCharacterPlacementA_OLD = NULL;
GetCharacterPlacementW_FUNC GetCharacterPlacementW_OLD = NULL;
GetFontUnicodeRanges_FUNC GetFontUnicodeRanges_OLD = NULL;
GetGlyphIndicesA_FUNC GetGlyphIndicesA_OLD = NULL;
GetGlyphIndicesW_FUNC GetGlyphIndicesW_OLD = NULL;
GetTextExtentExPointI_FUNC GetTextExtentExPointI_OLD = NULL;
GetViewportExtEx_FUNC GetViewportExtEx_OLD = NULL;
GetViewportOrgEx_FUNC GetViewportOrgEx_OLD = NULL;
GetWindowExtEx_FUNC GetWindowExtEx_OLD = NULL;
GetWindowOrgEx_FUNC GetWindowOrgEx_OLD = NULL;
IntersectClipRect_FUNC IntersectClipRect_OLD = NULL;
InvertRgn_FUNC InvertRgn_OLD = NULL;
LineTo_FUNC LineTo_OLD = NULL;
MaskBlt_FUNC MaskBlt_OLD = NULL;
PlgBlt_FUNC PlgBlt_OLD = NULL;
OffsetClipRgn_FUNC OffsetClipRgn_OLD = NULL;
PatBlt_FUNC PatBlt_OLD = NULL;
Pie_FUNC Pie_OLD = NULL;
PlayMetaFile_FUNC PlayMetaFile_OLD = NULL;
PaintRgn_FUNC PaintRgn_OLD = NULL;
PolyPolygon_FUNC PolyPolygon_OLD = NULL;
PtVisible_FUNC PtVisible_OLD = NULL;
RectVisible_FUNC RectVisible_OLD = NULL;
Rectangle_FUNC Rectangle_OLD = NULL;
RestoreDC_FUNC RestoreDC_OLD = NULL;
ResetDCA_FUNC ResetDCA_OLD = NULL;
ResetDCW_FUNC ResetDCW_OLD = NULL;
RealizePalette_FUNC RealizePalette_OLD = NULL;
RoundRect_FUNC RoundRect_OLD = NULL;
SaveDC_FUNC SaveDC_OLD = NULL;
SelectClipRgn_FUNC SelectClipRgn_OLD = NULL;
ExtSelectClipRgn_FUNC ExtSelectClipRgn_OLD = NULL;
SetMetaRgn_FUNC SetMetaRgn_OLD = NULL;
SelectObject_FUNC SelectObject_OLD = NULL;
SelectPalette_FUNC SelectPalette_OLD = NULL;
SetBkColor_FUNC SetBkColor_OLD = NULL;
SetDCBrushColor_FUNC SetDCBrushColor_OLD = NULL;
SetDCPenColor_FUNC SetDCPenColor_OLD = NULL;
SetBkMode_FUNC SetBkMode_OLD = NULL;
SetBoundsRect_FUNC SetBoundsRect_OLD = NULL;
SetDIBits_FUNC SetDIBits_OLD = NULL;
SetDIBitsToDevice_FUNC SetDIBitsToDevice_OLD = NULL;
SetMapperFlags_FUNC SetMapperFlags_OLD = NULL;
SetGraphicsMode_FUNC SetGraphicsMode_OLD = NULL;
SetMapMode_FUNC SetMapMode_OLD = NULL;
SetLayout_FUNC SetLayout_OLD = NULL;
GetLayout_FUNC GetLayout_OLD = NULL;
SetPixel_FUNC SetPixel_OLD = NULL;
SetPixelV_FUNC SetPixelV_OLD = NULL;
SetPixelFormat_FUNC SetPixelFormat_OLD = NULL;
SetPolyFillMode_FUNC SetPolyFillMode_OLD = NULL;
StretchBlt_FUNC StretchBlt_OLD = NULL;
StretchDIBits_FUNC StretchDIBits_OLD = NULL;
SetROP2_FUNC SetROP2_OLD = NULL;
SetStretchBltMode_FUNC SetStretchBltMode_OLD = NULL;
SetSystemPaletteUse_FUNC SetSystemPaletteUse_OLD = NULL;
SetTextCharacterExtra_FUNC SetTextCharacterExtra_OLD = NULL;
SetTextColor_FUNC SetTextColor_OLD = NULL;
SetTextAlign_FUNC SetTextAlign_OLD = NULL;
SetTextJustification_FUNC SetTextJustification_OLD = NULL;
UpdateColors_FUNC UpdateColors_OLD = NULL;
AlphaBlend_FUNC AlphaBlend_OLD = NULL;
TransparentBlt_FUNC TransparentBlt_OLD = NULL;
GradientFill_FUNC GradientFill_OLD = NULL;
PlayMetaFileRecord_FUNC PlayMetaFileRecord_OLD = NULL;
EnumMetaFile_FUNC EnumMetaFile_OLD = NULL;
CloseEnhMetaFile_FUNC CloseEnhMetaFile_OLD = NULL;
CreateEnhMetaFileA_FUNC CreateEnhMetaFileA_OLD = NULL;
CreateEnhMetaFileW_FUNC CreateEnhMetaFileW_OLD = NULL;
EnumEnhMetaFile_FUNC EnumEnhMetaFile_OLD = NULL;
GetWinMetaFileBits_FUNC GetWinMetaFileBits_OLD = NULL;
PlayEnhMetaFile_FUNC PlayEnhMetaFile_OLD = NULL;
PlayEnhMetaFileRecord_FUNC PlayEnhMetaFileRecord_OLD = NULL;
SetWinMetaFileBits_FUNC SetWinMetaFileBits_OLD = NULL;
GdiComment_FUNC GdiComment_OLD = NULL;
GetTextMetricsA_FUNC GetTextMetricsA_OLD = NULL;
GetTextMetricsW_FUNC GetTextMetricsW_OLD = NULL;
AngleArc_FUNC AngleArc_OLD = NULL;
PolyPolyline_FUNC PolyPolyline_OLD = NULL;
GetWorldTransform_FUNC GetWorldTransform_OLD = NULL;
SetWorldTransform_FUNC SetWorldTransform_OLD = NULL;
ModifyWorldTransform_FUNC ModifyWorldTransform_OLD = NULL;
CreateDIBSection_FUNC CreateDIBSection_OLD = NULL;
GetDIBColorTable_FUNC GetDIBColorTable_OLD = NULL;
SetDIBColorTable_FUNC SetDIBColorTable_OLD = NULL;
SetColorAdjustment_FUNC SetColorAdjustment_OLD = NULL;
GetColorAdjustment_FUNC GetColorAdjustment_OLD = NULL;
CreateHalftonePalette_FUNC CreateHalftonePalette_OLD = NULL;
StartDocA_FUNC StartDocA_OLD = NULL;
StartDocW_FUNC StartDocW_OLD = NULL;
EndDoc_FUNC EndDoc_OLD = NULL;
StartPage_FUNC StartPage_OLD = NULL;
EndPage_FUNC EndPage_OLD = NULL;
AbortDoc_FUNC AbortDoc_OLD = NULL;
SetAbortProc_FUNC SetAbortProc_OLD = NULL;
AbortPath_FUNC AbortPath_OLD = NULL;
ArcTo_FUNC ArcTo_OLD = NULL;
BeginPath_FUNC BeginPath_OLD = NULL;
CloseFigure_FUNC CloseFigure_OLD = NULL;
EndPath_FUNC EndPath_OLD = NULL;
FillPath_FUNC FillPath_OLD = NULL;
FlattenPath_FUNC FlattenPath_OLD = NULL;
GetPath_FUNC GetPath_OLD = NULL;
PathToRegion_FUNC PathToRegion_OLD = NULL;
PolyDraw_FUNC PolyDraw_OLD = NULL;
SelectClipPath_FUNC SelectClipPath_OLD = NULL;
SetArcDirection_FUNC SetArcDirection_OLD = NULL;
SetMiterLimit_FUNC SetMiterLimit_OLD = NULL;
StrokeAndFillPath_FUNC StrokeAndFillPath_OLD = NULL;
StrokePath_FUNC StrokePath_OLD = NULL;
WidenPath_FUNC WidenPath_OLD = NULL;
GetMiterLimit_FUNC GetMiterLimit_OLD = NULL;
GetArcDirection_FUNC GetArcDirection_OLD = NULL;
MoveToEx_FUNC MoveToEx_OLD = NULL;
TextOutA_FUNC TextOutA_OLD = NULL;
TextOutW_FUNC TextOutW_OLD = NULL;
ExtTextOutA_FUNC ExtTextOutA_OLD = NULL;
ExtTextOutW_FUNC ExtTextOutW_OLD = NULL;
PolyTextOutA_FUNC PolyTextOutA_OLD = NULL;
PolyTextOutW_FUNC PolyTextOutW_OLD = NULL;
DPtoLP_FUNC DPtoLP_OLD = NULL;
LPtoDP_FUNC LPtoDP_OLD = NULL;
Polygon_FUNC Polygon_OLD = NULL;
Polyline_FUNC Polyline_OLD = NULL;
PolyBezier_FUNC PolyBezier_OLD = NULL;
PolyBezierTo_FUNC PolyBezierTo_OLD = NULL;
PolylineTo_FUNC PolylineTo_OLD = NULL;
SetViewportExtEx_FUNC SetViewportExtEx_OLD = NULL;
SetViewportOrgEx_FUNC SetViewportOrgEx_OLD = NULL;
SetWindowExtEx_FUNC SetWindowExtEx_OLD = NULL;
SetWindowOrgEx_FUNC SetWindowOrgEx_OLD = NULL;
OffsetViewportOrgEx_FUNC OffsetViewportOrgEx_OLD = NULL;
OffsetWindowOrgEx_FUNC OffsetWindowOrgEx_OLD = NULL;
ScaleViewportExtEx_FUNC ScaleViewportExtEx_OLD = NULL;
ScaleWindowExtEx_FUNC ScaleWindowExtEx_OLD = NULL;
SetBrushOrgEx_FUNC SetBrushOrgEx_OLD = NULL;
GetTextFaceA_FUNC GetTextFaceA_OLD = NULL;
GetTextFaceW_FUNC GetTextFaceW_OLD = NULL;
GetKerningPairsA_FUNC GetKerningPairsA_OLD = NULL;
GetKerningPairsW_FUNC GetKerningPairsW_OLD = NULL;
GetDCOrgEx_FUNC GetDCOrgEx_OLD = NULL;
FixBrushOrgEx_FUNC FixBrushOrgEx_OLD = NULL;
SetICMMode_FUNC SetICMMode_OLD = NULL;
CheckColorsInGamut_FUNC CheckColorsInGamut_OLD = NULL;
GetColorSpace_FUNC GetColorSpace_OLD = NULL;
SetColorSpace_FUNC SetColorSpace_OLD = NULL;
GetICMProfileA_FUNC GetICMProfileA_OLD = NULL;
GetICMProfileW_FUNC GetICMProfileW_OLD = NULL;
SetICMProfileA_FUNC SetICMProfileA_OLD = NULL;
SetICMProfileW_FUNC SetICMProfileW_OLD = NULL;
GetDeviceGammaRamp_FUNC GetDeviceGammaRamp_OLD = NULL;
SetDeviceGammaRamp_FUNC SetDeviceGammaRamp_OLD = NULL;
ColorMatchToTarget_FUNC ColorMatchToTarget_OLD = NULL;
EnumICMProfilesA_FUNC EnumICMProfilesA_OLD = NULL;
EnumICMProfilesW_FUNC EnumICMProfilesW_OLD = NULL;
ColorCorrectPalette_FUNC ColorCorrectPalette_OLD = NULL;
wglCreateContext_FUNC wglCreateContext_OLD = NULL;
wglCreateLayerContext_FUNC wglCreateLayerContext_OLD = NULL;
wglMakeCurrent_FUNC wglMakeCurrent_OLD = NULL;
wglUseFontBitmapsA_FUNC wglUseFontBitmapsA_OLD = NULL;
wglUseFontBitmapsW_FUNC wglUseFontBitmapsW_OLD = NULL;
SwapBuffers_FUNC SwapBuffers_OLD = NULL;
wglUseFontOutlinesA_FUNC wglUseFontOutlinesA_OLD = NULL;
wglUseFontOutlinesW_FUNC wglUseFontOutlinesW_OLD = NULL;
wglDescribeLayerPlane_FUNC wglDescribeLayerPlane_OLD = NULL;
wglSetLayerPaletteEntries_FUNC wglSetLayerPaletteEntries_OLD = NULL;
wglGetLayerPaletteEntries_FUNC wglGetLayerPaletteEntries_OLD = NULL;
wglRealizeLayerPalette_FUNC wglRealizeLayerPalette_OLD = NULL;
wglSwapLayerBuffers_FUNC wglSwapLayerBuffers_OLD = NULL;

//Replacement Functions Code
BOOL WINAPI Arc_Replacement(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	SubstituteDC(hdc);
	return Arc_OLD(hdc, x1, y1, x2, y2, x3, y3, x4, y4);
}
BOOL WINAPI BitBlt_Replacement(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	SubstituteDC(hdc);
	return BitBlt_OLD(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}
BOOL WINAPI CancelDC_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CancelDC_OLD(hdc);
}
BOOL WINAPI Chord_Replacement(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	SubstituteDC(hdc);
	return Chord_OLD(hdc, x1, y1, x2, y2, x3, y3, x4, y4);
}
int WINAPI ChoosePixelFormat_Replacement(HDC hdc, CONST PIXELFORMATDESCRIPTOR* ppfd)
{
	SubstituteDC(hdc);
	return ChoosePixelFormat_OLD(hdc, ppfd);
}
HMETAFILE WINAPI CloseMetaFile_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CloseMetaFile_OLD(hdc);
}
HBITMAP WINAPI CreateCompatibleBitmap_Replacement(HDC hdc, int cx, int cy)
{
	SubstituteDC(hdc);
	return CreateCompatibleBitmap_OLD(hdc, cx, cy);
}
HBITMAP WINAPI CreateDiscardableBitmap_Replacement(HDC hdc, int cx, int cy)
{
	SubstituteDC(hdc);
	return CreateDiscardableBitmap_OLD(hdc, cx, cy);
}
HDC WINAPI CreateCompatibleDC_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CreateCompatibleDC_OLD(hdc);
}
HBITMAP WINAPI CreateDIBitmap_Replacement(HDC hdc, CONST BITMAPINFOHEADER* pbmih, DWORD flInit, CONST VOID* pjBits, CONST BITMAPINFO* pbmi, UINT iUsage)
{
	SubstituteDC(hdc);
	return CreateDIBitmap_OLD(hdc, pbmih, flInit, pjBits, pbmi, iUsage);
}
BOOL WINAPI DeleteDC_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return DeleteDC_OLD(hdc);
}
int WINAPI DescribePixelFormat_Replacement(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
	SubstituteDC(hdc);
	return DescribePixelFormat_OLD(hdc, iPixelFormat, nBytes, ppfd);
}
int WINAPI DrawEscape_Replacement(HDC hdc, int iEscape, int cjIn, LPCSTR lpIn)
{
	SubstituteDC(hdc);
	return DrawEscape_OLD(hdc, iEscape, cjIn, lpIn);
}
BOOL WINAPI Ellipse_Replacement(HDC hdc, int left, int top, int right, int bottom)
{
	SubstituteDC(hdc);
	return Ellipse_OLD(hdc, left, top, right, bottom);
}
int WINAPI EnumFontFamiliesExA_Replacement(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	SubstituteDC(hdc);
	return EnumFontFamiliesExA_OLD(hdc, lpLogfont, lpProc, lParam, dwFlags);
}
int WINAPI EnumFontFamiliesExW_Replacement(HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags)
{
	SubstituteDC(hdc);
	return EnumFontFamiliesExW_OLD(hdc, lpLogfont, lpProc, lParam, dwFlags);
}
int WINAPI EnumFontFamiliesA_Replacement(HDC hdc, LPCSTR lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam)
{
	SubstituteDC(hdc);
	return EnumFontFamiliesA_OLD(hdc, lpLogfont, lpProc, lParam);
}
int WINAPI EnumFontFamiliesW_Replacement(HDC hdc, LPCWSTR lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam)
{
	SubstituteDC(hdc);
	return EnumFontFamiliesW_OLD(hdc, lpLogfont, lpProc, lParam);
}
int WINAPI EnumFontsA_Replacement(HDC hdc, LPCSTR lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam)
{
	SubstituteDC(hdc);
	return EnumFontsA_OLD(hdc, lpLogfont, lpProc, lParam);
}
int WINAPI EnumFontsW_Replacement(HDC hdc, LPCWSTR lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam)
{
	SubstituteDC(hdc);
	return EnumFontsW_OLD(hdc, lpLogfont, lpProc, lParam);
}
int WINAPI EnumObjects_Replacement(HDC hdc, int nType, GOBJENUMPROC lpFunc, LPARAM lParam)
{
	SubstituteDC(hdc);
	return EnumObjects_OLD(hdc, nType, lpFunc, lParam);
}
int WINAPI Escape_Replacement(HDC hdc, int iEscape, int cjIn, LPCSTR pvIn, LPVOID pvOut)
{
	SubstituteDC(hdc);
	return Escape_OLD(hdc, iEscape, cjIn, pvIn, pvOut);
}
int WINAPI ExtEscape_Replacement(HDC hdc, int iEscape, int cjInput, LPCSTR lpInData, int cjOutput, LPSTR lpOutData)
{
	SubstituteDC(hdc);
	return ExtEscape_OLD(hdc, iEscape, cjInput, lpInData, cjOutput, lpOutData);
}
int WINAPI ExcludeClipRect_Replacement(HDC hdc, int left, int top, int right, int bottom)
{
	SubstituteDC(hdc);
	return ExcludeClipRect_OLD(hdc, left, top, right, bottom);
}
BOOL WINAPI ExtFloodFill_Replacement(HDC hdc, int x, int y, COLORREF color, UINT type)
{
	SubstituteDC(hdc);
	return ExtFloodFill_OLD(hdc, x, y, color, type);
}
BOOL WINAPI FillRgn_Replacement(HDC hdc, HRGN hrgn, HBRUSH hbr)
{
	SubstituteDC(hdc);
	return FillRgn_OLD(hdc, hrgn, hbr);
}
BOOL WINAPI FloodFill_Replacement(HDC hdc, int x, int y, COLORREF color)
{
	SubstituteDC(hdc);
	return FloodFill_OLD(hdc, x, y, color);
}
int WINAPI GetROP2_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetROP2_OLD(hdc);
}
BOOL WINAPI GetAspectRatioFilterEx_Replacement(HDC hdc, LPSIZE lpsize)
{
	SubstituteDC(hdc);
	return GetAspectRatioFilterEx_OLD(hdc, lpsize);
}
COLORREF WINAPI GetBkColor_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetBkColor_OLD(hdc);
}
COLORREF WINAPI GetDCBrushColor_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetDCBrushColor_OLD(hdc);
}
COLORREF WINAPI GetDCPenColor_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetDCPenColor_OLD(hdc);
}
int WINAPI GetBkMode_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetBkMode_OLD(hdc);
}
UINT WINAPI GetBoundsRect_Replacement(HDC hdc, LPRECT lprect, UINT flags)
{
	SubstituteDC(hdc);
	return GetBoundsRect_OLD(hdc, lprect, flags);
}
BOOL WINAPI GetBrushOrgEx_Replacement(HDC hdc, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return GetBrushOrgEx_OLD(hdc, lppt);
}
BOOL WINAPI GetCharWidthA_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPINT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidthA_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL WINAPI GetCharWidthW_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPINT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidthW_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL WINAPI GetCharWidth32A_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPINT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidth32A_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL WINAPI GetCharWidth32W_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPINT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidth32W_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL APIENTRY GetCharWidthFloatA_Replacement(HDC hdc, UINT iFirst, UINT iLast, PFLOAT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidthFloatA_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL APIENTRY GetCharWidthFloatW_Replacement(HDC hdc, UINT iFirst, UINT iLast, PFLOAT lpBuffer)
{
	SubstituteDC(hdc);
	return GetCharWidthFloatW_OLD(hdc, iFirst, iLast, lpBuffer);
}
BOOL APIENTRY GetCharABCWidthsA_Replacement(HDC hdc, UINT wFirst, UINT wLast, LPABC lpABC)
{
	SubstituteDC(hdc);
	return GetCharABCWidthsA_OLD(hdc, wFirst, wLast, lpABC);
}
BOOL APIENTRY GetCharABCWidthsW_Replacement(HDC hdc, UINT wFirst, UINT wLast, LPABC lpABC)
{
	SubstituteDC(hdc);
	return GetCharABCWidthsW_OLD(hdc, wFirst, wLast, lpABC);
}
BOOL APIENTRY GetCharABCWidthsFloatA_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPABCFLOAT lpABC)
{
	SubstituteDC(hdc);
	return GetCharABCWidthsFloatA_OLD(hdc, iFirst, iLast, lpABC);
}
BOOL APIENTRY GetCharABCWidthsFloatW_Replacement(HDC hdc, UINT iFirst, UINT iLast, LPABCFLOAT lpABC)
{
	SubstituteDC(hdc);
	return GetCharABCWidthsFloatW_OLD(hdc, iFirst, iLast, lpABC);
}
int WINAPI GetClipBox_Replacement(HDC hdc, LPRECT lprect)
{
	SubstituteDC(hdc);
	return GetClipBox_OLD(hdc, lprect);
}
int WINAPI GetClipRgn_Replacement(HDC hdc, HRGN hrgn)
{
	SubstituteDC(hdc);
	return GetClipRgn_OLD(hdc, hrgn);
}
int WINAPI GetMetaRgn_Replacement(HDC hdc, HRGN hrgn)
{
	SubstituteDC(hdc);
	return GetMetaRgn_OLD(hdc, hrgn);
}
HGDIOBJ WINAPI GetCurrentObject_Replacement(HDC hdc, UINT type)
{
	SubstituteDC(hdc);
	return GetCurrentObject_OLD(hdc, type);
}
BOOL WINAPI GetCurrentPositionEx_Replacement(HDC hdc, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return GetCurrentPositionEx_OLD(hdc, lppt);
}
int WINAPI GetDeviceCaps_Replacement(HDC hdc, int index)
{
	SubstituteDC(hdc);
	return GetDeviceCaps_OLD(hdc, index);
}
int WINAPI GetDIBits_Replacement(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage)
{
	SubstituteDC(hdc);
	return GetDIBits_OLD(hdc, hbm, start, cLines, lpvBits, lpbmi, usage);
}
DWORD WINAPI GetFontData_Replacement(HDC hdc, DWORD dwTable, DWORD dwOffset, PVOID pvBuffer, DWORD cjBuffer)
{
	SubstituteDC(hdc);
	return GetFontData_OLD(hdc, dwTable, dwOffset, pvBuffer, cjBuffer);
}
DWORD WINAPI GetGlyphOutlineA_Replacement(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2* lpmat2)
{
	SubstituteDC(hdc);
	return GetGlyphOutlineA_OLD(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}
DWORD WINAPI GetGlyphOutlineW_Replacement(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2* lpmat2)
{
	SubstituteDC(hdc);
	return GetGlyphOutlineW_OLD(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}
int WINAPI GetGraphicsMode_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetGraphicsMode_OLD(hdc);
}
int WINAPI GetMapMode_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetMapMode_OLD(hdc);
}
COLORREF WINAPI GetNearestColor_Replacement(HDC hdc, COLORREF color)
{
	SubstituteDC(hdc);
	return GetNearestColor_OLD(hdc, color);
}
UINT APIENTRY GetOutlineTextMetricsA_Replacement(HDC hdc, UINT cjCopy, LPOUTLINETEXTMETRICA potm)
{
	SubstituteDC(hdc);
	return GetOutlineTextMetricsA_OLD(hdc, cjCopy, potm);
}
UINT APIENTRY GetOutlineTextMetricsW_Replacement(HDC hdc, UINT cjCopy, LPOUTLINETEXTMETRICW potm)
{
	SubstituteDC(hdc);
	return GetOutlineTextMetricsW_OLD(hdc, cjCopy, potm);
}
COLORREF WINAPI GetPixel_Replacement(HDC hdc, int x, int y)
{
	SubstituteDC(hdc);
	return GetPixel_OLD(hdc, x, y);
}
int WINAPI GetPixelFormat_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetPixelFormat_OLD(hdc);
}
int WINAPI GetPolyFillMode_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetPolyFillMode_OLD(hdc);
}
int WINAPI GetRandomRgn_Replacement(HDC hdc, HRGN hrgn, INT i)
{
	SubstituteDC(hdc);
	return GetRandomRgn_OLD(hdc, hrgn, i);
}
int WINAPI GetStretchBltMode_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetStretchBltMode_OLD(hdc);
}
UINT WINAPI GetSystemPaletteEntries_Replacement(HDC hdc, UINT iStart, UINT cEntries, LPPALETTEENTRY pPalEntries)
{
	SubstituteDC(hdc);
	return GetSystemPaletteEntries_OLD(hdc, iStart, cEntries, pPalEntries);
}
UINT WINAPI GetSystemPaletteUse_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetSystemPaletteUse_OLD(hdc);
}
int WINAPI GetTextCharacterExtra_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetTextCharacterExtra_OLD(hdc);
}
UINT WINAPI GetTextAlign_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetTextAlign_OLD(hdc);
}
COLORREF WINAPI GetTextColor_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetTextColor_OLD(hdc);
}
BOOL APIENTRY GetTextExtentPointA_Replacement(HDC hdc, LPCSTR lpString, int c, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return GetTextExtentPointA_OLD(hdc, lpString, c, lpsz);
}
BOOL APIENTRY GetTextExtentPointW_Replacement(HDC hdc, LPCWSTR lpString, int c, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return GetTextExtentPointW_OLD(hdc, lpString, c, lpsz);
}
BOOL APIENTRY GetTextExtentPoint32A_Replacement(HDC hdc, LPCSTR lpString, int c, LPSIZE psizl)
{
	SubstituteDC(hdc);
	return GetTextExtentPoint32A_OLD(hdc, lpString, c, psizl);
}
BOOL APIENTRY GetTextExtentPoint32W_Replacement(HDC hdc, LPCWSTR lpString, int c, LPSIZE psizl)
{
	SubstituteDC(hdc);
	return GetTextExtentPoint32W_OLD(hdc, lpString, c, psizl);
}
BOOL APIENTRY GetTextExtentExPointA_Replacement(HDC hdc, LPCSTR lpszString, int cchString, int nMaxExtent, LPINT lpnFit, LPINT lpnDx, LPSIZE lpSize)
{
	SubstituteDC(hdc);
	return GetTextExtentExPointA_OLD(hdc, lpszString, cchString, nMaxExtent, lpnFit, lpnDx, lpSize);
}
BOOL APIENTRY GetTextExtentExPointW_Replacement(HDC hdc, LPCWSTR lpszString, int cchString, int nMaxExtent, LPINT lpnFit, LPINT lpnDx, LPSIZE lpSize)
{
	SubstituteDC(hdc);
	return GetTextExtentExPointW_OLD(hdc, lpszString, cchString, nMaxExtent, lpnFit, lpnDx, lpSize);
}
int WINAPI GetTextCharset_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetTextCharset_OLD(hdc);
}
int WINAPI GetTextCharsetInfo_Replacement(HDC hdc, LPFONTSIGNATURE lpSig, DWORD dwFlags)
{
	SubstituteDC(hdc);
	return GetTextCharsetInfo_OLD(hdc, lpSig, dwFlags);
}
DWORD WINAPI GetFontLanguageInfo_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetFontLanguageInfo_OLD(hdc);
}
DWORD WINAPI GetCharacterPlacementA_Replacement(HDC hdc, LPCSTR lpString, int nCount, int nMexExtent, LPGCP_RESULTSA lpResults, DWORD dwFlags)
{
	SubstituteDC(hdc);
	return GetCharacterPlacementA_OLD(hdc, lpString, nCount, nMexExtent, lpResults, dwFlags);
}
DWORD WINAPI GetCharacterPlacementW_Replacement(HDC hdc, LPCWSTR lpString, int nCount, int nMexExtent, LPGCP_RESULTSW lpResults, DWORD dwFlags)
{
	SubstituteDC(hdc);
	return GetCharacterPlacementW_OLD(hdc, lpString, nCount, nMexExtent, lpResults, dwFlags);
}
DWORD WINAPI GetFontUnicodeRanges_Replacement(HDC hdc, LPGLYPHSET lpgs)
{
	SubstituteDC(hdc);
	return GetFontUnicodeRanges_OLD(hdc, lpgs);
}
DWORD WINAPI GetGlyphIndicesA_Replacement(HDC hdc, LPCSTR lpstr, int c, LPWORD pgi, DWORD fl)
{
	SubstituteDC(hdc);
	return GetGlyphIndicesA_OLD(hdc, lpstr, c, pgi, fl);
}
DWORD WINAPI GetGlyphIndicesW_Replacement(HDC hdc, LPCWSTR lpstr, int c, LPWORD pgi, DWORD fl)
{
	SubstituteDC(hdc);
	return GetGlyphIndicesW_OLD(hdc, lpstr, c, pgi, fl);
}
BOOL WINAPI GetTextExtentExPointI_Replacement(HDC hdc, LPWORD lpwszString, int cwchString, int nMaxExtent, LPINT lpnFit, LPINT lpnDx, LPSIZE lpSize)
{
	SubstituteDC(hdc);
	return GetTextExtentExPointI_OLD(hdc, lpwszString, cwchString, nMaxExtent, lpnFit, lpnDx, lpSize);
}
BOOL WINAPI GetViewportExtEx_Replacement(HDC hdc, LPSIZE lpsize)
{
	SubstituteDC(hdc);
	return GetViewportExtEx_OLD(hdc, lpsize);
}
BOOL WINAPI GetViewportOrgEx_Replacement(HDC hdc, LPPOINT lppoint)
{
	SubstituteDC(hdc);
	return GetViewportOrgEx_OLD(hdc, lppoint);
}
BOOL WINAPI GetWindowExtEx_Replacement(HDC hdc, LPSIZE lpsize)
{
	SubstituteDC(hdc);
	return GetWindowExtEx_OLD(hdc, lpsize);
}
BOOL WINAPI GetWindowOrgEx_Replacement(HDC hdc, LPPOINT lppoint)
{
	SubstituteDC(hdc);
	return GetWindowOrgEx_OLD(hdc, lppoint);
}
int WINAPI IntersectClipRect_Replacement(HDC hdc, int left, int top, int right, int bottom)
{
	SubstituteDC(hdc);
	return IntersectClipRect_OLD(hdc, left, top, right, bottom);
}
BOOL WINAPI InvertRgn_Replacement(HDC hdc, HRGN hrgn)
{
	SubstituteDC(hdc);
	return InvertRgn_OLD(hdc, hrgn);
}
BOOL WINAPI LineTo_Replacement(HDC hdc, int x, int y)
{
	SubstituteDC(hdc);
	return LineTo_OLD(hdc, x, y);
}
BOOL WINAPI MaskBlt_Replacement(HDC hdcDest, int xDest, int yDest, int width, int height, HDC hdcSrc, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD rop)
{
	SubstituteDC(hdcDest);
	SubstituteDC(hdcSrc);
	return MaskBlt_OLD(hdcDest, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, hbmMask, xMask, yMask, rop);
}
BOOL WINAPI PlgBlt_Replacement(HDC hdcDest, CONST POINT* lpPoint, HDC hdcSrc, int xSrc, int ySrc, int width, int height, HBITMAP hbmMask, int xMask, int yMask)
{
	SubstituteDC(hdcDest);
	SubstituteDC(hdcSrc);
	return PlgBlt_OLD(hdcDest, lpPoint, hdcSrc, xSrc, ySrc, width, height, hbmMask, xMask, yMask);
}
int WINAPI OffsetClipRgn_Replacement(HDC hdc, int x, int y)
{
	SubstituteDC(hdc);
	return OffsetClipRgn_OLD(hdc, x, y);
}
BOOL WINAPI PatBlt_Replacement(HDC hdc, int x, int y, int w, int h, DWORD rop)
{
	SubstituteDC(hdc);
	return PatBlt_OLD(hdc, x, y, w, h, rop);
}
BOOL WINAPI Pie_Replacement(HDC hdc, int left, int top, int right, int bottom, int xr1, int yr1, int xr2, int yr2)
{
	SubstituteDC(hdc);
	return Pie_OLD(hdc, left, top, right, bottom, xr1, yr1, xr2, yr2);
}
BOOL WINAPI PlayMetaFile_Replacement(HDC hdc, HMETAFILE hmf)
{
	SubstituteDC(hdc);
	return PlayMetaFile_OLD(hdc, hmf);
}
BOOL WINAPI PaintRgn_Replacement(HDC hdc, HRGN hrgn)
{
	SubstituteDC(hdc);
	return PaintRgn_OLD(hdc, hrgn);
}
BOOL WINAPI PolyPolygon_Replacement(HDC hdc, CONST POINT* apt, CONST INT* asz, int csz)
{
	SubstituteDC(hdc);
	return PolyPolygon_OLD(hdc, apt, asz, csz);
}
BOOL WINAPI PtVisible_Replacement(HDC hdc, int x, int y)
{
	SubstituteDC(hdc);
	return PtVisible_OLD(hdc, x, y);
}
BOOL WINAPI RectVisible_Replacement(HDC hdc, CONST RECT* lprect)
{
	SubstituteDC(hdc);
	return RectVisible_OLD(hdc, lprect);
}
BOOL WINAPI Rectangle_Replacement(HDC hdc, int left, int top, int right, int bottom)
{
	SubstituteDC(hdc);
	return Rectangle_OLD(hdc, left, top, right, bottom);
}
BOOL WINAPI RestoreDC_Replacement(HDC hdc, int nSavedDC)
{
	SubstituteDC(hdc);
	return RestoreDC_OLD(hdc, nSavedDC);
}
HDC WINAPI ResetDCA_Replacement(HDC hdc, CONST DEVMODEA* lpdm)
{
	SubstituteDC(hdc);
	return ResetDCA_OLD(hdc, lpdm);
}
HDC WINAPI ResetDCW_Replacement(HDC hdc, CONST DEVMODEW* lpdm)
{
	SubstituteDC(hdc);
	return ResetDCW_OLD(hdc, lpdm);
}
UINT WINAPI RealizePalette_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return RealizePalette_OLD(hdc);
}
BOOL WINAPI RoundRect_Replacement(HDC hdc, int left, int top, int right, int bottom, int width, int height)
{
	SubstituteDC(hdc);
	return RoundRect_OLD(hdc, left, top, right, bottom, width, height);
}
int WINAPI SaveDC_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return SaveDC_OLD(hdc);
}
int WINAPI SelectClipRgn_Replacement(HDC hdc, HRGN hrgn)
{
	SubstituteDC(hdc);
	return SelectClipRgn_OLD(hdc, hrgn);
}
int WINAPI ExtSelectClipRgn_Replacement(HDC hdc, HRGN hrgn, int mode)
{
	SubstituteDC(hdc);
	return ExtSelectClipRgn_OLD(hdc, hrgn, mode);
}
int WINAPI SetMetaRgn_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return SetMetaRgn_OLD(hdc);
}
HGDIOBJ WINAPI SelectObject_Replacement(HDC hdc, HGDIOBJ h)
{
	SubstituteDC(hdc);
	return SelectObject_OLD(hdc, h);
}
HPALETTE WINAPI SelectPalette_Replacement(HDC hdc, HPALETTE hPal, BOOL bForceBkgd)
{
	SubstituteDC(hdc);
	return SelectPalette_OLD(hdc, hPal, bForceBkgd);
}
COLORREF WINAPI SetBkColor_Replacement(HDC hdc, COLORREF color)
{
	SubstituteDC(hdc);
	return SetBkColor_OLD(hdc, color);
}
COLORREF WINAPI SetDCBrushColor_Replacement(HDC hdc, COLORREF color)
{
	SubstituteDC(hdc);
	return SetDCBrushColor_OLD(hdc, color);
}
COLORREF WINAPI SetDCPenColor_Replacement(HDC hdc, COLORREF color)
{
	SubstituteDC(hdc);
	return SetDCPenColor_OLD(hdc, color);
}
int WINAPI SetBkMode_Replacement(HDC hdc, int mode)
{
	SubstituteDC(hdc);
	return SetBkMode_OLD(hdc, mode);
}
UINT WINAPI SetBoundsRect_Replacement(HDC hdc, CONST RECT* lprect, UINT flags)
{
	SubstituteDC(hdc);
	return SetBoundsRect_OLD(hdc, lprect, flags);
}
int WINAPI SetDIBits_Replacement(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, CONST VOID* lpBits, CONST BITMAPINFO* lpbmi, UINT ColorUse)
{
	SubstituteDC(hdc);
	return SetDIBits_OLD(hdc, hbm, start, cLines, lpBits, lpbmi, ColorUse);
}
int WINAPI SetDIBitsToDevice_Replacement(HDC hdc, int xDest, int yDest, DWORD w, DWORD h, int xSrc, int ySrc, UINT StartScan, UINT cLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT ColorUse)
{
	SubstituteDC(hdc);
	return SetDIBitsToDevice_OLD(hdc, xDest, yDest, w, h, xSrc, ySrc, StartScan, cLines, lpvBits, lpbmi, ColorUse);
}
DWORD WINAPI SetMapperFlags_Replacement(HDC hdc, DWORD flags)
{
	SubstituteDC(hdc);
	return SetMapperFlags_OLD(hdc, flags);
}
int WINAPI SetGraphicsMode_Replacement(HDC hdc, int iMode)
{
	SubstituteDC(hdc);
	return SetGraphicsMode_OLD(hdc, iMode);
}
int WINAPI SetMapMode_Replacement(HDC hdc, int iMode)
{
	SubstituteDC(hdc);
	return SetMapMode_OLD(hdc, iMode);
}
DWORD WINAPI SetLayout_Replacement(HDC hdc, DWORD l)
{
	SubstituteDC(hdc);
	return SetLayout_OLD(hdc, l);
}
DWORD WINAPI GetLayout_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetLayout_OLD(hdc);
}
COLORREF WINAPI SetPixel_Replacement(HDC hdc, int x, int y, COLORREF color)
{
	SubstituteDC(hdc);
	return SetPixel_OLD(hdc, x, y, color);
}
BOOL WINAPI SetPixelV_Replacement(HDC hdc, int x, int y, COLORREF color)
{
	SubstituteDC(hdc);
	return SetPixelV_OLD(hdc, x, y, color);
}
BOOL WINAPI SetPixelFormat_Replacement(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR* ppfd)
{
	SubstituteDC(hdc);
	return SetPixelFormat_OLD(hdc, format, ppfd);
}
int WINAPI SetPolyFillMode_Replacement(HDC hdc, int mode)
{
	SubstituteDC(hdc);
	return SetPolyFillMode_OLD(hdc, mode);
}
BOOL WINAPI StretchBlt_Replacement(HDC hdcDest, int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop)
{
	SubstituteDC(hdcDest);
	SubstituteDC(hdcSrc);
	return StretchBlt_OLD(hdcDest, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop);
}
int WINAPI StretchDIBits_Replacement(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, CONST VOID* lpBits, CONST BITMAPINFO* lpbmi, UINT iUsage, DWORD rop)
{
	SubstituteDC(hdc);
	return StretchDIBits_OLD(hdc, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop);
}
int WINAPI SetROP2_Replacement(HDC hdc, int rop2)
{
	SubstituteDC(hdc);
	return SetROP2_OLD(hdc, rop2);
}
int WINAPI SetStretchBltMode_Replacement(HDC hdc, int mode)
{
	SubstituteDC(hdc);
	return SetStretchBltMode_OLD(hdc, mode);
}
UINT WINAPI SetSystemPaletteUse_Replacement(HDC hdc, UINT use)
{
	SubstituteDC(hdc);
	return SetSystemPaletteUse_OLD(hdc, use);
}
int WINAPI SetTextCharacterExtra_Replacement(HDC hdc, int extra)
{
	SubstituteDC(hdc);
	return SetTextCharacterExtra_OLD(hdc, extra);
}
COLORREF WINAPI SetTextColor_Replacement(HDC hdc, COLORREF color)
{
	SubstituteDC(hdc);
	return SetTextColor_OLD(hdc, color);
}
UINT WINAPI SetTextAlign_Replacement(HDC hdc, UINT align)
{
	SubstituteDC(hdc);
	return SetTextAlign_OLD(hdc, align);
}
BOOL WINAPI SetTextJustification_Replacement(HDC hdc, int extra, int count)
{
	SubstituteDC(hdc);
	return SetTextJustification_OLD(hdc, extra, count);
}
BOOL WINAPI UpdateColors_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return UpdateColors_OLD(hdc);
}
BOOL WINAPI AlphaBlend_Replacement(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest, int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, BLENDFUNCTION ftn)
{
	SubstituteDC(hdcDest);
	return AlphaBlend_OLD(hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
}
BOOL WINAPI TransparentBlt_Replacement(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest, int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, UINT crTransparent)
{
	SubstituteDC(hdcDest);
	return TransparentBlt_OLD(hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, crTransparent);
}
BOOL WINAPI GradientFill_Replacement(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode)
{
	SubstituteDC(hdc);
	return GradientFill_OLD(hdc, pVertex, nVertex, pMesh, nMesh, ulMode);
}
BOOL WINAPI PlayMetaFileRecord_Replacement(HDC hdc, LPHANDLETABLE lpHandleTable, LPMETARECORD lpMR, UINT noObjs)
{
	SubstituteDC(hdc);
	return PlayMetaFileRecord_OLD(hdc, lpHandleTable, lpMR, noObjs);
}
BOOL WINAPI EnumMetaFile_Replacement(HDC hdc, HMETAFILE hmf, MFENUMPROC proc, LPARAM param)
{
	SubstituteDC(hdc);
	return EnumMetaFile_OLD(hdc, hmf, proc, param);
}
HENHMETAFILE WINAPI CloseEnhMetaFile_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CloseEnhMetaFile_OLD(hdc);
}
HDC WINAPI CreateEnhMetaFileA_Replacement(HDC hdc, LPCSTR lpFilename, CONST RECT* lprc, LPCSTR lpDesc)
{
	SubstituteDC(hdc);
	return CreateEnhMetaFileA_OLD(hdc, lpFilename, lprc, lpDesc);
}
HDC WINAPI CreateEnhMetaFileW_Replacement(HDC hdc, LPCWSTR lpFilename, CONST RECT* lprc, LPCWSTR lpDesc)
{
	SubstituteDC(hdc);
	return CreateEnhMetaFileW_OLD(hdc, lpFilename, lprc, lpDesc);
}
BOOL WINAPI EnumEnhMetaFile_Replacement(HDC hdc, HENHMETAFILE hmf, ENHMFENUMPROC proc, LPVOID param, CONST RECT* lpRect)
{
	SubstituteDC(hdc);
	return EnumEnhMetaFile_OLD(hdc, hmf, proc, param, lpRect);
}
UINT WINAPI GetWinMetaFileBits_Replacement(HENHMETAFILE hemf, UINT cbData16, LPBYTE pData16, INT iMapMode, HDC hdcRef)
{
	SubstituteDC(hdcRef);
	return GetWinMetaFileBits_OLD(hemf, cbData16, pData16, iMapMode, hdcRef);
}
BOOL WINAPI PlayEnhMetaFile_Replacement(HDC hdc, HENHMETAFILE hmf, CONST RECT* lprect)
{
	SubstituteDC(hdc);
	return PlayEnhMetaFile_OLD(hdc, hmf, lprect);
}
BOOL WINAPI PlayEnhMetaFileRecord_Replacement(HDC hdc, LPHANDLETABLE pht, CONST ENHMETARECORD* pmr, UINT cht)
{
	SubstituteDC(hdc);
	return PlayEnhMetaFileRecord_OLD(hdc, pht, pmr, cht);
}
HENHMETAFILE WINAPI SetWinMetaFileBits_Replacement(UINT nSize, CONST BYTE* lpMeta16Data, HDC hdcRef, CONST METAFILEPICT* lpMFP)
{
	SubstituteDC(hdcRef);
	return SetWinMetaFileBits_OLD(nSize, lpMeta16Data, hdcRef, lpMFP);
}
BOOL WINAPI GdiComment_Replacement(HDC hdc, UINT nSize, CONST BYTE* lpData)
{
	SubstituteDC(hdc);
	return GdiComment_OLD(hdc, nSize, lpData);
}
BOOL WINAPI GetTextMetricsA_Replacement(HDC hdc, LPTEXTMETRICA lptm)
{
	SubstituteDC(hdc);
	return GetTextMetricsA_OLD(hdc, lptm);
}
BOOL WINAPI GetTextMetricsW_Replacement(HDC hdc, LPTEXTMETRICW lptm)
{
	SubstituteDC(hdc);
	return GetTextMetricsW_OLD(hdc, lptm);
}
BOOL WINAPI AngleArc_Replacement(HDC hdc, int x, int y, DWORD r, FLOAT StartAngle, FLOAT SweepAngle)
{
	SubstituteDC(hdc);
	return AngleArc_OLD(hdc, x, y, r, StartAngle, SweepAngle);
}
BOOL WINAPI PolyPolyline_Replacement(HDC hdc, CONST POINT* apt, CONST DWORD* asz, DWORD csz)
{
	SubstituteDC(hdc);
	return PolyPolyline_OLD(hdc, apt, asz, csz);
}
BOOL WINAPI GetWorldTransform_Replacement(HDC hdc, LPXFORM lpxf)
{
	SubstituteDC(hdc);
	return GetWorldTransform_OLD(hdc, lpxf);
}
BOOL WINAPI SetWorldTransform_Replacement(HDC hdc, CONST XFORM* lpxf)
{
	SubstituteDC(hdc);
	return SetWorldTransform_OLD(hdc, lpxf);
}
BOOL WINAPI ModifyWorldTransform_Replacement(HDC hdc, CONST XFORM* lpxf, DWORD mode)
{
	SubstituteDC(hdc);
	return ModifyWorldTransform_OLD(hdc, lpxf, mode);
}
HBITMAP WINAPI CreateDIBSection_Replacement(HDC hdc, CONST BITMAPINFO* pbmi, UINT usage, VOID** ppvBits, HANDLE hSection, DWORD offset)
{
	SubstituteDC(hdc);
	return CreateDIBSection_OLD(hdc, pbmi, usage, ppvBits, hSection, offset);
}
UINT WINAPI GetDIBColorTable_Replacement(HDC hdc, UINT iStart, UINT cEntries, RGBQUAD* prgbq)
{
	SubstituteDC(hdc);
	return GetDIBColorTable_OLD(hdc, iStart, cEntries, prgbq);
}
UINT WINAPI SetDIBColorTable_Replacement(HDC hdc, UINT iStart, UINT cEntries, CONST RGBQUAD* prgbq)
{
	SubstituteDC(hdc);
	return SetDIBColorTable_OLD(hdc, iStart, cEntries, prgbq);
}
BOOL WINAPI SetColorAdjustment_Replacement(HDC hdc, CONST COLORADJUSTMENT* lpca)
{
	SubstituteDC(hdc);
	return SetColorAdjustment_OLD(hdc, lpca);
}
BOOL WINAPI GetColorAdjustment_Replacement(HDC hdc, LPCOLORADJUSTMENT lpca)
{
	SubstituteDC(hdc);
	return GetColorAdjustment_OLD(hdc, lpca);
}
HPALETTE WINAPI CreateHalftonePalette_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CreateHalftonePalette_OLD(hdc);
}
int WINAPI StartDocA_Replacement(HDC hdc, CONST DOCINFOA* lpdi)
{
	SubstituteDC(hdc);
	return StartDocA_OLD(hdc, lpdi);
}
int WINAPI StartDocW_Replacement(HDC hdc, CONST DOCINFOW* lpdi)
{
	SubstituteDC(hdc);
	return StartDocW_OLD(hdc, lpdi);
}
int WINAPI EndDoc_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return EndDoc_OLD(hdc);
}
int WINAPI StartPage_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return StartPage_OLD(hdc);
}
int WINAPI EndPage_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return EndPage_OLD(hdc);
}
int WINAPI AbortDoc_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return AbortDoc_OLD(hdc);
}
int WINAPI SetAbortProc_Replacement(HDC hdc, ABORTPROC proc)
{
	SubstituteDC(hdc);
	return SetAbortProc_OLD(hdc, proc);
}
BOOL WINAPI AbortPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return AbortPath_OLD(hdc);
}
BOOL WINAPI ArcTo_Replacement(HDC hdc, int left, int top, int right, int bottom, int xr1, int yr1, int xr2, int yr2)
{
	SubstituteDC(hdc);
	return ArcTo_OLD(hdc, left, top, right, bottom, xr1, yr1, xr2, yr2);
}
BOOL WINAPI BeginPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return BeginPath_OLD(hdc);
}
BOOL WINAPI CloseFigure_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return CloseFigure_OLD(hdc);
}
BOOL WINAPI EndPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return EndPath_OLD(hdc);
}
BOOL WINAPI FillPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return FillPath_OLD(hdc);
}
BOOL WINAPI FlattenPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return FlattenPath_OLD(hdc);
}
int WINAPI GetPath_Replacement(HDC hdc, LPPOINT apt, LPBYTE aj, int cpt)
{
	SubstituteDC(hdc);
	return GetPath_OLD(hdc, apt, aj, cpt);
}
HRGN WINAPI PathToRegion_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return PathToRegion_OLD(hdc);
}
BOOL WINAPI PolyDraw_Replacement(HDC hdc, CONST POINT* apt, CONST BYTE* aj, int cpt)
{
	SubstituteDC(hdc);
	return PolyDraw_OLD(hdc, apt, aj, cpt);
}
BOOL WINAPI SelectClipPath_Replacement(HDC hdc, int mode)
{
	SubstituteDC(hdc);
	return SelectClipPath_OLD(hdc, mode);
}
int WINAPI SetArcDirection_Replacement(HDC hdc, int dir)
{
	SubstituteDC(hdc);
	return SetArcDirection_OLD(hdc, dir);
}
BOOL WINAPI SetMiterLimit_Replacement(HDC hdc, FLOAT limit, PFLOAT old)
{
	SubstituteDC(hdc);
	return SetMiterLimit_OLD(hdc, limit, old);
}
BOOL WINAPI StrokeAndFillPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return StrokeAndFillPath_OLD(hdc);
}
BOOL WINAPI StrokePath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return StrokePath_OLD(hdc);
}
BOOL WINAPI WidenPath_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return WidenPath_OLD(hdc);
}
BOOL WINAPI GetMiterLimit_Replacement(HDC hdc, PFLOAT plimit)
{
	SubstituteDC(hdc);
	return GetMiterLimit_OLD(hdc, plimit);
}
int WINAPI GetArcDirection_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetArcDirection_OLD(hdc);
}
BOOL WINAPI MoveToEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return MoveToEx_OLD(hdc, x, y, lppt);
}
BOOL WINAPI TextOutA_Replacement(HDC hdc, int x, int y, LPCSTR lpString, int c)
{
	SubstituteDC(hdc);
	return TextOutA_OLD(hdc, x, y, lpString, c);
}
BOOL WINAPI TextOutW_Replacement(HDC hdc, int x, int y, LPCWSTR lpString, int c)
{
	SubstituteDC(hdc);
	return TextOutW_OLD(hdc, x, y, lpString, c);
}
BOOL WINAPI ExtTextOutA_Replacement(HDC hdc, int x, int y, UINT options, CONST RECT* lprect, LPCSTR lpString, UINT c, CONST INT* lpDx)
{
	SubstituteDC(hdc);
	return ExtTextOutA_OLD(hdc, x, y, options, lprect, lpString, c, lpDx);
}
BOOL WINAPI ExtTextOutW_Replacement(HDC hdc, int x, int y, UINT options, CONST RECT* lprect, LPCWSTR lpString, UINT c, CONST INT* lpDx)
{
	SubstituteDC(hdc);
	return ExtTextOutW_OLD(hdc, x, y, options, lprect, lpString, c, lpDx);
}
BOOL WINAPI PolyTextOutA_Replacement(HDC hdc, CONST POLYTEXTA* ppt, int nstrings)
{
	SubstituteDC(hdc);
	return PolyTextOutA_OLD(hdc, ppt, nstrings);
}
BOOL WINAPI PolyTextOutW_Replacement(HDC hdc, CONST POLYTEXTW* ppt, int nstrings)
{
	SubstituteDC(hdc);
	return PolyTextOutW_OLD(hdc, ppt, nstrings);
}
BOOL WINAPI DPtoLP_Replacement(HDC hdc, LPPOINT lppt, int c)
{
	SubstituteDC(hdc);
	return DPtoLP_OLD(hdc, lppt, c);
}
BOOL WINAPI LPtoDP_Replacement(HDC hdc, LPPOINT lppt, int c)
{
	SubstituteDC(hdc);
	return LPtoDP_OLD(hdc, lppt, c);
}
BOOL WINAPI Polygon_Replacement(HDC hdc, CONST POINT* apt, int cpt)
{
	SubstituteDC(hdc);
	return Polygon_OLD(hdc, apt, cpt);
}
BOOL WINAPI Polyline_Replacement(HDC hdc, CONST POINT* apt, int cpt)
{
	SubstituteDC(hdc);
	return Polyline_OLD(hdc, apt, cpt);
}
BOOL WINAPI PolyBezier_Replacement(HDC hdc, CONST POINT* apt, DWORD cpt)
{
	SubstituteDC(hdc);
	return PolyBezier_OLD(hdc, apt, cpt);
}
BOOL WINAPI PolyBezierTo_Replacement(HDC hdc, CONST POINT* apt, DWORD cpt)
{
	SubstituteDC(hdc);
	return PolyBezierTo_OLD(hdc, apt, cpt);
}
BOOL WINAPI PolylineTo_Replacement(HDC hdc, CONST POINT* apt, DWORD cpt)
{
	SubstituteDC(hdc);
	return PolylineTo_OLD(hdc, apt, cpt);
}
BOOL WINAPI SetViewportExtEx_Replacement(HDC hdc, int x, int y, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return SetViewportExtEx_OLD(hdc, x, y, lpsz);
}
BOOL WINAPI SetViewportOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return SetViewportOrgEx_OLD(hdc, x, y, lppt);
}
BOOL WINAPI SetWindowExtEx_Replacement(HDC hdc, int x, int y, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return SetWindowExtEx_OLD(hdc, x, y, lpsz);
}
BOOL WINAPI SetWindowOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return SetWindowOrgEx_OLD(hdc, x, y, lppt);
}
BOOL WINAPI OffsetViewportOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return OffsetViewportOrgEx_OLD(hdc, x, y, lppt);
}
BOOL WINAPI OffsetWindowOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return OffsetWindowOrgEx_OLD(hdc, x, y, lppt);
}
BOOL WINAPI ScaleViewportExtEx_Replacement(HDC hdc, int xn, int dx, int yn, int yd, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return ScaleViewportExtEx_OLD(hdc, xn, dx, yn, yd, lpsz);
}
BOOL WINAPI ScaleWindowExtEx_Replacement(HDC hdc, int xn, int xd, int yn, int yd, LPSIZE lpsz)
{
	SubstituteDC(hdc);
	return ScaleWindowExtEx_OLD(hdc, xn, xd, yn, yd, lpsz);
}
BOOL WINAPI SetBrushOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return SetBrushOrgEx_OLD(hdc, x, y, lppt);
}
int WINAPI GetTextFaceA_Replacement(HDC hdc, int c, LPSTR lpName)
{
	SubstituteDC(hdc);
	return GetTextFaceA_OLD(hdc, c, lpName);
}
int WINAPI GetTextFaceW_Replacement(HDC hdc, int c, LPWSTR lpName)
{
	SubstituteDC(hdc);
	return GetTextFaceW_OLD(hdc, c, lpName);
}
DWORD WINAPI GetKerningPairsA_Replacement(HDC hdc, DWORD nPairs, LPKERNINGPAIR lpKernPair)
{
	SubstituteDC(hdc);
	return GetKerningPairsA_OLD(hdc, nPairs, lpKernPair);
}
DWORD WINAPI GetKerningPairsW_Replacement(HDC hdc, DWORD nPairs, LPKERNINGPAIR lpKernPair)
{
	SubstituteDC(hdc);
	return GetKerningPairsW_OLD(hdc, nPairs, lpKernPair);
}
BOOL WINAPI GetDCOrgEx_Replacement(HDC hdc, LPPOINT lppt)
{
	SubstituteDC(hdc);
	return GetDCOrgEx_OLD(hdc, lppt);
}
BOOL WINAPI FixBrushOrgEx_Replacement(HDC hdc, int x, int y, LPPOINT ptl)
{
	SubstituteDC(hdc);
	return FixBrushOrgEx_OLD(hdc, x, y, ptl);
}
int WINAPI SetICMMode_Replacement(HDC hdc, int mode)
{
	SubstituteDC(hdc);
	return SetICMMode_OLD(hdc, mode);
}
BOOL WINAPI CheckColorsInGamut_Replacement(HDC hdc, LPRGBTRIPLE lpRGBTriple, LPVOID dlpBuffer, DWORD nCount)
{
	SubstituteDC(hdc);
	return CheckColorsInGamut_OLD(hdc, lpRGBTriple, dlpBuffer, nCount);
}
HCOLORSPACE WINAPI GetColorSpace_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return GetColorSpace_OLD(hdc);
}
HCOLORSPACE WINAPI SetColorSpace_Replacement(HDC hdc, HCOLORSPACE hcs)
{
	SubstituteDC(hdc);
	return SetColorSpace_OLD(hdc, hcs);
}
BOOL WINAPI GetICMProfileA_Replacement(HDC hdc, LPDWORD pBufSize, LPSTR pszFilename)
{
	SubstituteDC(hdc);
	return GetICMProfileA_OLD(hdc, pBufSize, pszFilename);
}
BOOL WINAPI GetICMProfileW_Replacement(HDC hdc, LPDWORD pBufSize, LPWSTR pszFilename)
{
	SubstituteDC(hdc);
	return GetICMProfileW_OLD(hdc, pBufSize, pszFilename);
}
BOOL WINAPI SetICMProfileA_Replacement(HDC hdc, LPSTR lpFileName)
{
	SubstituteDC(hdc);
	return SetICMProfileA_OLD(hdc, lpFileName);
}
BOOL WINAPI SetICMProfileW_Replacement(HDC hdc, LPWSTR lpFileName)
{
	SubstituteDC(hdc);
	return SetICMProfileW_OLD(hdc, lpFileName);
}
BOOL WINAPI GetDeviceGammaRamp_Replacement(HDC hdc, LPVOID lpRamp)
{
	SubstituteDC(hdc);
	return GetDeviceGammaRamp_OLD(hdc, lpRamp);
}
BOOL WINAPI SetDeviceGammaRamp_Replacement(HDC hdc, LPVOID lpRamp)
{
	SubstituteDC(hdc);
	return SetDeviceGammaRamp_OLD(hdc, lpRamp);
}
BOOL WINAPI ColorMatchToTarget_Replacement(HDC hdc, HDC hdcTarget, DWORD action)
{
	SubstituteDC(hdc);
	return ColorMatchToTarget_OLD(hdc, hdcTarget, action);
}
int WINAPI EnumICMProfilesA_Replacement(HDC hdc, ICMENUMPROCA proc, LPARAM param)
{
	SubstituteDC(hdc);
	return EnumICMProfilesA_OLD(hdc, proc, param);
}
int WINAPI EnumICMProfilesW_Replacement(HDC hdc, ICMENUMPROCW proc, LPARAM param)
{
	SubstituteDC(hdc);
	return EnumICMProfilesW_OLD(hdc, proc, param);
}
BOOL WINAPI ColorCorrectPalette_Replacement(HDC hdc, HPALETTE hPal, DWORD deFirst, DWORD num)
{
	SubstituteDC(hdc);
	return ColorCorrectPalette_OLD(hdc, hPal, deFirst, num);
}
HGLRC WINAPI wglCreateContext_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return wglCreateContext_OLD(hdc);
}
HGLRC WINAPI wglCreateLayerContext_Replacement(HDC hdc, int _arg2)
{
	SubstituteDC(hdc);
	return wglCreateLayerContext_OLD(hdc, _arg2);
}
BOOL WINAPI wglMakeCurrent_Replacement(HDC hdc, HGLRC _arg2)
{
	SubstituteDC(hdc);
	return wglMakeCurrent_OLD(hdc, _arg2);
}
BOOL WINAPI wglUseFontBitmapsA_Replacement(HDC hdc, DWORD _arg2, DWORD _arg3, DWORD _arg4)
{
	SubstituteDC(hdc);
	return wglUseFontBitmapsA_OLD(hdc, _arg2, _arg3, _arg4);
}
BOOL WINAPI wglUseFontBitmapsW_Replacement(HDC hdc, DWORD _arg2, DWORD _arg3, DWORD _arg4)
{
	SubstituteDC(hdc);
	return wglUseFontBitmapsW_OLD(hdc, _arg2, _arg3, _arg4);
}
BOOL WINAPI SwapBuffers_Replacement(HDC hdc)
{
	SubstituteDC(hdc);
	return SwapBuffers_OLD(hdc);
}
BOOL WINAPI wglUseFontOutlinesA_Replacement(HDC hdc, DWORD _arg2, DWORD _arg3, DWORD _arg4, FLOAT _arg5, FLOAT _arg6, int _arg7, LPGLYPHMETRICSFLOAT _arg8)
{
	SubstituteDC(hdc);
	return wglUseFontOutlinesA_OLD(hdc, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8);
}
BOOL WINAPI wglUseFontOutlinesW_Replacement(HDC hdc, DWORD _arg2, DWORD _arg3, DWORD _arg4, FLOAT _arg5, FLOAT _arg6, int _arg7, LPGLYPHMETRICSFLOAT _arg8)
{
	SubstituteDC(hdc);
	return wglUseFontOutlinesW_OLD(hdc, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8);
}
BOOL WINAPI wglDescribeLayerPlane_Replacement(HDC hdc, int _arg2, int _arg3, UINT _arg4, LPLAYERPLANEDESCRIPTOR _arg5)
{
	SubstituteDC(hdc);
	return wglDescribeLayerPlane_OLD(hdc, _arg2, _arg3, _arg4, _arg5);
}
int WINAPI wglSetLayerPaletteEntries_Replacement(HDC hdc, int _arg2, int _arg3, int _arg4, CONST COLORREF* _arg5)
{
	SubstituteDC(hdc);
	return wglSetLayerPaletteEntries_OLD(hdc, _arg2, _arg3, _arg4, _arg5);
}
int WINAPI wglGetLayerPaletteEntries_Replacement(HDC hdc, int _arg2, int _arg3, int _arg4, COLORREF* _arg5)
{
	SubstituteDC(hdc);
	return wglGetLayerPaletteEntries_OLD(hdc, _arg2, _arg3, _arg4, _arg5);
}
BOOL WINAPI wglRealizeLayerPalette_Replacement(HDC hdc, int _arg2, BOOL _arg3)
{
	SubstituteDC(hdc);
	return wglRealizeLayerPalette_OLD(hdc, _arg2, _arg3);
}
BOOL WINAPI wglSwapLayerBuffers_Replacement(HDC hdc, UINT _arg2)
{
	SubstituteDC(hdc);
	return wglSwapLayerBuffers_OLD(hdc, _arg2);
}
