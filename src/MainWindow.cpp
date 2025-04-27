// Start of wxWidgets "Hello World" Program

#include "MainWindow.h"


using namespace std;
using namespace windowIDs;



MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Serial Comunication")
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
    int ResultListWidth = 1950;
    // Set the proportions: Message is 8 times the width of Timestamp
    int timestampWidth = ResultListWidth / 9;   // 1 part of the total 9 parts (timestamp + 8 * message)
    int messageWidth = timestampWidth * 6; // 8 parts for message

    //Sizer for list 
    resultsSizer = new wxBoxSizer(wxVERTICAL);

    //Main sizer do i need this?
    mainSizer = new wxBoxSizer(wxVERTICAL);

    //sizer for all gui componens as: two buttons the drop down list of ports, and the result list
    componentsSizer = new wxBoxSizer(wxVERTICAL);

    mainPanel = new wxPanel(this, windowIDs::ID_MAIN_PANEL);

    resultList = new wxListCtrl(mainPanel, windowIDs::ID_COMMAND_LIST, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_AUTOARRANGE);


    SetSize(960,450);
    wxIcon _frame_icon(wxICON(MAINICON));
    SetIcon(_frame_icon);

    SetStatusText("Command Runner!");

    Bind(wxEVT_MENU, &MainWindow::OnHello, this, windowIDs::ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);

    // Add columns for timestamp and message
    resultList->InsertColumn(0, "Timestamp");
    resultList->InsertColumn(1, "Sent Message");
    resultList->InsertColumn(2, "Received Message");
    resultList->InsertColumn(3, "RX Interpretation");
    // Set the column widths
    resultList->SetColumnWidth(0, timestampWidth);      // Timestamp column
    resultList->SetColumnWidth(1, timestampWidth);      // Message column
    resultList->SetColumnWidth(2, timestampWidth);      // Message column
    resultList->SetColumnWidth(3, messageWidth);        // Message column

    runBT = new wxButton(mainPanel, windowIDs::ID_RUN_COMMAND_BT, "Send Message(s)", wxDefaultPosition, wxDefaultSize);
    threadBT = new wxButton(mainPanel, windowIDs::ID_START_THREAD_BT, "Create Listen Thread", wxDefaultPosition, wxDefaultSize);



    serliaPortsToChoices();

    listOfPorts_wxChoice = new wxChoice(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, listOfPorts_wxArr);
    if (listOfPorts_wxArr.size() >= 1) {
        listOfPorts_wxChoice->SetSelection(0);
    }

    serialPort_sp = new serial::Serial("", 115200, serial::Timeout::simpleTimeout(150));

    resultsSizer->Add(resultList, 1, wxEXPAND | wxALL, 1);

    componentsSizer->Add(runBT, 0, wxEXPAND | wxALL, 1);
    componentsSizer->Add(threadBT, 0, wxEXPAND | wxALL, 1);
    componentsSizer->Add(listOfPorts_wxChoice, 0, wxEXPAND | wxALL, 1);
    componentsSizer->Add(resultsSizer, 1, wxEXPAND | wxALL, 1);

    mainPanel->SetSizer(componentsSizer);

    mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

    //SetSizer(componentsSizer);
    SetSizer(mainSizer);
    mainPanel->Show(true);

    thisCRC8 = new CRC8();

    //Bindings palced at the end to avoid addin new bindings in position where components aren't created yet
    Bind(wxEVT_MENU, &MainWindow::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MainWindow::OnButtonEvent, this, ID_RUN_COMMAND_BT);
    Bind(wxEVT_BUTTON, &MainWindow::OnStartThread, this, ID_START_THREAD_BT);
    Bind(wxEVT_THREAD_RESULT, &MainWindow::OnThreadResult, this);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
    resultList->Bind(wxEVT_KEY_DOWN, &MainWindow::OnKeyDown, this);
    resultList->Bind(wxEVT_MOTION, &MainWindow::OnMouseMove, this);
    
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("onAbout Hello wxWidgets");
}


void MainWindow::serliaPortsToChoices() {

    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();
    wxString listofPorts = "";
    wxString result;
    while (iter != devices_found.end())
    {
        serial::PortInfo device = *iter++;
        listofPorts = wxString::Format("PORT IS: \"%s\"; - DESCRIPTION IS: \"%s\"; -  HARDWARE ID IS: \"%s\"\n", device.port, device.description.c_str(),
            device.hardware_id.c_str());
        listOfPorts_wxArr.Add(device.port);
        result += listofPorts;
        //wxLogMessage(listofPorts);
    } 
    /*
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
    */

    AddMessage(get_current_timestamp(), "Reading ports", "" , result);

}

