/* 
 * Copyright (C) Rudi Pettazzi <rudi.pettazzi@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <windows.h>
#include "win_api.h"

/** 
 * Translate the given win32 error-code.
 */
QString waFormatMessage(int code) {	
	QString msg;
	LPVOID buf;
    int status = FormatMessage(
    		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            0, code, 0, (LPTSTR)&buf, 0, NULL);
	if (status) {
    	msg = QString::fromUtf16(reinterpret_cast<const ushort *> (buf));
    } else {
        msg = "An unknown error has occured.";
    }
    LocalFree((HLOCAL)buf);
    return msg;
}

/** 
 * Save path to registry 
 */
int waSavePath(const QString& s)
{
    HKEY hKey;
    DWORD res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
    		PE_REG_ENV_KEY, 0, KEY_ALL_ACCESS, &hKey);
    
    if (res != ERROR_SUCCESS) {
    	return res;
    }

    std::wstring tmp = s.toStdWString();
    const wchar_t *path = tmp.c_str();
    // count the NULL terminator for RegSetValueEx
    DWORD cbdata = tmp.length() * sizeof(wchar_t) + 1; 
    
    res = RegSetValueEx(hKey, PE_REG_PATH_KEY, 0, REG_EXPAND_SZ, (LPBYTE) path, cbdata);
    RegCloseKey(hKey);
    
    if (res != ERROR_SUCCESS) {
        return res;
    }
    
	waRefreshEnvironment();
	
    return 0;
}

/** 
 * Load path value from registry into the string "path"  
 */
int waLoadPath(QString *path)
{
    HKEY  hKey;
    DWORD buflen;
    DWORD res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, PE_REG_ENV_KEY, 0, KEY_QUERY_VALUE, &hKey);
    
    if (res != ERROR_SUCCESS) {
        return res;
    }
    
    // get path size
    res = RegQueryValueEx(hKey, PE_REG_PATH_KEY, NULL, NULL, NULL, &buflen);

    if (res != ERROR_SUCCESS) {
    	RegCloseKey(hKey);
        return res;
    }

    // read path (the type of the key is REG_EXPAND_SZ, "a null-terminated 
    // string that contains unexpanded references to environment variables")
    QByteArray data(buflen, 0);
    res = RegQueryValueEx(hKey, PE_REG_PATH_KEY, NULL, NULL, 
    		reinterpret_cast<LPBYTE>(data.data()), &buflen);
    RegCloseKey(hKey);

    if (res != ERROR_SUCCESS) {
        return res;
    }

    //QString path = QString::fromWCharArray(buffer);
    *path = QString::fromUtf16(reinterpret_cast<const ushort*>(data.constData()));
    
    return 0;
}

/**
 * Broadcast to notify the running system of the environment variable update 
 */
void waRefreshEnvironment()
{
	const wchar_t *message = L"Environment";
    RegisterWindowMessage(message);    
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) message,
                       SMTO_ABORTIFHUNG, 5000, NULL);
}
