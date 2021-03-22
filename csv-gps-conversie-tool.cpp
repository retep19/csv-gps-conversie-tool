// csv-gps-conversie-tool.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <Shobjidl.h>
#include "csv-gps-conversie-tool.h"
#include "log.h" 
#include <iostream>
#include "csv.hpp"
#include <fstream>
#include <wchar.h>
#include "Commctrl.h"

using namespace csv;

//verwachtte opmaak csv bestand
static std::vector<std::string> col_cmp = {"Naam","Paneel","Face id","Object model","Paneel model","Object locatie","Locatie naam","Locatietype","Straat","Nummer","Plaats","Bushalte","Metro","Hectometer","Wegnummer","Opmerkingen","Wisselovereenkomst","Serviceovereenkomsten","GPS Latitude","GPS Longitude","N/A"};

#define MAX_LOADSTRING 100
#define PROGRESSBAR_TIMER_ID 1

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hwndDLGPB = NULL;                          // Handle of progress dialog
std::wstringstream messagestream;               // message stream fileimport
PWSTR pszFilePath;                              // filepath 

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    //initializations window messageboxen
    send_log_info("Import tool opgestart");
    messagestream << L"Import tool. Klik op file->open om een importbestand in te lezen";
    // Initialize global strings

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CSVGPSCONVERSIETOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CSVGPSCONVERSIETOOL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CSVGPSCONVERSIETOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CSVGPSCONVERSIETOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, 1200, 150, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_OPEN:
                opencsvfile(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...           
            if (messagestream.str().size() > 0) {
                TextOut(hdc, 5, 10, messagestream.str().c_str(), messagestream.str().size());
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK ProgressProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG: {
        //Fire a timer event each second.for checking if done!
        SetTimer(hwndDlg, PROGRESSBAR_TIMER_ID, 1000, NULL);

        //initialize progressbar
        HWND hwndProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS1);
        SendMessage(hwndProgressBar, PBM_SETRANGE32, 0, lParam);
        SendMessage(hwndProgressBar, PBM_SETSTEP, (WPARAM)1, 0); //set stepsize to 1

        //set textual counter "lijnen geimporteerd" to 0
        wchar_t buffer[10];
        _itow_s(0, buffer, 10);
        SetDlgItemText(hwndDlg, IDC_STATIC, buffer);
        return TRUE;
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwndDlg, wParam);
            hwndDLGPB = NULL;
            messagestream << L"Inhoud importbestand ingelezen. Klik op file->open om een importbestand in te lezen";
            return TRUE;
            break;
        case IDCANCEL:
            int msgboxID = MessageBox(NULL, (LPCWSTR)L"Weet u zeker dat u het importprocess wilt onderbreken?", (LPCWSTR)L"Cancel import", MB_ICONEXCLAMATION | MB_OKCANCEL | MB_DEFBUTTON2);
            switch (msgboxID)
            {
            case IDCANCEL:
                return TRUE;
                break;
            case IDOK:
                EndDialog(hwndDlg, wParam);
                hwndDLGPB = NULL;
                messagestream << L"Inhoud importbestand niet volledig ingelezen, gecancelled door gebruiker. Klik op file->open om een importbestand in te lezen";
                return TRUE;
                break;
            }
        }
    case WM_PARENTNOTIFY:
        return TRUE;
        break;
    case WM_TIMER: {
        /*
         * Catch the timer event that is fired each second. check if progress
         * bar is 100% , then enable ok button. also update text in dialogwindow to current rownnumber
         */
        HWND hwndProgressBar = GetDlgItem(hwndDlg, IDC_PROGRESS1);
        UINT iPos = SendMessage(hwndProgressBar, PBM_GETPOS, 0, 0);
        UINT maxrange = SendMessage(hwndProgressBar, PBM_GETRANGE, FALSE, NULL);
        wchar_t buffer[10];
        _itow_s(iPos, buffer, 10);
        SetDlgItemText(hwndDlg, IDC_STATIC, buffer);

        if (iPos >= maxrange) {
            //KillTimer(hwndDlg, PROGRESSBAR_TIMER_ID);
            EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
        }
        return TRUE;
        break;
    }
    }
    return FALSE;
}

//  FUNCTION: opencsvfile(HWND, UINT, WPARAM, LPARAM)
/*
//  PURPOSE: opens dialogbox to choose import file. check for file type and calls file reader
*/
void opencsvfile(HWND hWnd) {

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr))
                    {

                        std::wstring filepath_w = pszFilePath;
                        std::string filepath_s(filepath_w.begin(), filepath_w.end());
                        std::tm reportdate;

                        //test if it is a csv file
                        if (filepath_s.substr(filepath_s.length() - 3) != "csv") {
                            messagestream.str(L"");
                            messagestream << L"Bestandstype niet herkend, bestand niet ingelezen. Klik op file->open om een importbestand in te lezen";
                            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
                            return;
                        }

                        int n_rows, n_cols;
                        double delta_progress;
                        std::vector<std::string> colnames;
                        try {
                            auto info = get_file_info(filepath_s);
                            n_rows = info.n_rows;
                            n_cols = info.n_cols;
                            colnames = info.col_names;
                            delta_progress = 100 / n_rows;
                            messagestream.str(L"");
                            messagestream << L"processing: " << pszFilePath << L"\n [" << n_rows << "]rows in file";
                            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
                        }
                        catch (std::exception& e) {
                            send_debug_msg(e.what());
                            send_log_error(e.what());
                            messagestream.str(L"");
                            messagestream << L"bestand geopend in ander programma, of inhoud niet herkend. Bestand niet ingelezen. Klik op file->open om een importbestand in te lezen";
                            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
                            return;
                        }

                        //make modeless progress dialog
                        if (!IsWindow(hwndDLGPB)) {
                            hwndDLGPB = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_PROGRESS), hWnd, ProgressProc, n_rows + 1);
                            ShowWindow(hwndDLGPB, SW_SHOW);
                        }

                        //read import file  as csv as new thread
                        std::thread task(read_bessy_object_importfile, std::ref(hwndDLGPB), colnames, filepath_s);
                        task.detach();

                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
}