void MainWindow::OnHello(wxCommandEvent& event)
{
    wxMessageBox("onHello Hello wxWidgets");
}



wxString MainWindow::get_current_timestamp() {
    using namespace std::chrono;

    // Get current time as a time_point
    system_clock::time_point now = system_clock::now();

    // Convert to system time
    std::time_t now_time = system_clock::to_time_t(now);

    // Convert to string
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));

    return wxString(buffer);
}


//Catching Ctrl+A and Ctrl+C for select all or copy 
void MainWindow::OnKeyDown(wxKeyEvent& event) {
    if (event.ControlDown() && event.GetKeyCode() == 'A') {  // Detect Ctrl+A
        SelectAllItems();
    }
    else if (event.ControlDown() && event.GetKeyCode() == 'C') {  // Detect Ctrl+C
        CopySelectedRow();
    }
    else {
        event.Skip();  // Let other key events process normally
    }
}

//Doing the actual selection of all rows
void MainWindow::SelectAllItems() {
    long itemIndex = -1;
    while ((itemIndex = resultList->GetNextItem(itemIndex, wxLIST_NEXT_ALL)) != wxNOT_FOUND) {
        // Select the item by setting its state to selected
        resultList->SetItemState(itemIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }
}

void MainWindow::OnMouseMove(wxMouseEvent& event) {
    int flags;  // Placeholder for hit-test flags
    long itemIndex = resultList->HitTest(event.GetPosition(), flags);

    if (itemIndex >= 0) {  // If a valid row is found
        wxString fullText = resultList->GetItemText(itemIndex, 0) + "\n" + resultList->GetItemText(itemIndex, 1)
                            + resultList->GetItemText(itemIndex, 2) + "\n" + resultList->GetItemText(itemIndex, 3);
        resultList->SetToolTip(fullText);  // Show full text as tooltip
    }
    else {
        resultList->SetToolTip("");  // Clear tooltip when not hovering over an item
    }

    event.Skip();  // Allow further event processing
}

void MainWindow::CopySelectedRow() {
    wxString allSelectedRows;

    // Start iterating through all selected rows
    long itemIndex = -1;
    while ((itemIndex = resultList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
        wxString rowText;

        // Get the text for each column in the row
        for (int col = 0; col < resultList->GetColumnCount(); col++) {
            rowText += resultList->GetItemText(itemIndex, col);
            if (col < resultList->GetColumnCount() - 1)
                rowText += "\t";  // Tab-separated for easy pasting into spreadsheets
        }

        allSelectedRows += rowText + "\n";  // Add the row to the final result with a newline
    }

    // If no rows are selected, return early
    if (allSelectedRows.IsEmpty())
        return;

    // Copy to clipboard
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(allSelectedRows));
        wxTheClipboard->Close();
    }
}
// Function to add message to the list


void MainWindow::AddMessage(const wxString& timestamp, const wxString& message1, const wxString& message2, const wxString& message3) {
    // Insert the new message at the top
    long index = resultList->InsertItem(0, timestamp);
    resultList->SetItem(index, 1, message1);
    resultList->SetItem(index, 2, message2);
    resultList->SetItem(index, 3, message3);
}


void MainWindow::OnClose(wxCloseEvent& event) {
    //if (event.CanVeto())
    //{
    //    if (wxMessageBox("Are you sure you want to quit, unfinished CMDs will be terminated!!!",
    //        "Request to quit application",
    //        wxICON_QUESTION | wxYES_NO) != wxYES)
    //    {
    //        event.Veto();
    //        return;
    //    }
    //}
    //blockingThread = false;
    //AddMessage(get_current_timestamp(), "Quitting app", "", "");
    Destroy();
}


//Converts a wxString into vector of uint8_t
std::vector<uint8_t> MainWindow::WxStringToBytes(const wxString& input)
{
    std::vector<uint8_t> result;
    wxCharBuffer buffer = input.ToUTF8();  // UTF-8 conversion

    if (buffer.data()) {
        const char* raw = buffer.data();
        size_t len = strlen(raw);
        result.assign(raw, raw + len);
    }

    return result;
}

