<%@ Page Title="Home Page" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="WebApplication1._Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

     <div class="row">
        <div class="col-md-4">
            <h2>감정분석</h2>

            <p>
                <asp:TextBox ID="TextBox1" runat="server" Width="302px" Height="21px"></asp:TextBox>
                &nbsp;&nbsp;&nbsp;&nbsp;
                <asp:Button ID="Button2" runat="server" OnClick="Button2_Click" Text="검색" Width="64px" />
                &nbsp;&nbsp;&nbsp;&nbsp;
                <%--<asp:RadioButtonList ID="RadioButtonList1" runat="server" Height="25px" Width="185px" OnSelectedIndexChanged="RadioButtonList1_SelectedIndexChanged" RepeatDirection="Horizontal" RepeatLayout="Flow" style="margin-top: 0px" >
                    <asp:ListItem Text="기본" Selected="True"/>
                    <asp:ListItem Text="v2"/>
                    <asp:ListItem Text="v3"/>
                </asp:RadioButtonList>--%>
                <asp:BulletedList ID="BulletedList1" runat="server"></asp:BulletedList>
            </p>
        </div>
    </div>

</asp:Content>