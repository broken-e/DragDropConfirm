 ___________________________________________
//                                         \\
||      DragDropConfirm by Trey Miller     ||
\\                                         //
 \\------------ { Broken e; } ------------//
 //                                       \\ 
//   http://broken-e.com/dragdropconfirm   \\
\\_________________________________________//

 
A modification of Microsoft's CppShellExtDragDropHandler to provide a confirmation dialog upon a drag and drop move event in a Windows environment.

//===========================================================================\\
||  - - - - - - - - - - - - -  DESCRIPTION - - - - - - - - - - - - - - - - - ||
\\===========================================================================//

If you have ever worked in a Windows environment where many users shared files on a server, you have probably seen it happen where a folder or file is suddenly gone, only to be discovered in some other folder (often a sibling folder). This is usually due to an accident that is very easy to do: press slightly too hard on the mouse button while moving the cursor over the windows explorer window, causing a drag and drop move event with no confirmation.

So... this is a simple shell extension for Windows to create that confirmation. It is essentially a "fork" of the CppShellExtDragDropHandler example found here: https://code.msdn.microsoft.com/CppShellExtDragDropHandler-bbdb6bac

You can view the Microsoft Open Source Licenses here: http://www.microsoft.com/en-us/openness/licenses.aspx

How this works:

First, an important understanding about the way Windows drag and drop works: if you right-click to drag and drop, upon dropping the file, it will not be moved immediately, but instead explorer will open a context menu, with Move here bolded because it is the default. Now, what happens when you left-click drag and drop is that this context menu is silently used and the default is chosen automatically.

So basically what the code does is catch this drag and drop context menu, check to see if the default is Move here, and if so, pops up a dialog window, asking if you are sure you want to move. If you say OK, it lets go of its stranglehold on your context menu and allows the move. If you hit Cancel, it will create a new context menu item called Don't move, which does exactly what it says, and it will set that as the default item.

The main code is in FileDragDropExt.cpp, in the overridden IShelExtInit::Initialize and IContextMenu::QueryContextMenu methods. First, in the Initialize method, we check to see if the objects involved include at least one file or folder. If so, the result is S_OK, otherwise the next methods will not be called. Second, in QueryContextMenu, we get the current context menu ready to be manipulated, and read its default menu item. If this menu item's text is equal to "&Move here" (as it would be if an accidental drag/drop were about to happen), then we open a dialog window asking if you really want to move the folders. On OK, we do nothing. On Cancel, we create a new MENUITEMINFO, add it to the context menu, and then set it as default.

//===========================================================================\\
||  - - - - - - - - - - - - - LICENSE INFO - - - - - - - - - - - - - - - - - ||
\\===========================================================================//
(Copied from http://www.microsoft.com/en-us/openness/licenses.aspx on 2014-10-19)

Microsoft maintains two licenses that have been certified by the Open Source Initiative (OSI). Certification by the OSI means that developers can be confident that the licenses meet the terms of the Open Source Definition. This page contains the text of those licenses; developers are free to use them as they wish within their works.

On this page:

Microsoft Public License
Microsoft Reciprocal License
Microsoft Public License (Ms-PL)

This license governs use of the accompanying software. If you use the software, you accept this license. If you do not accept the license, do not use the software.

Definitions
The terms "reproduce," "reproduction," "derivative works," and "distribution" have the same meaning here as under U.S. copyright law.
A "contribution" is the original software, or any additions or changes to the software.
A "contributor" is any person that distributes its contribution under this license.
"Licensed patents" are a contributor's patent claims that read directly on its contribution.
Grant of Rights
(A) Copyright Grant- Subject to the terms of this license, including the license conditions and limitations in section 3, each contributor grants you a non-exclusive, worldwide, royalty-free copyright license to reproduce its contribution, prepare derivative works of its contribution, and distribute its contribution or any derivative works that you create.
(B) Patent Grant- Subject to the terms of this license, including the license conditions and limitations in section 3, each contributor grants you a non-exclusive, worldwide, royalty-free license under its licensed patents to make, have made, use, sell, offer for sale, import, and/or otherwise dispose of its contribution in the software or derivative works of the contribution in the software.
Conditions and Limitations
(A) No Trademark License- This license does not grant you rights to use any contributors' name, logo, or trademarks.
(B) If you bring a patent claim against any contributor over patents that you claim are infringed by the software, your patent license from such contributor to the software ends automatically.
(C) If you distribute any portion of the software, you must retain all copyright, patent, trademark, and attribution notices that are present in the software.
(D) If you distribute any portion of the software in source code form, you may do so only under this license by including a complete copy of this license with your distribution. If you distribute any portion of the software in compiled or object code form, you may only do so under a license that complies with this license.
(E) The software is licensed "as-is." You bear the risk of using it. The contributors give no express warranties, guarantees, or conditions. You may have additional consumer rights under your local laws which this license cannot change. To the extent permitted under your local laws, the contributors exclude the implied warranties of merchantability, fitness for a particular purpose and non-infringement.
Microsoft Reciprocal License (Ms-RL)

This license governs use of the accompanying software. If you use the software, you accept this license. If you do not accept the license, do not use the software.

Definitions
The terms "reproduce," "reproduction," "derivative works," and "distribution" have the same meaning here as under U.S. copyright law.
A "contribution" is the original software, or any additions or changes to the software.
A "contributor" is any person that distributes its contribution under this license. 
"Licensed patents" are a contributor's patent claims that read directly on its contribution.
Grant of Rights
(A) Copyright Grant- Subject to the terms of this license, including the license conditions and limitations in section 3, each contributor grants you a non-exclusive, worldwide, royalty-free copyright license to reproduce its contribution, prepare derivative works of its contribution, and distribute its contribution or any derivative works that you create.
(B) Patent Grant- Subject to the terms of this license, including the license conditions and limitations in section 3, each contributor grants you a non-exclusive, worldwide, royalty-free license under its licensed patents to make, have made, use, sell, offer for sale, import, and/or otherwise dispose of its contribution in the software or derivative works of the contribution in the software.
Conditions and Limitations
(A) Reciprocal Grants- For any file you distribute that contains code from the software (in source code or binary format), you must provide recipients the source code to that file along with a copy of this license, which license will govern that file. You may license other files that are entirely your own work and do not contain code from the software under any terms you choose.
(B) No Trademark License- This license does not grant you rights to use any contributors' name, logo, or trademarks.
(C) If you bring a patent claim against any contributor over patents that you claim are infringed by the software, your patent license from such contributor to the software ends automatically.
(D) If you distribute any portion of the software, you must retain all copyright, patent, trademark, and attribution notices that are present in the software.
(E) If you distribute any portion of the software in source code form, you may do so only under this license by including a complete copy of this license with your distribution. If you distribute any portion of the software in compiled or object code form, you may only do so under a license that complies with this license.
(F) The software is licensed "as-is." You bear the risk of using it. The contributors give no express warranties, guarantees, or conditions. You may have additional consumer rights under your local laws which this license cannot change. To the extent permitted under your local laws, the contributors exclude the implied warranties of merchantability, fitness for a particular purpose and non-infringement.
Microsoft
Privacy Statement
Trademarks
Terms Of Use
© 2012 Microsoft