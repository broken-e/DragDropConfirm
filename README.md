##DragDropInterceptor
===================

A simple shell extension for Windows to stop accidental drag and drop moves of folders.  This is essentially a "fork" of the CppShellExtDragDropHandler found here: https://code.msdn.microsoft.com/CppShellExtDragDropHandler-bbdb6bac

NOTE: I have only tested this with Windows 7.  

###How this works:
First, an important understanding about the way Windows drag and drop works: if you right-click to drag and drop, it will not do so immediately, but instead will open a context menu, with **Move here** bolded because it is the default. Now, what happens when you left-click drag and drop, like normal? This context menu is silently used and the default is chosen automatically. 

So basically what the code does is catch this drag and drop context menu, check to see if the default is **Move here**, and if so, pops up a dialog window, asking if you are sure you want to move. If you say OK, it lets go of its stranglehold on your context menu and allows the move. If you hit Cancel, it will create a *new* context menu item called **Don't move**, which does exactly what it says, and it will set that as the default item.

The main code is in `FileDragDropExt.cpp`, in the overridden `IShelExtInit::Initialize` and `IContextMenu::QueryContextMenu` methods. First, in the `Initialize` method, we check to see if the objects involved include at least one file or folder.  If so, the result is `S_OK`, otherwise the next methods will not be called. Second, in `QueryContextMenu`, we get the current context menu ready to be manipulated, and read its default menu item.  If this menu item's text is equal to `"&Move here"` (as it would be if an accidental drag/drop were about to happen), then we open a dialog window asking if you really want to move the folders. On OK, we do nothing.  On Cancel, we create a new `MENUITEMINFO` that does nothing, add it to the context menu, and then set it as default.
