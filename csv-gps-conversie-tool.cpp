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

//verwachtte opmaak csv bessy objecten_import bestand
static std::vector<std::string> colnames_obj_bessy_import = {"Naam","Paneel","Face id","Object model","Paneel model","Object locatie","Locatie naam","Locatietype","Straat","Nummer","Plaats","Bushalte","Metro","Hectometer","Wegnummer","Opmerkingen","Wisselovereenkomst","Serviceovereenkomsten","GPS Latitude","GPS Longitude","N/A"};

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

                        //test if it is a csv file
                        if (filepath_s.substr(filepath_s.length() - 3) != "csv") {
                            messagestream.str(L"");
                            messagestream << L"Bestandstype niet herkend, bestand niet ingelezen. Klik op file->open om een importbestand in te lezen";
                            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
                            return;
                        }

                        //find max number of lines
                        std::ifstream inFile(filepath_s);
                        int n_rows = std::count(std::istreambuf_iterator<char>(inFile),
                            std::istreambuf_iterator<char>(), '\n');

                        //make modeless progress dialog
                        if (!IsWindow(hwndDLGPB)) {
                            hwndDLGPB = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_PROGRESS), hWnd, ProgressProc, n_rows);
                            ShowWindow(hwndDLGPB, SW_SHOW);
                        }

                        //read import file  as csv as new thread
                        std::thread task(read_import_file, std::ref(hwndDLGPB), pszFilePath);
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

//threaded task //open import file, read creation date , when found/opened verwerk importbestand.
void read_import_file(HWND hWndDLG, PWSTR pszFilePath) {

    int parsed;
    std::stringstream creation_date;
    std::wstring filepath_w = pszFilePath;
    std::string filepath_s(filepath_w.begin(), filepath_w.end());
    HANDLE hFile;
    std::tm Peildatum;

    //print out filelocation in log and listbox
    send_log_lbox_info(hWndDLG, filepath_s);

    //zoek bestandscreatiedatum en zet als Peildatum
    hFile = CreateFile(pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        send_log_lbox_error(hWndDLG, "INVALID_HANDLE_VALUE hfile, bestand niet kunnen openen.");
        CloseHandle(hFile);
        goto end;
    }
    else if (!GETFileCreationDate(hFile, Peildatum)) {
        send_log_lbox_error(hWndDLG, "Bestandsdatum niet kunnen lezen [Peildatum], bestand niet kunnen openen.");
        CloseHandle(hFile);
        goto end;
    }
    else {
        creation_date << "Peildatum gevonden [" << Peildatum.tm_mday << "-" << (Peildatum.tm_mon + 1) << "-" << (Peildatum.tm_year + 1900) << "]";
        send_log_lbox_info(hWndDLG, creation_date.str());
        CloseHandle(hFile);
    }

    //verwerk importbestand
    parse_import_file(hWndDLG, filepath_s, Peildatum);

end:
    //maak button "Ok" beschikbaar om door te gaan. (direct without logging)
    SendDlgItemMessage(hWndDLG, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");
    SendDlgItemMessage(hWndDLG, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L"Klik op ok om door te gaan");
    SendDlgItemMessage(hWndDLG, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)L" ");
    EnableWindow(GetDlgItem(hWndDLG, IDOK), TRUE);
    return;



}

