#pragma once

#include <string>
#include <wtypes.h>


void send_debug_msg(std::string message);
void send_debug_msg(std::wstring message);
void send_debug_msg(const char* message);


void send_log_info(std::string msg);
void send_log_error(std::string msg);

void send_log_lbox_debug(HWND hwnd, std::string msg);
void send_log_lbox_info(HWND hWnd, std::string msg);
void send_log_lbox_error(HWND hWnd, std::string msg);

std::string get_filepath();