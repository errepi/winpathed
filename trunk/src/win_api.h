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
#ifndef WIN_REGISTRY_H_
#define WIN_REGISTRY_H_

#include <QString>

//#define PE_REG_ENV_KEY  L"SOFTWARE\\Bioware\\SW\\Kotor\\test"
#define PE_REG_ENV_KEY  L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define PE_REG_PATH_KEY L"Path"
#define MAX_KEY_SIZE 32768 // 16,383 Unicode characters

int waSavePath(const QString& s);
int waLoadPath(QString *path);
QString waFormatMessage(int code);
void waRefreshEnvironment();


#endif /*WIN_REGISTRY_H_*/
