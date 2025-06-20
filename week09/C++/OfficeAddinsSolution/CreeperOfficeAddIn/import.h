#pragma once

#import "libid:00020905-0000-0000-C000-000000000046" \
    rename("RGB", "WordRGB") \
    rename("DocumentProperties", "WordDocumentProperties") \
    rename("ExitWindows", "WordExitWindows") \
    rename("FindText", "WordFindText") \
    rename("CopyFile", "WordCopyFile") \
    rename("ReplaceText", "WordReplaceText") \
    rename("SearchPath", "WordSearchPath") \
    rename("FontNames", "WordFontNames") \
    rename("Rectangle", "WordRectangle") \
    auto_search \
    rename_namespace("Word")

#import "libid:91493440-5A91-11CF-8700-00AA0060263B" \
    auto_search \
    rename_namespace("PPT") \
    rename("RGB", "PPT_RGB") \
    rename("DocumentProperties", "PPT_DocumentProperties") \
    rename("PageSetup", "PPT_PageSetup") \
    rename("Shape", "PPT_Shape") \
    rename("TextRange", "PPT_TextRange")

#import "libid:00020813-0000-0000-C000-000000000046" \
    rename("RGB", "ExcelRGB") \
    rename("DocumentProperties", "ExcelDocumentProperties") \
    rename("ExitWindows", "ExcelExitWindows") \
    rename("FindText", "ExcelFindText") \
    rename("CopyFile", "ExcelCopyFile") \
    rename("ReplaceText", "ExcelReplaceText") \
    rename("SearchPath", "ExcelSearchPath") \
    rename("FontNames", "ExcelFontNames") \
    rename("Rectangle", "ExcelRectangle") \
    rename("DialogBox", "ExcelDialogBox") \
    rename("Window", "ExcelWindow") \
    auto_search \
    rename_namespace("Excel") \


// Microsoft Add-in Designer (MSADDNDR.DLL)
#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" \
    named_guids, auto_search \
    rename_namespace("AddInDesignerObjects") \
