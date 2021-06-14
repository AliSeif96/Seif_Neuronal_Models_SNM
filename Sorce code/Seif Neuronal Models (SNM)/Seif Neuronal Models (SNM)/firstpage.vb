Public Class firstpage


    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        If ComboBox1.Text = "Kuramoto" Then
            ComboBox1.Text = "Choose your model"
            Kuramoto.Show()
        Else
            Kuramoto.Close()
        End If
    End Sub

    Private Sub firstpage_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Kuramoto.Close()
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged
        Kuramoto.Close()
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        End
    End Sub

    Private Sub Label1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Label1.Click

    End Sub
End Class
