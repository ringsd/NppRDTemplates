//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <tchar.h>
#include <stdio.h>
#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <string>
using namespace std;

//
// The plugin data that Notepad++ needs
//
FuncItem		funcItem[FUNC_MAX];
ShortcutKey		shortcutKeyItem[FUNC_MAX];

#define MAX_DATA_TIME_SIZE	256
TCHAR	dateAndTimeFormat[MAX_DATA_TIME_SIZE];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

#define insertTemplateIdDef( id )					\
	void insertTemplate##id(){insertTemplate(id);}

#define insertTemplateIdPointer( id )					\
	insertTemplate##id

insertTemplateIdDef( 0 )
insertTemplateIdDef( 1 )
insertTemplateIdDef( 2 )
insertTemplateIdDef( 3 )
insertTemplateIdDef( 4 )
insertTemplateIdDef( 5 )
insertTemplateIdDef( 6 )
insertTemplateIdDef( 7 )
insertTemplateIdDef( 8 )
insertTemplateIdDef( 9 )

typedef void(*insertTemplateFunc)( void );

static insertTemplateFunc insertTemplateFuncArray[] = {
	insertTemplateIdPointer( 0 ),
	insertTemplateIdPointer( 1 ),
	insertTemplateIdPointer( 2 ),
	insertTemplateIdPointer( 3 ),
	insertTemplateIdPointer( 4 ),
	insertTemplateIdPointer( 5 ),
	insertTemplateIdPointer( 6 ),
	insertTemplateIdPointer( 7 ),
	insertTemplateIdPointer( 8 ),
	insertTemplateIdPointer( 9 ),
};

#define setCommandAndsetShortcutKey( name, id )					\
	setShortcutKey(FUNC_INSERT_Template0 + id, false, true, false, '0' + id );\
	setCommand(FUNC_INSERT_Template0 + id, name, insertTemplate##id, false);



//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	int i = 0;
	TCHAR templateName[nbChar];

	//get the date time format
	if( ::GetPrivateProfileString( _T("RDTemplates"), _T("DATE_TIME"), _T("yyyy:MM:dd hh:mm:ss"), dateAndTimeFormat, MAX_DATA_TIME_SIZE - 1, _T(".//plugins//Config//RDTemplates//RDTemplates.ini")) )
	{
		if( ::WritePrivateProfileString( _T("RDTemplates"), _T("DATE_TIME"), dateAndTimeFormat, _T(".//plugins//Config//RDTemplates//RDTemplates.ini")) )
		{
			//wriet private profile string failed
		}
	}
	setCommand(FUNC_INSERT_DATA_AND_TIME, TEXT("InsertData&Time"), insertDataAndTime, false);
	setShortcutKey(FUNC_INSERT_DATA_AND_TIME, false, false, false, VK_F7);

	//get the templates name
	for ( i=0; i<=9; i++ )
	{
		TCHAR getTemplateName[nbChar];
		wsprintf( templateName, _T("InsertTemplate%d"), i );
		if( ::GetPrivateProfileString( _T("RDTemplates"), templateName, templateName, getTemplateName, nbChar - 1, _T(".//plugins//Config//RDTemplates//RDTemplates.ini")) )
		{
			if( ::WritePrivateProfileString( _T("RDTemplates"), templateName, getTemplateName, _T(".//plugins//Config//RDTemplates//RDTemplates.ini")) )
			{
				//wriet private profile string failed
			}
		}
		setShortcutKey(FUNC_INSERT_Template0 + i, false, true, false, '0' + i);\
		setCommand(FUNC_INSERT_Template0 + i, getTemplateName, insertTemplateFuncArray[i], false);
	}
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}

void setShortcutKey(int index, bool ctrl, bool alt, bool shift, UCHAR key)
{
	if (index < FUNC_MAX) 
	{
		shortcutKeyItem[index]._isCtrl = ctrl;
		shortcutKeyItem[index]._isAlt = alt;
		shortcutKeyItem[index]._isShift = shift;
		shortcutKeyItem[index]._key = key;
	}
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, bool check0nInit) 
{
    if (index >= FUNC_MAX)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = &shortcutKeyItem[index];

    return true;
}