//Sends some message containing few bytes , the amount of messages sent can be changed in the for loop
void MainWindow::OnButtonEvent(wxCommandEvent& event) {

    size_t readBytes = 0;
    //Setting port to 8N1
    wxString received = "";
    wxString interpretation = "nothing";
    unsigned long baudrate = 115200;
    //serialPortName will me taken from droplist
    serialPortName = listOfPorts_wxChoice->GetString(listOfPorts_wxChoice->GetSelection());
    std::vector<uint8_t> response = { 0x00 };
    try {
        /*
            Attention here, as a lazy person i didn't read info and only found after "unexpected" behaviour 
            that the creation of the serial port, tries to open the port in case it hase a port name.
            thus calling open() will give the exception of port open.
            i made a change to the library:
            when trying to open and it is already open it will get closed and reopened
        */
        if (serialPort_sp == nullptr) {
            serialPort_sp  = new serial::Serial("", baudrate, serial::Timeout::simpleTimeout(150));
        }


        serialPort_sp->setPort(serialPortName);
        serialPort_sp->setBytesize(serial::eightbits);
        serialPort_sp->setFlowcontrol(serial::flowcontrol_none);
        serialPort_sp->setStopbits(serial::stopbits_one);

        serial::Timeout thisTimeout(15, 25, 0, 25, 0);
        serialPort_sp->setTimeout(thisTimeout);
        if (!serialPort_sp->isOpen()) {
            serialPort_sp->open();
        }

        if (serialPort_sp->isOpen()) {
            for (uint8_t i = 0; i <= 0xFE; i++) {
                response.clear();
                uint8_t shiftedValue = i;
                int const msgSize = 12;        //SOH   addr  CM1   CM2   CM3   Len  STX    DAT1  DAT2  ETX   CRC   EOT
                std::vector<uint8_t> message = { 0x01, 0xff, 0x0A, 0x01, 0x00, 12,  0x02 ,   i,    i,  0x03, 0xF8, 0x04 };



                int* messageSize = new int(message.size());

                u8* arrayMessage = new u8[*messageSize];
                std::copy(message.begin(), message.end(), arrayMessage);
                thisCRC8->add(arrayMessage, message.size()-2);

                uint8_t thisCrc = getCRC_test((char*) &arrayMessage[0], messageSize);
                message.at(message.size() - 2) = thisCrc;

                //int const msgSize = 12;           //SOH   addr  CM1   CM2   CM3   Len  STX    DAT1  DAT2  ETX   CRC   EOT
                std::vector<uint8_t> messageOrig = { 0x01, 0xff, 0x01, 0x0A, 0x00, 12,  0x02 ,   i,    i,  0x03, 0xF8, 0x04 };

                int* messageSizeOrig = new int(messageOrig.size());
                char* charArrayOrig = new char[*messageSizeOrig];
                std::copy(messageOrig.begin(), messageOrig.end(), charArrayOrig);

                uint8_t thisCrcOrig = getCRC_test( &charArrayOrig[0], messageSizeOrig);
                messageOrig.at(message.size() - 2) = thisCrcOrig;

                wxString uCharMessageToWXs = BytesToWxString(&message[0], msgSize);
                wxString uCharMessageToWXs_orig = BytesToWxString(&messageOrig[0], msgSize);
                bool debugWithNoSerial = true;
                if (!debugWithNoSerial) {
                    if (!serialPortName.empty()) {
                        serialPort_sp->write(message);
                        std::this_thread::sleep_for(std::chrono::milliseconds(25));
                        serial::Timeout thisTimeout_1(1, 3, 0, 3, 0);
                        serialPort_sp->setTimeout(thisTimeout_1);
                        bool timeout = false;
                        int maxMillis = 1750;
                        int count = 0;
                        int waitingMillis = 5;
                        while (!timeout && !(serialPort_sp->available())) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(waitingMillis));
                            count += waitingMillis;
                            if (count >= maxMillis) {
                                timeout = true;
                            }
                            //this make gui responsive while cycling in while for timout
                            wxYield();
                        }
                        //serliaPort_sp->waitReadable();
                        readBytes = serialPort_sp->read(response, 2094);
                    }
                    else {
                        wxMessageBox("portname not selected");
                    }

                    if (response.size() > 38 && readBytes > 38) {
                        interpretation = "";
                        for (int j = 0; j < 16; ++j) {
                            int value = static_cast<unsigned char>(response[9 + j * 2]) +
                                (static_cast<unsigned char>(response[10 + j * 2]) << 8);
                            interpretation += wxString::Format("A%02d = %d; ", j, value);
                        }
                    }
                    int sizeOfVec = response.size();
                    if (sizeOfVec > 0) {
                        received = BytesToWxStringASCII(&response[0], sizeOfVec);
                    }
                }
                //received = BytesToWxString(&message[0], messageSize);
                //Printing a message to list to keep track
                AddMessage(get_current_timestamp(), uCharMessageToWXs, received, interpretation);
                AddMessage(get_current_timestamp(), uCharMessageToWXs_orig, received, interpretation);
                wxYield();
            }
            //serialPort_sp->close();
        }
        else {
            response = WxStringToBytes(wxString("Failed to open port: " + serialPort_sp->getPort() + "\n"));
        }
    }
    catch (const std::exception& e) {
        response = WxStringToBytes(wxString("Error: " + wxString::FromUTF8(e.what()) + "\n"));
        AddMessage(get_current_timestamp(), BytesToWxString(&response[0], response.size()), received, interpretation);
    }
    
}

