ribbon ui所需的xml文件在c++文件夹下实现了，这里就不重复写了

Sub CountWords()
    Dim totalChars As Long
    kim chineseChars As Long
    Dim englishChars As Long
    Dim i As Long
    Dim ch As String
    
    totalChars = ActiveDocument.Characters.Count
    For i = 1 To totalChars
        ch = AscW(Mid(ActiveDocument.Content.Text, i, 1))
        If (ch >= 65 And ch <= 90) Or (ch >= 97 And ch <= 122) Then
            englishChars = englishChars + 1
        ElseIf (ch >= 19968 And ch <= 40959) Then ' Unicode for Chinese characters
            chineseChars = chineseChars + 1
        End If
    Next i
    MsgBox "总字数：" & totalChars & vbCrLf & _
           "中文字符数：" & chineseChars & vbCrLf & _
           "英文字符数：" & englishChars
End Sub


Sub ApplyFormat(control As IRibbonControl)
    With Selection.Paragraphs(1)
        .LeftIndent = CentimetersToPoints(0.74) ' 首行缩进 2 字符 ≈ 0.74cm
        .LineSpacingRule = wdLineSpace1pt5 ' 行距 1.5 倍
    End With
End Sub

Sub CountNonEmptyCells()
    Dim ws As Worksheet
    Set ws = ActiveSheet
    Dim count As Long
    count = Application.WorksheetFunction.CountA(ws.UsedRange)
    MsgBox "当前工作表非空单元格数为：" & count
End Sub

Sub GenerateChart(control As IRibbonControl)
    Dim chartObj As ChartObject
    Set chartObj = ActiveSheet.ChartObjects.Add(Left:=300, Width:=375, Top:=50, Height:=250)
    With chartObj.Chart
        .SetSourceData Source:=Selection
        .ChartType = xlColumnClustered
    End With
End Sub

Sub CountTextBoxes()
    Dim slide As slide
    Dim shape As Shape
    Dim count As Integer
    For Each slide In ActivePresentation.Slides
        For Each shape In slide.Shapes
            If shape.HasTextFrame Then
                If shape.TextFrame.TextRange <> "" Then
                    count = count + 1
                End If
            End If
        Next shape
    Next slide
    MsgBox "非空文本框总数：" & count
End Sub

Sub AddFormattedSlide(control As IRibbonControl)
    Dim newSlide As slide
    Set newSlide = ActivePresentation.Slides.Add(Index:=ActivePresentation.Slides.Count + 1, Layout:=ppLayoutBlank)
    With newSlide.Background
        .Fill.Visible = msoTrue
        .Fill.Solid
        .Fill.ForeColor.ObjectThemeColor = msoThemeColorAccent1 ' 主题色着色
        .Fill.Transparency = 0
    End With
    newSlide.Select
End Sub