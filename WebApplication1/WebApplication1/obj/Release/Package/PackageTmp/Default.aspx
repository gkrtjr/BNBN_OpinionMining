<%@ Page Title="Home Page" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="WebApplication1._Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

     <div class="row">
        <div class="col-md-4">
            <h2>Search</h2>
            <p>
                
                <asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>
                &nbsp;
                <asp:Button ID="Button2" runat="server" OnClick="Button2_Click" Text="Button" Width="64px" />
                <asp:BulletedList ID="BulletedList1" runat="server"></asp:BulletedList>
            </p>
        </div>
    </div>

</asp:Content>