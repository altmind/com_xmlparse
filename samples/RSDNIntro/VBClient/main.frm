VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Client"
   ClientHeight    =   2745
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4020
   LinkTopic       =   "Form1"
   ScaleHeight     =   2745
   ScaleWidth      =   4020
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtTime 
      Height          =   285
      Index           =   2
      Left            =   2400
      TabIndex        =   8
      Top             =   2040
      Width           =   1215
   End
   Begin VB.TextBox txtTime 
      Height          =   285
      Index           =   1
      Left            =   2400
      TabIndex        =   7
      Top             =   1440
      Width           =   1215
   End
   Begin VB.TextBox txtTime 
      Height          =   285
      Index           =   0
      Left            =   2400
      TabIndex        =   6
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton cmdStatic 
      Caption         =   "Static Binding"
      Height          =   375
      Left            =   240
      TabIndex        =   5
      Top             =   2040
      Width           =   1695
   End
   Begin VB.CommandButton cmdIDBinding 
      Caption         =   "ID Binding"
      Height          =   375
      Left            =   240
      TabIndex        =   4
      Top             =   1440
      Width           =   1695
   End
   Begin VB.CommandButton cmdDynamic 
      Caption         =   "Dynamic"
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   840
      Width           =   1695
   End
   Begin VB.TextBox txtResult 
      Height          =   285
      Left            =   2400
      TabIndex        =   2
      Top             =   360
      Width           =   1095
   End
   Begin VB.TextBox txtOp2 
      Height          =   285
      Left            =   1320
      TabIndex        =   1
      Text            =   "10"
      Top             =   360
      Width           =   855
   End
   Begin VB.TextBox txtOp1 
      Height          =   285
      Left            =   240
      TabIndex        =   0
      Text            =   "55"
      Top             =   360
      Width           =   855
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Const ITERATIONS = 40000
Private Sub cmdDynamic_Click()
    ' An example of late binding
    Dim obj As Object
    Dim Start, i  As Long
    Set obj = CreateObject("math.math.1")
    Start = Timer
    txtResult = obj.Add(txtOp1, txtOp2)
    For i = 1 To ITERATIONS
        obj.Add txtOp1, txtOp2
    Next
    txtTime(0) = Timer - Start
    Set obj = Nothing
End Sub

Private Sub cmdIDBinding_Click()
    ' An example of ID (early) binding
    Dim obj As MFCMathComponent
    Dim Start, i  As Long
    Set obj = New MFCMathComponent
    Start = Timer
    txtResult = obj.Add(txtOp1, txtOp2)
    For i = 1 To ITERATIONS
        obj.Add txtOp1, txtOp2
    Next
    txtTime(1) = Timer - Start
End Sub

Private Sub cmdStatic_Click()
    ' An example of late (very early) binding
    Dim obj As New MathComponent
    Dim Start, i  As Long
    Start = Timer
    txtResult = obj.Add(txtOp1, txtOp2)
    For i = 1 To ITERATIONS
        obj.Add txtOp1, txtOp2
    Next
    txtTime(2) = Timer - Start
End Sub
