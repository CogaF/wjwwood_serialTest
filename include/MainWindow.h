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
#include <random>
#include "CRC8.h"
#define MaxNrOfCMDs 25
wxDEFINE_EVENT(wxEVT_THREAD_RESULT, wxCommandEvent);
/*
    Writing the whole uintx_t is messy so
    for my usage i will define a shorter way to use them
*/
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

class CRC8;

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
    CRC8*            thisCRC8;

private:

    u8 _EOT = 0x04;

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
    wxString BytesToWxStringASCII(const unsigned char* data, size_t size);
    std::vector<uint8_t> WxStringToBytes(const wxString& input);
    int getRandom(int maxValue);
    uint8_t getCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr);
    uint8_t getCRC_test(char* const thisMsgPtr, int* const thisSizePtr);
    bool addCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr);
    bool checkCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr);
};