void parseDataAndTime( char* dateAndtime )
{
	int i = 0, j = 0, k = 0;
	TCHAR dateAndTimeFlag[] = _T("yMdhms");
	SYSTEMTIME st;
	char dataFormat[ 10 ];
	int  dataFormatData = 0;
	int dateAndTimeFlagLength = _tcslen(dateAndTimeFlag);
	int dateAndTimeFormatLength = _tcslen(dateAndTimeFormat);
	GetLocalTime( &st );
	for ( i=0; i<dateAndTimeFormatLength; i++ )
	{
		for ( j=0; j<dateAndTimeFlagLength; j++ )
		{
			if ( dateAndTimeFormat[i] == dateAndTimeFlag[j] )
			{
				int count = 0;
				while ( dateAndTimeFormat[i] == dateAndTimeFlag[j] )
				{
					count++;
					i ++;
				}

				switch( j )
				{
					case 0:
						dataFormatData = st.wYear;
						break;
					case 1:
						dataFormatData = st.wMonth;
						break;
					case 2:
						dataFormatData = st.wDay;
						break;
					case 3:
						dataFormatData = st.wHour;
						break;
					case 4:
						dataFormatData = st.wMinute;
						break;
					case 5:
						dataFormatData = st.wSecond;
						break;
					default:
						break;
				}
				wsprintfA( dataFormat, "%%0%dd", count );
				wsprintfA( &dateAndtime[k], dataFormat, dataFormatData );
				k += count;
			}
		}
		dateAndtime[k++] = (char)dateAndTimeFormat[i];
	}
	dateAndtime[k++] = 0;
}


void parseDataOrTimeA( char* dateOrTime, const char* dateOrTimeFormat, int dataOrTime )
{
	int i = 0, j = 0, k = 0;
	char dateFlag[] = "yMd";
	char timeFlag[] = "hms";
	char* dateOrTimeFlag = NULL;
	SYSTEMTIME st;
	char dataFormat[ 10 ];
	int  dataFormatData = 0;
	int dateFlagLength = strlen( dateFlag );
	int timeFlagLength = strlen( timeFlag );
	int dateOrTimeFlagLength = 0;
	int dateOrTimeFormatLength = strlen( dateOrTimeFormat );

	if ( dataOrTime )
	{
		dateOrTimeFlag = dateFlag;
		dateOrTimeFlagLength = dateFlagLength;
	}
	else
	{
		dateOrTimeFlag = timeFlag;
		dateOrTimeFlagLength = timeFlagLength;
	}
	GetLocalTime( &st );
	for ( i=0; i<dateOrTimeFormatLength; i++ )
	{
		for ( j=0; j<dateOrTimeFlagLength; j++ )
		{
			if ( dateOrTimeFormat[i] == dateOrTimeFlag[j] )
			{
				int count = 0;
				while ( dateOrTimeFormat[i] == dateOrTimeFlag[j] )
				{
					count++;
					i ++;
				}

				switch( j )
				{
				case 0:
					if ( dataOrTime )
					{
						dataFormatData = st.wYear;
					}
					else
					{
						dataFormatData = st.wHour;
					}
					break;
				case 1:
					if ( dataOrTime )
					{
						dataFormatData = st.wMonth;
					}
					else
					{
						dataFormatData = st.wMinute;
					}
					break;
				case 2:
					if ( dataOrTime )
					{
						dataFormatData = st.wDay;
					}
					else
					{
						dataFormatData = st.wSecond;
					}
					break;
				default:
					break;
				}
				wsprintfA( dataFormat, "%%0%dd", count );
				wsprintfA( &dateOrTime[k], dataFormat, dataFormatData );
				k += count;
			}
		}
		dateOrTime[k++] = dateOrTimeFormat[i];
	}
	dateOrTime[k++] = 0;
}

void insertDataAndTime()
{
    // Get the current scintilla
    int which = -1;
	char dataAndTime[MAX_DATA_TIME_SIZE];
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	if( ::GetPrivateProfileString( _T("RDTemplates"), _T("DATE_TIME"), _T("yyyy:MM:dd hh:mm:ss"), dateAndTimeFormat, MAX_DATA_TIME_SIZE - 1, _T(".//plugins//Config//RDTemplates//RDTemplates.ini")) )
	{
		//get private profile string failed, using the default value
	}
	parseDataAndTime( dataAndTime );
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_REPLACESEL, which, (LPARAM)dataAndTime);
	//NPPM_GETFILENAME
}