//Converts bytes to string, except ascii characters all the other will be printed with h suffix e.g. 0x00 = h00
wxString MainWindow::BytesToWxStringASCII(const unsigned char* data, size_t size)
{
    wxString toReturn = "";

    for (size_t i = 0; i < size; ++i)
    {
        unsigned char _byte = data[i];

        if (_byte >= 0x20 && _byte <= 0x7E) // Printable ASCII
        {

            toReturn += wxString::Format("-%c", _byte);
        }
        else
        {
            toReturn += wxString::Format(" x%02X", _byte);
        }
    }

    return toReturn;
}

//Converts bytes to string, printed with x suffix e.g. 0x00 = " x00"
wxString MainWindow::BytesToWxString(const unsigned char* data, size_t size)
{
    wxString toReturn = "";
    for (size_t i = 0; i < size; ++i)
    {
        toReturn += wxString::Format(" x%02X", data[i]);
    }
    return toReturn;
}


// Function to run the command and capture output
std::string MainWindow::RunCommand(const std::string& command, int commandIndex) {

    // Create pipes for capturing output
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hRead, &hWrite, &saAttr, 0)) {
        return "Error: Failed to create pipe!";
    }

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = { 0 };

    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;  // Hides the window
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;  // Redirect stderr to stdout


    int viewFlag = CREATE_NO_WINDOW;
    // Create the process
    if (!CreateProcessA(NULL, (LPSTR)(("cmd /c \"" + command + "\"").c_str()), NULL, NULL, TRUE, viewFlag, NULL, NULL, &si, &pi)) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return "Error: Failed to execute command!";
    }

    // Close the write handle to allow reading from the pipe
    CloseHandle(hWrite);

    // Read the command output
    std::string result;
    char buffer[0xBFFFF];
    DWORD bytesRead;

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        wxYield();
        if (bytesRead >= sizeof(buffer)) {
            buffer[sizeof(buffer) - 1] = '\0';  // Prevent overflow
        }
        else {
            buffer[bytesRead] = '\0';  // Safe termination
        }
        result += buffer;
    }

    // Cleanup
    CloseHandle(hRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    wxYield();
    return result;
}


void MainWindow::onRunCommand(wxCommandEvent& event)
{
    wxYield();
}



void MainWindow::StartThread(const wxString& input, int CommandIndex, bool sequentialStatus) {
    // Start a background thread
    std::thread([this, input, CommandIndex]() {
        std::string result = RunCommand(input.ToStdString(), CommandIndex);

        wxYield();
        // Create and post an event to the main thread
        wxCommandEvent event(wxEVT_THREAD_RESULT);
        event.SetString(result);
        event.SetInt(CommandIndex);
        wxQueueEvent(this, event.Clone());

        }).detach();  // Join to run it sequentially

    wxYield();
#ifdef _DEBUG
    AddMessage(get_current_timestamp(), wxString::Format("Started thread for CMD %d", CommandIndex + 1), "", "");
#endif
}

