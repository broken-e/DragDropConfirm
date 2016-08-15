/******************************** Module Header ********************************\
Module Name:  FileDragDropExt.cpp
Project:      CppShellExtDragDropHandler
Copyright (c) Microsoft Corporation.

The code sample demonstrates creating a Shell drag-and-drop handler with C++.

When a user right-clicks a Shell object to drag an object, a context menu is
displayed when the user attempts to drop the object. A drag-and-drop handler
is a context menu handler that can add items to this context menu.

The example drag-and-drop handler adds the menu item "Create hard link here" to
the context menu. When you right-click a file and drag the file to a directory or
a drive or the desktop, a context menu will be displayed with the "Create hard
link here" menu item. By clicking the menu item, the handler will create a hard
link for the dragged file in the dropped location. The name of the link is "Hard
link to <source file name>".

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/en-us/openness/licenses.aspx#MP.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\*******************************************************************************/

#include "FileDragDropExt.h"
#include <strsafe.h>
#include <Shlwapi.h>
#include <string>
#include <sstream>
#include "Reg.h"


#pragma comment(lib, "shlwapi.lib")

extern long g_cDllRef;


#define IDM_RUNFROMHERE            0  // The command's identifier offset

FileDragDropExt::FileDragDropExt() : m_cRef(1),
m_pszMenuText(L"Don't move") {
	InterlockedIncrement(&g_cDllRef);
}


FileDragDropExt::~FileDragDropExt() {
	InterlockedDecrement(&g_cDllRef);
}