void insertTemplate( int id )
{
	// Get the current scintilla
	int which = -1;
	FILE *file;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	string str( "" );
	TCHAR templatefileName[MAX_PATH];

	TCHAR *filePath = new TCHAR[MAX_PATH];
	TCHAR *fileName = new TCHAR[MAX_PATH];
	TCHAR *fileExt = new TCHAR[MAX_PATH];
	char *filePathA = NULL;
	char *fileNameA = NULL;
	char *fileExtA = NULL;

	if ( filePath == NULL || fileName == NULL || fileExt == NULL )
	{
		return;
	}

	wsprintf( templatefileName, _T(".//plugins//Config//RDTemplates//RDTemplates%d.dat"), id );
	file = _tfsopen( templatefileName, _T("r"), _SH_DENYRD );
	if ( file )
	{
		int length = 0;
		char* buffer = 0;
		fseek( file, 0, SEEK_END );
		length = ftell( file );
		if( length && length < 10 * 1024 )
		{
			buffer = new char[length];
			if ( buffer )
			{
				ZeroMemory( buffer, length );
				fseek( file, 0, SEEK_SET );
				fread( buffer, 1, length, file );
				str.append( buffer );
				delete buffer;
			}
		}
		fclose( file );
	}

	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)filePath);
	::SendMessage(nppData._nppHandle, NPPM_GETNAMEPART, MAX_PATH, (LPARAM)fileName);
	::SendMessage(nppData._nppHandle, NPPM_GETEXTPART, MAX_PATH, (LPARAM)fileExt);

#ifdef UNICODE
	filePathA = new char[MAX_PATH*2];
	fileNameA = new char[MAX_PATH*2];
	fileExtA = new char[MAX_PATH*2];
	if ( filePath == NULL || fileName == NULL || fileExt == NULL )
	{
		if( filePath ) delete filePath;
		if( fileName ) delete fileName;
		if( fileExt ) delete fileExt;
		if( filePath ) delete filePath;
		if( fileName ) delete fileName;
		if( fileExt ) delete fileExt;
		return ;
	}

	WideCharToMultiByte( CP_ACP,NULL, filePath, -1 , filePathA, MAX_PATH*2, NULL, FALSE);
	WideCharToMultiByte( CP_ACP,NULL, fileName, -1 ,fileNameA, MAX_PATH*2, NULL, FALSE);
	WideCharToMultiByte( CP_ACP,NULL, fileExt, -1 ,fileExtA, MAX_PATH*2, NULL, FALSE);
#else
	filePathA = filePath;
	fileNameA = fileName;
	fileExtA = fileExt;
#endif

	const char* templateSingleDefined[] = { 
		"[FILE_PATH]",
		"[FILE_NAME]" ,
		"[FILE_EXTENSION]" ,
	};

	int i = 0;
	int singleDefinedCount = sizeof(templateSingleDefined)/(sizeof(char*));
	for ( i=0; i<singleDefinedCount; i++ )
	{
		int startPos = str.find( templateSingleDefined[i] );
		char* subString;
		while ( startPos != -1 )
		{
			switch ( i )
			{
			case 0:
				subString = filePathA;
				break;
			case 1:
				subString = fileNameA;
				break;
			case 2:
				subString = fileExtA;
				break;
			default:
				break;
			}
			str.replace( startPos, strlen(templateSingleDefined[i]), subString );
			startPos = str.find( templateSingleDefined[i] );
		}
	}

	const char* templateDoubleDefined[] = { 
		"[DATE_USER]" ,
		"[DATE_USER_END]" ,
		"[TIME_USER]" ,
		"[TIME_USER_END]" ,
	};
	int doubleDefinedCount = sizeof(templateDoubleDefined)/(sizeof(char*));
	for ( i=0; i<doubleDefinedCount; i+=2 )
	{
		int startPos = str.find( templateDoubleDefined[i] );
		int endPos = str.find( templateDoubleDefined[i+1] );
		while ( startPos != -1 && endPos != -1 )
		{
			string subString = str.substr( startPos + strlen( templateDoubleDefined[i] ), endPos - startPos - strlen( templateDoubleDefined[i] ) );
			char*  subStringOut = new char[(endPos - startPos + 1) * 2];
			if ( subStringOut )
			{
				ZeroMemory( subStringOut, (endPos - startPos + 1) * 2 );
				switch ( i )
				{
				case 0:
					parseDataOrTimeA( subStringOut, subString.c_str(), 1 );
					break;
				case 2:
					parseDataOrTimeA( subStringOut, subString.c_str(), 0 );
					break;
				default:
					break;
				}
				str.replace( endPos, strlen(templateDoubleDefined[i+1]), "" );
				str.replace( startPos, endPos - startPos, subStringOut );
				delete subStringOut;
			}
			startPos = str.find( templateDoubleDefined[i] );
			endPos = str.find( templateDoubleDefined[i+1] );
		}
	}


	// Scintilla control has no Unicode mode, so we use (char *) here
	::SendMessage(curScintilla, SCI_REPLACESEL, 0, (LPARAM)str.c_str());

	if( filePath ) delete filePath;
	if( fileName ) delete fileName;
	if( fileExt ) delete fileExt;
#ifdef UNICODE
	if( filePathA ) delete filePathA;
	if( fileNameA ) delete fileNameA;
	if( fileExtA ) delete fileExtA;
#endif
}

