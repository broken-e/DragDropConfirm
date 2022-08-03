## DragDropConfirm

(previously called ~~DragDropInterceptor~~) 

If you have ever worked in a Windows environment where many users shared files on a server, you have probably seen it happen where a folder or file is suddenly gone, only to be discovered in some other folder (often a sibling folder). This is usually due to an accident that is very easy to do: press slightly too hard on the mouse button while moving the cursor over the windows explorer window, causing a drag and drop move event with no confirmation.

So... this is a simple shell extension for Windows to create that confirmation.  It is essentially a "fork" of the CppShellExtDragDropHandler example found here: https://code.msdn.microsoft.com/CppShellExtDragDropHandler-bbdb6bac 

You can view the Microsoft Open Source Licenses here: http://www.microsoft.com/en-us/openness/licenses.aspx

NOTE: I have only tested this with Windows 7.  

Update 1/9/2017: Version 1.2 modified for compatibility with Windows XP.
Note that the Item text in XP for "$Move Here" must be manually specified in the registry because of the capitalization of 'Here' is different.


### How this works:

First, an important understanding about the way Windows drag and drop works: if you right-click to drag and drop, upon dropping the file, it will not be moved immediately, but instead explorer will open a context menu, with **Move here** bolded because it is the default. Now, what happens when you left-click drag and drop is that this context menu is silently used and the default is chosen automatically. 

So basically what the code does is catch this drag and drop context menu, check to see if the default is **Move here**, and if so, pops up a dialog window, asking if you are sure you want to move. If you say OK, it lets go of its stranglehold on your context menu and allows the move. If you hit Cancel, it will create a *new* context menu item called **Don't move**, which does exactly what it says, and it will set that as the default item.

The main code is in `FileDragDropExt.cpp`, in the overridden `IShellExtInit::Initialize` and `IContextMenu::QueryContextMenu` methods. First, in the `Initialize` method, we check to see if the objects involved include at least one file or folder.  If so, the result is `S_OK`, otherwise the next methods will not be called. Second, in `QueryContextMenu`, we get the current context menu ready to be manipulated, and read its default menu item.  If this menu item's text is equal to `"&Move here"` (as it would be if an accidental drag/drop were about to happen), then we open a dialog window asking if you really want to move the folders. On OK, we do nothing.  On Cancel, we create a new `MENUITEMINFO`, add it to the context menu, and then set it as default.

### Configuring this tool:

Default settings are intended for English language. If the language of your operating system is not English or the dialog window just doesn't pop up when you drag a file in Windows Explorer, you should set some parameters to make this tool work.

Settings are read from registry key:
`HKEY_LOCAL_MACHINE\SOFTWARE\DragDropConfirm`

This key is not created automatically by installer. You should do it manually.

As for me it is a strategical mistake to keep settings there and should be changed to HKEY_CURRENT_USER in the future, with a fallback to HKEY_LOCAL_MACHINE, because multilanguage operating systems have per-user language settings.

- In this key the REG_DWORD value `ShowDefaultText` and set it to 1.

- If this value is 1 and you drag to copy or move, a dialog will appear showing the default command text for your language.

- Make a REG_SZ value named `ItemText` and set that to whatever was displayed in the dialog box. (For example, for English it is: &Move here). The & is part of it and is required if it appears.

- If you want to enable intercepting also copies, make `ItemText2` and ctrl/drag to see what the copy default text is, and put it into REG_SZ `ItemText2` value. Also make a REG_DWORD value called `WarnOnCopy` and set it to 1.

The possible values to customize in the key are:

| Name | Type | Meaning |
| --- | --- | --- |
| ShowDefaultText | REG_DWORD | Set to 1 and delete ItemText parameter to check what the default text for ItemText should be |
| ItemText        | REG_SZ    | String value to search in hidden menu to catch move operations |
| ItemText2       | REG_SZ    | String value to search in hidden menu to catch copy operations |
| AskTitle        | REG_SZ    | Title bar text of the popup window for move operation |
| AskTitle2       | REG_SZ    | Title bar text of the popup window for copy operation |
| AskDescription  | REG_SZ    | Popup window text for move operations |
| AskDescription2 | REG_SZ    | Popup window text for copy operations |
| ShowDirectory   | REG_DWORD | Set to 0 to stop directory path from being shown in the popup box |
| WarnOnCopy      | REG_DWORD | Set to 1 to warn on copies as well (when holding Ctrl or dragging across drives/shares) |
| DefaultButtonOK | REG_DWORD | Set to 1 to make "Ok" the default button |
