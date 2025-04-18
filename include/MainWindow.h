#pragma once

#include <wx/wx.h>
#include "../include/App.h"
#include "../include/Id.h"
#include <serial/serial.h>


class MainWindow : public wxFrame
{
public:
    MainWindow();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

