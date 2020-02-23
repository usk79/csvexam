Attribute VB_Name = "Generator"
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
    
End Sub

Sub save_csv()
    Dim objFso As Object
    Dim TextFile As Object
    Set objFso = CreateObject("Scripting.FileSystemObject")
 
    Dim FileName As Variant
    FileName = Application.GetSaveAsFilename(InitialFileName:=ActiveSheet.range("DefaultFileName"), FileFilter:="CSVファイル,*.csv")
    
    If FileName = False Then
        Exit Sub
    End If
    
 
    If Not objFso.FileExists(FileName) Then
        objFso.CreateTextFile (FileName)
    End If
    
    Set TextFile = objFso.OpenTextFile(FileName, 2)
    
    
    ' 1行目
    Call get_settings
    Dim sh As Worksheet
    Dim i As Integer, j As Integer, k As Integer
    Dim expNum As Integer
    Dim tmpstr As String
    Dim tmpstr2 As String ' 時刻以外
    Dim value As Double, lsb As Double
    Dim cycle As Integer
    tmpstr = ""
    
    Set sh = ActiveSheet
    
    For i = 1 To varnum
        tmpstr = tmpstr & sh.range("A1").Offset(i, 0)
        
        If i < varnum Then
            tmpstr = tmpstr & ","
        End If
    Next
    TextFile.WriteLine tmpstr
    
    ' 2行目以降
    For i = 0 To cyclenum - 1
        tmpstr2 = ""
        
        ' 時刻以外の変数の文字列を作成
        For j = 1 To varnum - 1
            value = sh.range("origin").Offset(j, i)
            lsb = sh.range("origin").Offset(j, -1)
            
            tmpstr2 = tmpstr2 & CStr(value)
            
            If j < varnum - 1 Then
                tmpstr2 = tmpstr2 & ","
            End If

        Next
        
            
        ' 時刻を拡張する
        expNum = sh.range("cycle_time") / sh.range("B2")
        cycle = sh.range("origin").Offset(0, i)
        lsb = sh.range("B2")
        
        For k = 0 To expNum - 1
            tmpstr = CStr(cycle * sh.range("cycle_time") + lsb * k)
            
            TextFile.WriteLine tmpstr & "," & tmpstr2
        Next
        
    Next

    
    TextFile.Close
 
    Set objFso = Nothing
    
End Sub

Sub clear_line()
Attribute clear_line.VB_ProcData.VB_Invoke_Func = "A\n14"
    Call get_settings
    Dim sh As Worksheet
    Dim target As range
    Set sh = ActiveSheet
    Dim row As Integer, col As Integer
    
    row = Selection.row
    col = ActiveSheet.range("origin").Column
    
    Set target = sh.range(sh.Cells(row, col), sh.Cells(row, col + cyclenum - 1))
    
    target = 0
    
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
    
End Sub

Sub set_coloring_rule(ByVal value As Integer, ByVal row As Integer)
    Dim target As range
    Dim sh As Worksheet
    Dim col As Integer
    
    If value = 0 Then
        Exit Sub
    End If
    
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

Sub CopyToSelection()
Attribute CopyToSelection.VB_ProcData.VB_Invoke_Func = "Z\n14"
    Dim target As range
    Dim value As Double
    Dim colnum As Integer
    Dim origin As range
    Set origin = ActiveSheet.range("origin")
    
    Call get_settings
        
    Set target = Selection(1, 1)
    
    If target.row > origin.row And target.row < origin.row + varnum And target.Column >= origin.Column And target.Column < origin.Column + cyclenum - 1 Then
        
    
        value = target(1, 1)
        
        colnum = Selection.Columns.Count
        
        
        ActiveSheet.range(target.Offset(0, 0), target.Offset(0, colnum - 1)) = value
        
        Call set_coloring_rule(value, target.row)
    End If
End Sub

Sub CopyToLast()
Attribute CopyToLast.VB_ProcData.VB_Invoke_Func = "X\n14"
    Dim value As Double
    Dim col As Integer
    Dim target As range
    Dim origin As range
    
    Set origin = ActiveSheet.range("origin")
    
    Call get_settings
    
    
    Set target = Selection(1, 1)
    
    If target.row > origin.row And target.row < origin.row + varnum And target.Column >= origin.Column And target.Column < origin.Column + cyclenum - 1 Then
    
        value = target(1, 1)
        col = target.Column
        
        ActiveSheet.range(target.Offset(0, 0), target.Offset(0, cyclenum - col + 2)) = value
        
        Call set_coloring_rule(value, target.row)
    End If
End Sub

