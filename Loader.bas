Attribute VB_Name = "Loader"
Option Explicit

Dim varnum As Integer
Dim cyclenum As Integer
Dim maxrow As Integer
    
Sub get_settings()
    varnum = ActiveSheet.range("varnum")
    cyclenum = ActiveSheet.range("cyclenum")
    maxrow = ActiveSheet.range("maxrow")
End Sub

Sub init_sheet()
    Dim i As Integer
    Dim origin As range
    Dim target As range
        
    Call get_settings
    
    Set origin = ActiveSheet.range("origin")
    
    Set target = ActiveSheet.range(origin.Offset(1, 0), origin.Offset(maxrow - 2, cyclenum - 1))
      
    target.ClearContents
    target = 0
    
    target.FormatConditions.Delete

    target.FormatConditions.Add Type:=xlExpression, Formula1:="=C3=0"
    target.FormatConditions(target.FormatConditions.Count).SetFirstPriority
    With target.FormatConditions(1).Interior
        .PatternColorIndex = xlAutomatic
        .ThemeColor = xlThemeColorDark2
        .TintAndShade = 0
    End With
    
    Dim sh As Worksheet
    Set sh = ActiveSheet
    
    sh.range(sh.range("A1").Offset(2, 0), sh.range("A1").Offset(varnum, 1)).ClearContents
    
End Sub

Sub save_csv()
    Dim objFso As Object
    Dim TextFile As Object
    Set objFso = CreateObject("Scripting.FileSystemObject")
 
    Dim FileName As Variant
    FileName = Application.GetOpenFilename(FileFilter:="CSVファイル,*.csv")
    
    If FileName = False Then
        Exit Sub
    End If
    
    If Not objFso.FileExists(FileName) Then
        MsgBox "ファイルが見つかりません"
        Exit Sub
    End If
    
    Set TextFile = objFso.OpenTextFile(FileName, 1)
    Dim str As String
    Dim ary As Variant
    Dim i As Integer, j As Integer, k As Integer
    Dim sh As Worksheet
    Dim interval As Integer
    
    
    Set sh = ActiveSheet
    
    ' 1行目を読み込む
    str = TextFile.readline
    ary = Split(str, ",")
    For i = 0 To UBound(ary)
        sh.range("A1").Offset(i + 1, 0) = ary(i)
    Next
    
    ' 2行目以降を読み込む
    j = 0
    k = 0
    interval = Int(sh.range("cycle_time") / sh.range("B2"))
    
    Do Until TextFile.AtEndOfStream
        
        str = TextFile.readline
        
        If k = 0 Then
            ary = Split(str, ",")
            For i = 1 To UBound(ary) ' 1列目は時間なので無視
                sh.range("origin").Offset(i, j) = ary(i)
            Next
            
            j = j + 1
        End If
        
        k = k + 1
        If k >= interval Then
            k = 0
        End If
        
    Loop
    
    TextFile.Close
 
    Set objFso = Nothing
    
    
    Call get_settings
    
    For i = 1 To varnum - 1
        Call set_coloring_rule(i + 2)
    Next
    
End Sub

Sub set_coloring_rule(ByVal row As Integer)
    Dim target As range
    Dim sh As Worksheet
    Dim col As Integer
    
    Call get_settings
    Set sh = ActiveSheet
    
    col = ActiveSheet.range("origin").Column
    
    Set target = sh.range(sh.Cells(row, col), sh.Cells(row, col + cyclenum - 1))
    
         
    target.FormatConditions.Delete
    
    Dim str As String
    str = "=" & Cells(row, col).Address(RowAbsolute:=False, ColumnAbsolute:=False) & "=0"
    
    target.FormatConditions.Add Type:=xlExpression, Formula1:=str
    target.FormatConditions(target.FormatConditions.Count).SetFirstPriority
    With target.FormatConditions(1).Interior
        .PatternColorIndex = 0
        .ThemeColor = xlThemeColorDark2
        .TintAndShade = 0
        .PatternTintAndShade = 0
    End With
    
    target.FormatConditions.AddColorScale ColorScaleType:=3
    target.FormatConditions(target.FormatConditions.Count).SetFirstPriority
    target.FormatConditions(1).ColorScaleCriteria(1).Type = _
        xlConditionValueLowestValue
    With target.FormatConditions(1).ColorScaleCriteria(1).FormatColor
        .ThemeColor = xlThemeColorDark2
        .TintAndShade = 0
    End With
    target.FormatConditions(1).ColorScaleCriteria(2).Type = _
        xlConditionValuePercent
    target.FormatConditions(1).ColorScaleCriteria(2).value = 50
    With target.FormatConditions(1).ColorScaleCriteria(2).FormatColor
        .ThemeColor = xlThemeColorAccent1
        .TintAndShade = 0
    End With
    target.FormatConditions(1).ColorScaleCriteria(3).Type = _
        xlConditionValueHighestValue
    With target.FormatConditions(1).ColorScaleCriteria(3).FormatColor
        .Color = 8109667
        .TintAndShade = 0
    End With
    
    target.FormatConditions.AddColorScale ColorScaleType:=3
    target.FormatConditions(target.FormatConditions.Count).SetFirstPriority
    target.FormatConditions(1).ColorScaleCriteria(1).Type = _
        xlConditionValueLowestValue
    With target.FormatConditions(1).ColorScaleCriteria(1).FormatColor
        .ThemeColor = xlThemeColorDark2
        .TintAndShade = 0
    End With
    target.FormatConditions(1).ColorScaleCriteria(2).Type = _
        xlConditionValuePercent
    target.FormatConditions(1).ColorScaleCriteria(2).value = 50
    With target.FormatConditions(1).ColorScaleCriteria(2).FormatColor
        .ThemeColor = xlThemeColorAccent1
        .TintAndShade = 0
    End With
    target.FormatConditions(1).ColorScaleCriteria(3).Type = _
        xlConditionValueHighestValue
    With target.FormatConditions(1).ColorScaleCriteria(3).FormatColor
        .Color = 8109667
        .TintAndShade = 0
    End With
    

End Sub

