#pragma once

struct FileLocation {
	wchar_t Path[_MAX_PATH];
	wchar_t Drive[_MAX_DRIVE];
	wchar_t Directory[_MAX_DIR];
	wchar_t Name[_MAX_FNAME];
	wchar_t Extension[_MAX_EXT];
};