//zoek datum van creeren bestand and return als Peildatum
BOOL GETFileCreationDate(HANDLE hFile, std::tm& Peildatum) {

    int year, month, day;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;

    // Convert the creation time to local time.
    FileTimeToSystemTime(&ftCreate, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    //fill tm struct
    Peildatum.tm_year = stLocal.wYear - 1900;
    Peildatum.tm_mon = stLocal.wMonth - 1;
    Peildatum.tm_mday = stLocal.wDay;
    return TRUE;
}

//Zoek importbestand type en verwerk bestand naar gelang type. return 1 als volledig ingelezen, -1 als niet of deels ingelezen.
int parse_import_file(HWND hWndDLG, std::string filepath_s, std::tm Peildatum) {

    int rc = 0;
    int checktype = 0;
    int parsed = 0;

    checktype = check_importtype_csv(filepath_s);

    switch (checktype) {
    case 1:
        //schade
        send_log_lbox_info(hWndDLG, "Start inlezen bessy objecten_import bestand.");
        parsed = parse_bessy_objectenimport(hWndDLG, filepath_s);
        break;
    default:
        send_log_lbox_info(hWndDLG, "Inhoud importbestand niet herkend.");
        break;

    }

    if (parsed == 1) {
        send_log_lbox_info(hWndDLG, "Importbestand volledig ingelezen.");
        rc = 1;
    }
    else {
        send_log_lbox_info(hWndDLG, "Importbestand niet (volledig) ingelezen.");
        rc = -1;
    }
    return rc;
}

//checks for type import file : obj_bessy_import[1] , unknown[-1]
int check_importtype_csv(std::string filepath_s) {

    if (check_columns_csv(colnames_obj_bessy_import, filepath_s) == 1) {
        return 1;
    }
    else {
        return -1;
    }
}

//checks vector of columns in col_cmp are present in file; if all present outputs 1, else -1
int check_columns_csv(std::vector<std::string> col_cmp, std::string filepath_s) {

    int rc = -1;
    //number of columns found
    int nocolumnsfound = 0;
    std::stringstream log;
    int n_rows, n_cols;
    double delta_progress;
    std::vector<std::string> colnames;

    //fetch column names
    try {
        auto info = get_file_info(filepath_s);
        n_rows = info.n_rows;
        n_cols = info.n_cols;
        colnames = info.col_names;
        delta_progress = 100 / n_rows;

    }
    catch (std::exception& e) {
        return -1;
    }
    //for lenght of vector
    for (unsigned int i = 0; i < colnames.size(); i++) {
        std::string teststr = colnames[i];
        if (std::find(col_cmp.begin(), col_cmp.end(), colnames[i]) != col_cmp.end()) {

            nocolumnsfound++;
        }
    }

    //test if all columns found
    if (nocolumnsfound == col_cmp.size() - 1) {
        rc = 1;
    }

    log.str("");
    log << "[" << nocolumnsfound << "] van [" << (col_cmp.size() - 1) << "] gevonden in importbestand";
    send_log_info(log.str());
    return rc;
}

int parse_bessy_objectenimport(HWND hWndDLG, std::string filepath) {

    csv::string_view filepath_v(filepath);
    CSVReader reader(filepath_v);
    std::stringstream tmp_str;

    std::string filename_out = filepath;
    filename_out.replace(filename_out.end() - 4, filename_out.end(), "_gps_geconverteerd.csv");
    std::ofstream file_out(filename_out);

    std::vector<std::string> col_cmp = colnames_obj_bessy_import;

    int rowno = 1; // 1 = header
    HWND hwndProgressBar = GetDlgItem(hWndDLG, IDC_PROGRESS1);

    send_log_lbox_info(hWndDLG, filepath);
    send_log_lbox_info(hWndDLG, "converteren gps-coordinaten in csv-file");

    if (file_out.is_open()) {
        //read file per row (task) and write to output , check for correct gps coordinates.

        //write header to output
        file_out << col_cmp[0];
  
        for (int i = 1; i<(col_cmp.size()-1); i++) {
            file_out << "," << col_cmp[i];
        }
        file_out << "\n";

        for (CSVRow& row : reader) { //input iterator

            rowno++;
            //write line to ouput
            //std::string test = row[col_cmp[0]].get();
            file_out << row[col_cmp[0]].get();
            for (int i = 1; i < (col_cmp.size() - 3); i++) {
                std::string tempstr = row[col_cmp[i]].get();
                file_out << "," << row[col_cmp[i]].get();
            }
            file_out << "," << check_and_convert_gps(hWndDLG, row["GPS Latitude"].get(),0);
            file_out << "," << check_and_convert_gps(hWndDLG, row["GPS Longitude"].get(),1);
            file_out << "\n";

            //update info log
            tmp_str << "Lijn[" << rowno << "] verwerkt.";
            send_log_lbox_info(hWndDLG, tmp_str.str());
            tmp_str.str("");

            //step up progressbar
            SendMessage(hwndProgressBar, PBM_STEPIT, 0, 0);

        }
    }
    else {
        send_log_lbox_error(hWndDLG, "Kan output bestand niet maken!");
        return -1;
    }

    if (file_out.is_open()) { file_out.close(); }

    return 1;
}

std::string check_and_convert_gps(HWND hWndDLG, std::string input, int latlon) {
    size_t start_pos = 0;
    int converted = 0;
    std::string ofnlat = "51.86453480159791";
    std::string ofnlon = "5.318332207240367";
    std::string str = input;
    std::stringstream info;

    //change [,] to [ ]
    while ((start_pos = str.find(",", start_pos)) != std::string::npos) {
        str.replace(start_pos, 1, ".");
        start_pos = start_pos + 2;
        info << "komma gevonden en verwijderd. Input [" << input << "]";
        converted = 1;
        send_log_info(info.str());
        info.str("");
    }

    //check for multiple points (delete all but first and store firstlocation)
    int pointloc = 0;
    int pointcount = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '.') {
            if (pointcount == 0) {
                pointcount++;
                pointloc = i;
            }
            else {
                pointcount++;
                str.erase(i, 1);
                info << "dubbele punt gevonden en verwijderd. Input [" << input << "]";
                send_log_info(info.str());
                info.str("");
                converted = 1;
                i--; // string is opgeschoven, volgende character is nu huidige character.
            }

        }
    }

    //if no points where found then add point on expected location 
    if (input != "" && pointloc == 0) {
        if (latlon == 0) {
            info << "geen punt gevonden , toegevoegd. Input [" << input << "]";
            send_log_info(info.str());
            info.str("");
            converted = 1;
            str.insert(2,".");
        }
        if (latlon == 1) {
            info << "geen punt gevonden , toegevoegd. Input [" << input << "]";
            send_log_info(info.str());
            info.str("");
            converted = 1;
            str.insert(1, ".");
        }
    }

    //check if gps lat or long has expected format for netherlands, else delete input (lat 49. - 53. | lon 4. - 6.)
    std::string beforedot = str.substr(0, pointloc); 
    int beforedot_int;
    try { beforedot_int = stoi(beforedot); }
    catch (...)
    {
        beforedot_int = 0;
    }
    //check number if lattitude
    if (latlon == 0) {
        if (49 < beforedot_int && beforedot_int > 53) { 
            info << "geen geldige GPS [NL] , gps lattitude verwijderd. Input [" << input << "]";
            send_log_info(info.str());
            info.str("");
            converted = 1;
            str = ""; }
    //check number if Longitude     
    }
    if (latlon == 1) {
        if (4 < beforedot_int && beforedot_int > 6) { 
            info << "geen geldige GPS [NL] , gps longitude verwijderd. Input [" << input << "]";
            send_log_info(info.str());
            info.str(""); 
            converted = 1;
            str = ""; }
    }

    //if string is empty ( input empty or gps corrupted) add OFN coordinates.
    if (str.compare("") == 0) {
        if (latlon == 0) {
            info << "geen geldige GPS, lattitude aangepast naar OFN [" << ofnlat << "]. Input [" << input << "]";
            send_log_lbox_info(hWndDLG, info.str());
            info.str("");
            converted = 1;
            return ofnlat;
         }
        else if (latlon == 1) {
            info << "geen geldige GPS, longitude aangepast naar OFN [" << ofnlon << "]. Input [" << input << "]";
            send_log_lbox_info(hWndDLG, info.str());
            info.str("");
            converted = 1;
            return ofnlon;
        }
        else {
            return input;
        }
    }

    //report listbox and log.
    if (converted == 1) {
        if (latlon == 0) {
            info << "GPS aangepast, lattitude aangepast naar [" << str << "]. Input [" << input << "]";
            send_log_lbox_info(hWndDLG, info.str());
            info.str("");
        }
        else if (latlon == 1) {
            info << "GPS aangepast, longitude aangepast naar [" << str << "]. Input [" << input << "]";
            send_log_lbox_info(hWndDLG, info.str());
            info.str("");
        }
    }
    
    return str;
 }