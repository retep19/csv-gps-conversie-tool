#pragma once

#include "resource.h"
#include <string>
#include <vector>

void opencsvfile(HWND hWnd);

void read_bessy_object_importfile(HWND hWnd, std::vector<std::string> colnames, std::string filepath_s);

int check_bessy_objectenimport(HWND hWnd, std::vector<std::string> col_names);

int parse_bessy_objectenimport(HWND hWnd, std::string filepath_s);

std::string check_and_convert_gps(std::string input);