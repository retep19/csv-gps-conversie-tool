
#include "log.h"
#include <fstream>
#include <sstream>
#include <time.h>

//for listbox ID: IDC_LIST1
#include "csv-gps-conversie-tool.h"

bool debug_active = false;
bool log_active = true;


void send_debug_msg(std::string message) {

    if (debug_active == false) {
        return;
    }
    std::wstring widestr = std::wstring(message.begin(), message.end());
    const wchar_t* widecstr = widestr.c_str();
    MessageBox(NULL, widecstr, L"debug message to window!", MB_OK);
    send_log_info(message);
}

void send_debug_msg(std::wstring message) {
    if (debug_active == false) {
        return;
    }
    //std::wstring widestr = std::wstring(message.begin(), message.end());
    const wchar_t* widecstr = message.c_str();
    MessageBox(NULL, widecstr, L"debug message to window!", MB_OK);
}

void send_debug_msg(const char* message) {
    if (debug_active == false) {
        return;
    }
    const WCHAR* wmessage;
    // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, message, -1, NULL, 0);
    // allocate it
    wmessage = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, message, -1, (LPWSTR)wmessage, nChars);
    // use it....
    MessageBox(NULL, wmessage, L"debug message!", MB_OK);
    // delete it
    delete[] wmessage;
}

void send_log_info(std::string msg) {
    std::string filepath = get_filepath();
    std::ofstream log_file(filepath, std::ios_base::out | std::ios_base::app);
    log_file << time(0) << "," << "INFO," << msg << std::endl;
}

void send_log_error(std::string msg) {
    std::string filepath = get_filepath();
    std::ofstream log_file(filepath, std::ios_base::out | std::ios_base::app);
    log_file << time(0) << "," << "ERROR," << msg << std::endl;
}

void send_log_lbox_debug(HWND hwnd, std::string msg) {

    if (debug_active == false) {
        return;
    }
    std::string filepath = get_filepath();
    std::stringstream DLGmsg;
    std::ofstream log_file(filepath, std::ios_base::out | std::ios_base::app);
    log_file << time(0) << "," << "DEBUG," << msg << std::endl;
    SendDlgItemMessageA(hwnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)msg.c_str());
}

void send_log_lbox_info(HWND hwnd, std::string msg) {
    std::string filepath = get_filepath();
    std::stringstream DLGmsg;
    std::ofstream log_file(filepath, std::ios_base::out | std::ios_base::app);
    log_file << time(0) << "," << "INFO," << msg << std::endl;
    SendDlgItemMessageA(hwnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)msg.c_str());
}

void send_log_lbox_error(HWND hwnd, std::string msg) {
    std::string filepath = get_filepath();
    std::stringstream DLGmsg;
    std::ofstream log_file(filepath, std::ios_base::out | std::ios_base::app);
    log_file << time(0) << "," << "ERROR," << msg << std::endl;
    DLGmsg << time(0) << "," << "INFO," << msg << std::endl;
    SendDlgItemMessageA(hwnd, IDC_LIST1, LB_INSERTSTRING, 0, (LPARAM)msg.c_str());
}

std::string get_filepath() {
    
    std::stringstream pathstr;
    std::tm timeinfo; 
    time_t now = time(0);

    localtime_s(&timeinfo, &now); 

    pathstr << "log\\" << (timeinfo.tm_year + 1900) << (timeinfo.tm_mon + 1) << timeinfo.tm_mday << ".log";
    return pathstr.str();
}