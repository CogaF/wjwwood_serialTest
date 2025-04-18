// Start of wxWidgets "Hello World" Program

#include "MainWindow.h"


using namespace std;
using namespace windowIDs;



MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Hello World")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MainWindow::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("About Hello wxWidgets");
}

void MainWindow::OnHello(wxCommandEvent& event)
{
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();
    wxString listofPorts = "";
    while (iter != devices_found.end())
    {
        serial::PortInfo device = *iter++;
        listofPorts = wxString::Format("PORT IS: \"%s\"; - DESCRIPTION IS: \"%s\"; -  HARDWARE ID IS: \"%s\"\n", device.port, device.description.c_str(),
            device.hardware_id.c_str());
        wxLogMessage(listofPorts);
    }
    wxString toPrint = "Below the list of COM ports (if any)\n";
    wxString result;
    const wxString& port = "COM7";
    unsigned long baudrate = 115200;

    try {
        serial::Serial rigol(port.ToStdString(), baudrate, serial::Timeout::simpleTimeout(1000));
        rigol.setBytesize(serial::eightbits);
        rigol.setFlowcontrol(serial::flowcontrol_none);
        rigol.setStopbits(serial::stopbits_one);
        if (rigol.isOpen()) {
            result += "Connected to " + port + "\n";
            wxString rigolGetPort = rigol.getPort();
            rigol.write("*IDN?\r\n");
            std::string response = rigol.read(1024);

            result += "rigolGetPort is: \"" + rigolGetPort + "\" andResponse:\n" + wxString::FromUTF8(response) + "\n";
        }
        else {
            result += "Failed to open port: " + port + "\n";
        }

    }
    catch (const std::exception& e) {
        result += "Error: " + wxString::FromUTF8(e.what()) + "\n";
    }

    wxLogMessage(result);
}