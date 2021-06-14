Public NotInheritable Class Kuramoto
    Private Sub Kuramoto_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        firstpage.Hide()


    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        firstpage.Show()

    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        End
    End Sub

    Private Sub CheckBox8_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox8.CheckedChanged
        If CheckBox8.Checked = True Then
            CheckBox9.Checked = False
            Label10.Visible = False
            Label12.Visible = False
            TextBox4.Visible = True
            TextBox8.Visible = False
            TextBox9.Visible = False

        Else
            CheckBox9.Checked = True

        End If
    End Sub

    Private Sub CheckBox9_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox9.CheckedChanged
        If CheckBox9.Checked = True Then
            CheckBox8.Checked = False
            Label10.Visible = True
            Label12.Visible = True
            TextBox4.Visible = False
            TextBox8.Visible = True
            TextBox9.Visible = True
        Else
            CheckBox8.Checked = True

        End If
    End Sub

    Private Sub CheckBox2_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox2.CheckedChanged
        If CheckBox2.Checked = True Then
            CheckBox1.Checked = False
            Label11.Visible = False
            Label13.Visible = False
            TextBox12.Visible = True
            TextBox11.Visible = False
            TextBox10.Visible = False

        Else
            CheckBox1.Checked = True

        End If
    End Sub

    Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox1.CheckedChanged
        If CheckBox1.Checked = True Then
            CheckBox2.Checked = False
            Label11.Visible = True
            Label13.Visible = True
            TextBox12.Visible = False
            TextBox11.Visible = True
            TextBox10.Visible = True
        Else
            CheckBox2.Checked = True

        End If



    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        Dim file As System.IO.StreamWriter

        file = My.Computer.FileSystem.OpenTextFileWriter("data.txt", False)
        file.WriteLine("data")
        If CheckBox9.Checked = True Then
            file.WriteLine("1")
        Else
            file.WriteLine("0")
        End If
        If CheckBox8.Checked = True Then
            file.WriteLine("1")
        Else
            file.WriteLine("0")
        End If
        If CheckBox1.Checked = True Then
            If CheckBox9.Checked = True Then
                file.WriteLine("1")
            Else
                file.WriteLine("0")
            End If
        Else
            file.WriteLine("0")
        End If
        file.WriteLine(TextBox1.Text)
        file.WriteLine(TextBox2.Text)
        file.WriteLine(TextBox3.Text)
        file.WriteLine(TextBox5.Text)
        file.WriteLine(TextBox6.Text)
        file.WriteLine(TextBox7.Text)

        If CheckBox8.Checked = True Then
            file.WriteLine(TextBox4.Text)
            file.WriteLine(TextBox4.Text)
        Else
            file.WriteLine(TextBox8.Text)
            file.WriteLine(TextBox9.Text)
        End If

        If CheckBox2.Checked = True Then
            file.WriteLine(TextBox12.Text)
            file.WriteLine(TextBox12.Text)
        Else
            file.WriteLine(TextBox11.Text)
            file.WriteLine(TextBox10.Text)
        End If


        Shell("CMD.EXE /c g++ Source.cpp")
        file.Close()





        Me.Button4.PerformClick()

    End Sub

    Private Sub Button4_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles Button4.Click

        MsgBox("Please wait until the end of the run  (click to run the program)", MsgBoxStyle.Information, "Please wait")


        Shell("CMD.EXE /k a")




    End Sub
End Class
