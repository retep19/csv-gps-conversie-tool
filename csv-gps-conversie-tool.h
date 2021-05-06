#pragma once

#include "resource.h"
#include <string>
#include <vector>

void opencsvfile(HWND hWnd);
void read_import_file(HWND hWnd, PWSTR pszFilePath);
BOOL GETFileCreationDate(HANDLE hFile, std::tm& FilecreationDate);

int parse_import_file(HWND hWndDLG, std::string filepath_s, std::tm Peildatum);
int check_importtype_csv(std::string filepath_s);
int check_columns_csv(std::vector<std::string> col_cmp, std::string filepath_s);

int parse_bessy_objectenimport(HWND hWndDLG, std::string filepath_s);
std::string check_and_convert_gps(HWND hWndDLG, std::string input,int latlon);