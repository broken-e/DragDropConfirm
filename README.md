##DragDropConfirm

If you have ever worked in a Windows environment where many users shared files on a server, you have probably seen it happen where a folder or file is suddenly gone, only to be discovered in some other folder (often a sibling folder). This is usually due to an accident that is very easy to do: press slightly too hard on the mouse button while moving the cursor over the windows explorer window, causing a drag and drop move event with no confirmation.

So... this is a simple shell extension for Windows to create that confirmation.  It is essentially a "fork" of the CppShellExtDragDropHandler example found here: https://code.msdn.microsoft.com/CppShellExtDragDropHandler-bbdb6bac 

You can view the Microsoft Open Source Licenses here: http://www.microsoft.com/en-us/openness/licenses.aspx

NOTE: I have only tested this with Windows 7.  

###How this works:
First, an important understanding about the way Windows drag and drop works: if you right-click to drag and drop, upon dropping the file, it will not be moved immediately, but instead explorer will open a context menu, with **Move here** bolded because it is the default. Now, what happens when you left-click drag and drop is that this context menu is silently used and the default is chosen automatically. 

So basically what the code does is catch this drag and drop context menu, check to see if the default is **Move here**, and if so, pops up a dialog window, asking if you are sure you want to move. If you say OK, it lets go of its stranglehold on your context menu and allows the move. If you hit Cancel, it will create a *new* context menu item called **Don't move**, which does exactly what it says, and it will set that as the default item.

The main code is in `FileDragDropExt.cpp`, in the overridden `IShellExtInit::Initialize` and `IContextMenu::QueryContextMenu` methods. First, in the `Initialize` method, we check to see if the objects involved include at least one file or folder.  If so, the result is `S_OK`, otherwise the next methods will not be called. Second, in `QueryContextMenu`, we get the current context menu ready to be manipulated, and read its default menu item.  If this menu item's text is equal to `"&Move here"` (as it would be if an accidental drag/drop were about to happen), then we open a dialog window asking if you really want to move the folders. On OK, we do nothing.  On Cancel, we create a new `MENUITEMINFO`, add it to the context menu, and then set it as default.
