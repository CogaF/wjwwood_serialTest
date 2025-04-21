#pragma once

#include <wx/wx.h>
#include "App.h"
#include "Id.h"
#include <serial/serial.h>
#include <wx/listctrl.h>
#include <wx/event.h>
#include <thread>
#include <wx/clipbrd.h>
#include <wx/event.h>
#include <sstream>
#include <iomanip>
#define MaxNrOfCMDs 25
wxDEFINE_EVENT(wxEVT_THREAD_RESULT, wxCommandEvent);


class MainWindow : public wxFrame
{
public:
    MainWindow();
    wxListCtrl*     resultList;
    wxString        ArrayOfresponses[MaxNrOfCMDs]; 
    bool            blockingThread = false;
    //Main Sizer that will contain all components
    wxPanel*        mainPanel;
    wxBoxSizer*     mainSizer;
    wxBoxSizer*     componentsSizer;
    //Sizer for commands for now contains only run button
    //sizer for the list
    wxBoxSizer*     resultsSizer;
    //The RUN button
    wxButton*       runBT;
    wxButton*       threadBT;
    wxArrayString   listOfPorts_wxArr;
    wxChoice*       listOfPorts_wxChoice;
    std::string     serialPortName;
    serial::Serial* serialPort_sp;
    bool            quitThread = true;
    bool            threadExists = false;

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxString get_current_timestamp();
    void OnKeyDown(wxKeyEvent& event);
    void CopySelectedRow();
    void OnMouseMove(wxMouseEvent& event);
    void SelectAllItems();
    void OnButtonEvent(wxCommandEvent& event);
    void OnStartThread(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void StartThread(const wxString& input, int commandIndex, bool sequentialStatus);
    void OnThreadResult(wxCommandEvent& event);
    void onRunCommand(wxCommandEvent& event);
    std::string RunCommand(const std::string& command, int commandIndex);
    void AddMessage(const wxString& timestamp, const wxString& message, const wxString& messag2, const wxString& messag3);
    void serliaPortsToChoices();
    wxString BytesToWxString(const unsigned char* data, size_t size); 
    std::vector<uint8_t> WxStringToBytes(const wxString& input);
};