//Starts a detached thread that prints every received message and sends it back as received: *** 
void MainWindow::OnStartThread(wxCommandEvent& event) {
    quitThread = !quitThread;
    if (!quitThread) {
        AddMessage(get_current_timestamp(), "Thread will be active", "", "");
    }else{
        AddMessage(get_current_timestamp(), "Thread will deactivate", "", "");
    }
    bool *thiSquitThread = &quitThread;
    serial::Serial* thisSerialPort = serialPort_sp;
    if(!threadExists){
        threadExists = true;
        std::thread([this, thiSquitThread, thisSerialPort]() {
            size_t readBytes = 0;
            std::vector<uint8_t> response = { 0x00 };
            try {
                unsigned long baudrate = 115200;
                wxString serialPortName = listOfPorts_wxChoice->GetString(listOfPorts_wxChoice->GetSelection());

                thisSerialPort->setPort(serialPortName.ToStdString());
                thisSerialPort->setBytesize(serial::eightbits);
                thisSerialPort->setFlowcontrol(serial::flowcontrol_none);
                thisSerialPort->setStopbits(serial::stopbits_one);

                serial::Timeout thisTimeout(1, 3, 0, 3, 0);
                thisSerialPort->setTimeout(thisTimeout);
                if (!thisSerialPort->isOpen()) {
                    thisSerialPort->open();
                }
                if (!serialPortName.empty()) {
                    while (!(*thiSquitThread)) {
                        wxYield();
                        while (thisSerialPort->available()) {
                            readBytes = thisSerialPort->read(response, 256);
                        }
                        if (readBytes > 1) {
                            AddMessage(get_current_timestamp(), "", BytesToWxStringASCII(&response[0], response.size()), "");
#ifdef _DEBUG
                            //AddMessage(get_current_timestamp(), "", BytesToWxString(&response[0], response.size()), ""); 
                            int randomMillis = getRandom(150);
                            //std::this_thread::sleep_for(std::chrono::milliseconds(randomMillis));
                            //thisSerialPort->write("received: ");
                            //thisSerialPort->write(response);
#endif
                            readBytes = 0;
                            response = { 0x00 };
                        }
                    }
                    thisSerialPort->close();
                    threadExists = false; 
                    *thiSquitThread = true;
                }
            }
            
            catch (const std::exception& e) {
                response = WxStringToBytes(wxString("Error: " + wxString::FromUTF8(e.what()) + "\n"));
                AddMessage(get_current_timestamp(), BytesToWxString(&response[0], response.size()), "", "");
                threadExists = false;
            }
        }).detach();  // Join to run it sequentially
    }
    wxYield();
}

int MainWindow::getRandom(int maxValue) {
    static std::random_device rd;               // Seed
    static std::mt19937 gen(rd());              // Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, maxValue);
    return dist(gen);
}

void MainWindow::OnThreadResult(wxCommandEvent& event) {
}



uint8_t  MainWindow::getCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr) {

    thisCRC8->restart();

    for (int i = 0; i < (*thisSizePtr - 2); i++) {
        thisCRC8->add(*(thisMsgPtr + i));
    }
    return thisCRC8->calc();

}

uint8_t  MainWindow::getCRC_test(char* const thisMsgPtr, int* const thisSizePtr) {

    thisCRC8->restart();
    for (int i = 0; i < (*thisSizePtr - 2); i++) {
        thisCRC8->add((uint8_t)*(thisMsgPtr + i));
    }
    return thisCRC8->calc();

}

bool MainWindow::addCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr) {
    thisCRC8->restart();
    bool toReturnBool = false;
    u8 myCRC = 0x00;
    thisCRC8->add(thisMsgPtr, (*thisSizePtr - 3));
    myCRC = thisCRC8->calc();

    *(thisMsgPtr + *thisSizePtr - 2) = myCRC;
    *(thisMsgPtr + *thisSizePtr - 1) = _EOT;

    toReturnBool = true;
    return true;
}
bool  MainWindow::checkCRC(uint8_t* const thisMsgPtr, uint8_t* const thisSizePtr) {
    thisCRC8->restart();
    bool toReturnBool = false;
    u8 myCRC = 0x00;
    u8 thisrxCRC = *(thisMsgPtr + *thisSizePtr - 2);

    // for(int i = 0; i <= (*thisrxSizePtr-3); i++){
    //     myCRC = myCRC xor *(thisrxPtr + i);
    // }


    thisCRC8->add(thisMsgPtr, (*thisMsgPtr - 3));
    myCRC = thisCRC8->calc();


    if (myCRC == thisrxCRC) {
        toReturnBool = true;
    }


    // Serial.print(thisrxCRCPtr);
    // Serial.print(myCRC);
    return toReturnBool;

}