void read_bessy_object_importfile(HWND hWndDLG, std::vector<std::string> colnames, std::string filepath_s) {

    std::stringstream log_str;
    std::string log_path;

    if (check_bessy_objectenimport(hWndDLG, colnames) == 1) {
        //parse schade import file
        if (parse_bessy_objectenimport(hWndDLG, filepath_s) < 1) {
            return;
        }
    }
    else {
        messagestream.str(L"");
        messagestream << L"Inhoud importbestand niet herkend, bestand niet ingelezen. Klik op file->open om een importbestand in te lezen";
        return;
    }
    messagestream.str(L"");
    messagestream << L"Importbestand ingelezen. Klik op file->open om een volgend importbestand in te lezen";
}

int check_bessy_objectenimport(HWND hWnd, std::vector<std::string> col_names) {
    
    HWND hwndProgressBar = GetDlgItem(hWnd, IDC_PROGRESS1);
    
    if (col_cmp.size() != col_names.size() + 1) {
        //number of columns not matched
        return -2;
    }

    //for lenght of vector
    for (unsigned int i = 0; i < col_names.size(); i++) {
        std::string teststr = col_names[i];
        if (std::find(col_cmp.begin(), col_cmp.end(), col_names[i]) == col_cmp.end()) {
            //n/a column not available
            //todo: output which column not available
            //make progressbar full en make message all done
            std::stringstream tmp_str;
            tmp_str << "Onbekende kolom [" << teststr << "] in bestand. Bestand niet ingelezen!";
            send_log_lbox_error(hWnd, tmp_str.str());

            UINT maxrange = SendMessage(hwndProgressBar, PBM_GETRANGE, FALSE, NULL);
            SendMessage(hwndProgressBar, PBM_SETPOS, maxrange, NULL);
            SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");
            SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L"Klik op ok om door te gaan");
            SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");
            return -1;
        }
    }
    return 1;
}

int parse_bessy_objectenimport(HWND hWnd, std::string filepath) {

    csv::string_view filepath_v(filepath);
    CSVReader reader(filepath_v);
    std::stringstream tmp_str;

    std::string filename_out = filepath;
    filename_out.replace(filename_out.end() - 3, filename_out.end(), "_gps_geconverteerd.csv");
    std::ofstream file_out(filename_out);

    int rowno = 1; // 1 = header
    HWND hwndProgressBar = GetDlgItem(hWnd, IDC_PROGRESS1);

    send_log_lbox_info(hWnd, filepath);
    send_log_lbox_info(hWnd, "converteren gps-coordinaten in csv-file gestart");

    if (file_out.is_open()) {
        //read file per row (task) and write to output , check for correct gps coordinates.

        //write header to output
        file_out << col_cmp[0];
        for (int i = 1; (col_cmp.size() - 1); i++) {
            file_out << "," << col_cmp[i];
        }
        file_out << "\n";

        for (CSVRow& row : reader) { //input iterator

            rowno++;
            //write line to ouput
            file_out << row[col_cmp[0]].get();
            for (int i = 1; (col_cmp.size() - 3); i++) {
                if(row[col_cmp[0]].get().compare("GPS Latitude") == 0){
                    file_out << "," << check_and_convert_gps(row[col_cmp[0]].get());
                }
                else if(row[col_cmp[0]].get().compare("GPS Longitude") == 0){
                    file_out << "," << check_and_convert_gps(row[col_cmp[0]].get());
                }
                else {
                    file_out << "," << row[col_cmp[0]].get();
                }
            }
            file_out << "\n";

            //update info log
            tmp_str << "Lijn[" << rowno << "] verwerkt.";
            send_log_info(tmp_str.str());
            tmp_str.str("");

            //step up progressbar
            SendMessage(hwndProgressBar, PBM_STEPIT, 0, 0);

        }
    }
    else {
        send_log_error("Kan output bestand niet maken!");
        return -1;
    }

    if (file_out.is_open()) { file_out.close(); }

    //make progressbar full en make message all done
    UINT maxrange = SendMessage(hwndProgressBar, PBM_GETRANGE, FALSE, NULL);
    SendMessage(hwndProgressBar, PBM_SETPOS, maxrange, NULL);
    SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");
    SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L"Klik op ok om door te gaan");
    SendDlgItemMessage(hWnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");

    return 1;
}

std::string check_and_convert_gps(std::string input) {
    //check std:string string on:  [,] replacement [.] 
    size_t start_pos = 0;
    std::string str = input;
    std::stringstream info;

    //change ['] to [ ]
    while ((start_pos = str.find(",", start_pos)) != std::string::npos) {
        str.replace(start_pos, 1, ".");
        start_pos = start_pos + 2;
        info << "komma gevonden en vervangen in input [" << input << "]";
        send_log_info(info.str());
        info.str("");
    }
    //copy new string to output
    return str;
 }