#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP FileDragDropExt::QueryInterface(REFIID riid, void **ppv) {
	static const QITAB qit[] =
	{
		QITABENT(FileDragDropExt, IContextMenu),
		QITABENT(FileDragDropExt, IShellExtInit),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileDragDropExt::AddRef() {
	return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileDragDropExt::Release() {
	ULONG cRef = InterlockedDecrement(&m_cRef);
	if (0 == cRef) {
		delete this;
	}
	return cRef;
}

#pragma endregion


#pragma region IShellExtInit

// Initialize the drag and drop handler.  If we return S_OK, then QueryContextMenu will be called. 
// Otherwise this handler will not be used in this event.
IFACEMETHODIMP FileDragDropExt::Initialize(
	LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID) {
	// Get the directory where the file is dropped to.
	if (!SHGetPathFromIDList(pidlFolder, this->m_szTargetDir)) {
		return E_FAIL;
	}

	// Get the file(s) being dragged.
	if (NULL == pDataObj) {
		return E_INVALIDARG;
	}

	HRESULT hr = E_FAIL;

	FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stm;

	// The pDataObj pointer contains the objects being acted upon. In this 
	// example, we get an HDROP handle for enumerating the dragged files and 
	// folders.
	if (SUCCEEDED(pDataObj->GetData(&fe, &stm))) {
		// Get an HDROP handle.
		HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
		if (hDrop != NULL) {
			// if there's at least one file or folder, then we should use this handler.
			UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			if (nFiles > 0) {
				hr = S_OK; // our QueryContextMenu will be called.
			}
			GlobalUnlock(stm.hGlobal);
		}

		ReleaseStgMedium(&stm);
	}
	// If any value other than S_OK is returned from the method, the QueryContextMenu will not be called.
	return hr;
}

#pragma endregion


#pragma region IContextMenu

//
//   FUNCTION: FileDragDropExt::QueryContextMenu
//
//   PURPOSE: The Shell calls IContextMenu::QueryContextMenu to allow the 
//            context menu handler to add its menu items to the menu. It 
//            passes in the HMENU handle in the hmenu parameter. The 
//            indexMenu parameter is set to the index to be used for the 
//            first menu item that is to be added.
//
IFACEMETHODIMP FileDragDropExt::QueryContextMenu(
	HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) {
	// If uFlags include CMF_DEFAULTONLY then we should not do anything.
	if (CMF_DEFAULTONLY & uFlags)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
	}

	// get default menu item position
	int defPos = GetMenuDefaultItem(hMenu, TRUE, 0);
	//Initialize MENUITEMINFO structure:
	MENUITEMINFO dmii;
	memset(&dmii, 0, sizeof(dmii));
	dmii.cbSize = sizeof(dmii);
	dmii.fMask = MIIM_TYPE;
	dmii.fType = MFT_STRING;
	dmii.cch = 256;
	wchar_t szString[256];
	dmii.dwTypeData = szString;
	// copy default menu item's info into dmii
	if (!GetMenuItemInfo(hMenu, defPos, TRUE, &dmii))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	wchar_t itemText[MAX_PATH] = L"&Move here";
	// try to retrieve overridden itemText from registry
	HRESULT hr = GetHKLMRegistryKeyAndValue(L"SOFTWARE\\DragDropConfirm\\", L"ItemText", itemText, MAX_PATH);

	if (!SUCCEEDED(hr))
	{ // if overridden text is not found, see if we want to display the value in a messagebox
		wchar_t defaultText[4];
		hr = GetHKLMRegistryKeyAndValue(L"SOFTWARE\\DragDropConfirm\\", L"ShowDefaultText", defaultText, 4);
		if (SUCCEEDED(hr) && defaultText[0])
		{ // we'll display the actual current default value, for language customization
		  // Dialog is always topmost
			MessageBoxW(0, dmii.dwTypeData, L"The Default Item's Value Is:", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		}
	}

	// If default menu item's string doesn't equal "&Move here" or the overridden text, then we don't do anything. 
	if (wcscmp(itemText, dmii.dwTypeData))
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
	}

	// set messagebox title and description
	wchar_t askTitle[MAX_PATH] = L"Hold up there...";
	GetHKLMRegistryKeyAndValue(L"SOFTWARE\\DragDropConfirm\\", L"AskTitle", askTitle, MAX_PATH);
	wchar_t askDescription[1024] = L"Are you sure you want to move the file(s) or folder(s)?";
	GetHKLMRegistryKeyAndValue(L"SOFTWARE\\DragDropConfirm\\", L"AskDescription", askDescription, 1024);

	// ask if we want to do the default action (should be moving files)
	// Dialog is always topmost
	int button = MessageBoxW(0, askDescription, askTitle, MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2| MB_TOPMOST);
	if (button == IDCANCEL)
	{ // add the com-blocking menu item to stop default move action
		// Use either InsertMenu or InsertMenuItem to add menu items.
		MENUITEMINFO mii = { sizeof(mii) };
		mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
		mii.wID = idCmdFirst + IDM_RUNFROMHERE;
		mii.fType = MFT_STRING;
		mii.dwTypeData = m_pszMenuText;
		mii.fState = MFS_ENABLED;
		if (!InsertMenuItem(hMenu, indexMenu, TRUE, &mii))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// set new item to default
		if (!SetMenuDefaultItem(hMenu, (UINT)idCmdFirst, (UINT)FALSE))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		//   // Return an HRESULT value with the severity set to SEVERITY_SUCCESS. 
		//   // Set the code value to the offset of the largest command identifier 
		//   // that was assigned, plus one (1).
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_RUNFROMHERE + 1));
	}
	else
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
	}
}


//
//   FUNCTION: FileDragDropExt::InvokeCommand
//
//   PURPOSE: This method is called when a user clicks a menu item to tell 
//            the handler to run the associated command. It seems to not be called unless
//			  we actually add a menu item in QueryContextMenu.  That's ok, we don't do
//            anything here anyway.
//
IFACEMETHODIMP FileDragDropExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici) {
	return S_OK;
	//return E_FAIL;
}


//
//   FUNCTION: FileDragDropExt::GetCommandString
//
//   PURPOSE: If a user highlights one of the items added by a context menu 
//            handler, the handler's IContextMenu::GetCommandString method is 
//            called to request a Help text string that will be displayed on 
//            the Windows Explorer status bar. This method can also be called 
//            to request the verb string that is assigned to a command. 
//            Either ANSI or Unicode verb strings can be requested. This 
//            example does not need to specify any verb for the command, so 
//            the method returns E_NOTIMPL directly. 
//
IFACEMETHODIMP FileDragDropExt::GetCommandString(UINT_PTR idCommand,
	UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax) {
	return E_NOTIMPL;
}

#pragma endregion
