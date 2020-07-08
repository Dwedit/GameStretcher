#if _DEBUG

struct IUnknown;
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <time.h>

const char* GetWindowsMessageName(int wm)
{
	switch (wm)
	{
	case 0x0000: return "WM_NULL";
	case 0x0001: return "WM_CREATE";
	case 0x0002: return "WM_DESTROY";
	case 0x0003: return "WM_MOVE";
	case 0x0005: return "WM_SIZE";
	case 0x0006: return "WM_ACTIVATE";
	case 0x0007: return "WM_SETFOCUS";
	case 0x0008: return "WM_KILLFOCUS";
	case 0x000A: return "WM_ENABLE";
	case 0x000B: return "WM_SETREDRAW";
	case 0x000C: return "WM_SETTEXT";
	case 0x000D: return "WM_GETTEXT";
	case 0x000E: return "WM_GETTEXTLENGTH";
	case 0x000F: return "WM_PAINT";
	case 0x0010: return "WM_CLOSE";
	case 0x0011: return "WM_QUERYENDSESSION";
	case 0x0013: return "WM_QUERYOPEN";
	case 0x0016: return "WM_ENDSESSION";
	case 0x0012: return "WM_QUIT";
	case 0x0014: return "WM_ERASEBKGND";
	case 0x0015: return "WM_SYSCOLORCHANGE";
	case 0x0018: return "WM_SHOWWINDOW";
	case 0x001A: return "WM_SETTINGCHANGE";
	case 0x001B: return "WM_DEVMODECHANGE";
	case 0x001C: return "WM_ACTIVATEAPP";
	case 0x001D: return "WM_FONTCHANGE";
	case 0x001E: return "WM_TIMECHANGE";
	case 0x001F: return "WM_CANCELMODE";
	case 0x0020: return "WM_SETCURSOR";
	case 0x0021: return "WM_MOUSEACTIVATE";
	case 0x0022: return "WM_CHILDACTIVATE";
	case 0x0023: return "WM_QUEUESYNC";
	case 0x0024: return "WM_GETMINMAXINFO";
	case 0x0026: return "WM_PAINTICON";
	case 0x0027: return "WM_ICONERASEBKGND";
	case 0x0028: return "WM_NEXTDLGCTL";
	case 0x002A: return "WM_SPOOLERSTATUS";
	case 0x002B: return "WM_DRAWITEM";
	case 0x002C: return "WM_MEASUREITEM";
	case 0x002D: return "WM_DELETEITEM";
	case 0x002E: return "WM_VKEYTOITEM";
	case 0x002F: return "WM_CHARTOITEM";
	case 0x0030: return "WM_SETFONT";
	case 0x0031: return "WM_GETFONT";
	case 0x0032: return "WM_SETHOTKEY";
	case 0x0033: return "WM_GETHOTKEY";
	case 0x0037: return "WM_QUERYDRAGICON";
	case 0x0039: return "WM_COMPAREITEM";
	case 0x003D: return "WM_GETOBJECT";
	case 0x0041: return "WM_COMPACTING";
	case 0x0044: return "WM_COMMNOTIFY";
	case 0x0046: return "WM_WINDOWPOSCHANGING";
	case 0x0047: return "WM_WINDOWPOSCHANGED";
	case 0x0048: return "WM_POWER";
	case 0x004A: return "WM_COPYDATA";
	case 0x004B: return "WM_CANCELJOURNAL";
	case 0x004E: return "WM_NOTIFY";
	case 0x0050: return "WM_INPUTLANGCHANGEREQUEST";
	case 0x0051: return "WM_INPUTLANGCHANGE";
	case 0x0052: return "WM_TCARD";
	case 0x0053: return "WM_HELP";
	case 0x0054: return "WM_USERCHANGED";
	case 0x0055: return "WM_NOTIFYFORMAT";
	case 0x007B: return "WM_CONTEXTMENU";
	case 0x007C: return "WM_STYLECHANGING";
	case 0x007D: return "WM_STYLECHANGED";
	case 0x007E: return "WM_DISPLAYCHANGE";
	case 0x007F: return "WM_GETICON";
	case 0x0080: return "WM_SETICON";
	case 0x0081: return "WM_NCCREATE";
	case 0x0082: return "WM_NCDESTROY";
	case 0x0083: return "WM_NCCALCSIZE";
	case 0x0084: return "WM_NCHITTEST";
	case 0x0085: return "WM_NCPAINT";
	case 0x0086: return "WM_NCACTIVATE";
	case 0x0087: return "WM_GETDLGCODE";
	case 0x0088: return "WM_SYNCPAINT";
	case 0x0090: return "WM_UAHDESTROYWINDOW";
	case 0x0091: return "WM_UAHDRAWMENU";
	case 0x0092: return "WM_UAHDRAWMENUITEM";
	case 0x0093: return "WM_UAHINITMENU";
	case 0x0094: return "WM_UAHMEASUREMENUITEM";
	case 0x0095: return "WM_UAHNCPAINTMENUPOPUP";
	case 0x0096: return "WM_UAHUPDATE";
	case 0x00A0: return "WM_NCMOUSEMOVE";
	case 0x00A1: return "WM_NCLBUTTONDOWN";
	case 0x00A2: return "WM_NCLBUTTONUP";
	case 0x00A3: return "WM_NCLBUTTONDBLCLK";
	case 0x00A4: return "WM_NCRBUTTONDOWN";
	case 0x00A5: return "WM_NCRBUTTONUP";
	case 0x00A6: return "WM_NCRBUTTONDBLCLK";
	case 0x00A7: return "WM_NCMBUTTONDOWN";
	case 0x00A8: return "WM_NCMBUTTONUP";
	case 0x00A9: return "WM_NCMBUTTONDBLCLK";
	case 0x00AB: return "WM_NCXBUTTONDOWN";
	case 0x00AC: return "WM_NCXBUTTONUP";
	case 0x00AD: return "WM_NCXBUTTONDBLCLK";
	case 0x00FE: return "WM_INPUT_DEVICE_CHANGE";
	case 0x00FF: return "WM_INPUT";
	case 0x0100: return "WM_KEYDOWN";
	case 0x0101: return "WM_KEYUP";
	case 0x0102: return "WM_CHAR";
	case 0x0103: return "WM_DEADCHAR";
	case 0x0104: return "WM_SYSKEYDOWN";
	case 0x0105: return "WM_SYSKEYUP";
	case 0x0106: return "WM_SYSCHAR";
	case 0x0107: return "WM_SYSDEADCHAR";
	case 0x0109: return "WM_UNICHAR";
	case 0x010D: return "WM_IME_STARTCOMPOSITION";
	case 0x010E: return "WM_IME_ENDCOMPOSITION";
	case 0x010F: return "WM_IME_COMPOSITION";
	case 0x0110: return "WM_INITDIALOG";
	case 0x0111: return "WM_COMMAND";
	case 0x0112: return "WM_SYSCOMMAND";
	case 0x0113: return "WM_TIMER";
	case 0x0114: return "WM_HSCROLL";
	case 0x0115: return "WM_VSCROLL";
	case 0x0116: return "WM_INITMENU";
	case 0x0117: return "WM_INITMENUPOPUP";
	case 0x0119: return "WM_GESTURE";
	case 0x011A: return "WM_GESTURENOTIFY";
	case 0x011F: return "WM_MENUSELECT";
	case 0x0120: return "WM_MENUCHAR";
	case 0x0121: return "WM_ENTERIDLE";
	case 0x0122: return "WM_MENURBUTTONUP";
	case 0x0123: return "WM_MENUDRAG";
	case 0x0124: return "WM_MENUGETOBJECT";
	case 0x0125: return "WM_UNINITMENUPOPUP";
	case 0x0126: return "WM_MENUCOMMAND";
	case 0x0127: return "WM_CHANGEUISTATE";
	case 0x0128: return "WM_UPDATEUISTATE";
	case 0x0129: return "WM_QUERYUISTATE";
	case 0x0132: return "WM_CTLCOLORMSGBOX";
	case 0x0133: return "WM_CTLCOLOREDIT";
	case 0x0134: return "WM_CTLCOLORLISTBOX";
	case 0x0135: return "WM_CTLCOLORBTN";
	case 0x0136: return "WM_CTLCOLORDLG";
	case 0x0137: return "WM_CTLCOLORSCROLLBAR";
	case 0x0138: return "WM_CTLCOLORSTATIC";
	case 0x0200: return "WM_MOUSEMOVE";
	case 0x0201: return "WM_LBUTTONDOWN";
	case 0x0202: return "WM_LBUTTONUP";
	case 0x0203: return "WM_LBUTTONDBLCLK";
	case 0x0204: return "WM_RBUTTONDOWN";
	case 0x0205: return "WM_RBUTTONUP";
	case 0x0206: return "WM_RBUTTONDBLCLK";
	case 0x0207: return "WM_MBUTTONDOWN";
	case 0x0208: return "WM_MBUTTONUP";
	case 0x0209: return "WM_MBUTTONDBLCLK";
	case 0x020A: return "WM_MOUSEWHEEL";
	case 0x020B: return "WM_XBUTTONDOWN";
	case 0x020C: return "WM_XBUTTONUP";
	case 0x020D: return "WM_XBUTTONDBLCLK";
	case 0x020E: return "WM_MOUSEHWHEEL";
	case 0x0210: return "WM_PARENTNOTIFY";
	case 0x0211: return "WM_ENTERMENULOOP";
	case 0x0212: return "WM_EXITMENULOOP";
	case 0x0213: return "WM_NEXTMENU";
	case 0x0214: return "WM_SIZING";
	case 0x0215: return "WM_CAPTURECHANGED";
	case 0x0216: return "WM_MOVING";
	case 0x0218: return "WM_POWERBROADCAST";
	case 0x0219: return "WM_DEVICECHANGE";
	case 0x0220: return "WM_MDICREATE";
	case 0x0221: return "WM_MDIDESTROY";
	case 0x0222: return "WM_MDIACTIVATE";
	case 0x0223: return "WM_MDIRESTORE";
	case 0x0224: return "WM_MDINEXT";
	case 0x0225: return "WM_MDIMAXIMIZE";
	case 0x0226: return "WM_MDITILE";
	case 0x0227: return "WM_MDICASCADE";
	case 0x0228: return "WM_MDIICONARRANGE";
	case 0x0229: return "WM_MDIGETACTIVE";
	case 0x0230: return "WM_MDISETMENU";
	case 0x0231: return "WM_ENTERSIZEMOVE";
	case 0x0232: return "WM_EXITSIZEMOVE";
	case 0x0233: return "WM_DROPFILES";
	case 0x0234: return "WM_MDIREFRESHMENU";
	case 0x0238: return "WM_POINTERDEVICECHANGE";
	case 0x0239: return "WM_POINTERDEVICEINRANGE";
	case 0x023A: return "WM_POINTERDEVICEOUTOFRANGE";
	case 0x0240: return "WM_TOUCH";
	case 0x0241: return "WM_NCPOINTERUPDATE";
	case 0x0242: return "WM_NCPOINTERDOWN";
	case 0x0243: return "WM_NCPOINTERUP";
	case 0x0245: return "WM_POINTERUPDATE";
	case 0x0246: return "WM_POINTERDOWN";
	case 0x0247: return "WM_POINTERUP";
	case 0x0249: return "WM_POINTERENTER";
	case 0x024A: return "WM_POINTERLEAVE";
	case 0x024B: return "WM_POINTERACTIVATE";
	case 0x024C: return "WM_POINTERCAPTURECHANGED";
	case 0x024D: return "WM_TOUCHHITTESTING";
	case 0x024E: return "WM_POINTERWHEEL";
	case 0x024F: return "WM_POINTERHWHEEL";
	case 0x0281: return "WM_IME_SETCONTEXT";
	case 0x0282: return "WM_IME_NOTIFY";
	case 0x0283: return "WM_IME_CONTROL";
	case 0x0284: return "WM_IME_COMPOSITIONFULL";
	case 0x0285: return "WM_IME_SELECT";
	case 0x0286: return "WM_IME_CHAR";
	case 0x0288: return "WM_IME_REQUEST";
	case 0x0290: return "WM_IME_KEYDOWN";
	case 0x0291: return "WM_IME_KEYUP";
	case 0x02A1: return "WM_MOUSEHOVER";
	case 0x02A3: return "WM_MOUSELEAVE";
	case 0x02A0: return "WM_NCMOUSEHOVER";
	case 0x02A2: return "WM_NCMOUSELEAVE";
	case 0x02B1: return "WM_WTSSESSION_CHANGE";
	case 0x02c0: return "WM_TABLET_FIRST";
	case 0x02df: return "WM_TABLET_LAST";
	case 0x02E0: return "WM_DPICHANGED";
	case 0x0300: return "WM_CUT";
	case 0x0301: return "WM_COPY";
	case 0x0302: return "WM_PASTE";
	case 0x0303: return "WM_CLEAR";
	case 0x0304: return "WM_UNDO";
	case 0x0305: return "WM_RENDERFORMAT";
	case 0x0306: return "WM_RENDERALLFORMATS";
	case 0x0307: return "WM_DESTROYCLIPBOARD";
	case 0x0308: return "WM_DRAWCLIPBOARD";
	case 0x0309: return "WM_PAINTCLIPBOARD";
	case 0x030A: return "WM_VSCROLLCLIPBOARD";
	case 0x030B: return "WM_SIZECLIPBOARD";
	case 0x030C: return "WM_ASKCBFORMATNAME";
	case 0x030D: return "WM_CHANGECBCHAIN";
	case 0x030E: return "WM_HSCROLLCLIPBOARD";
	case 0x030F: return "WM_QUERYNEWPALETTE";
	case 0x0310: return "WM_PALETTEISCHANGING";
	case 0x0311: return "WM_PALETTECHANGED";
	case 0x0312: return "WM_HOTKEY";
	case 0x0317: return "WM_PRINT";
	case 0x0318: return "WM_PRINTCLIENT";
	case 0x0319: return "WM_APPCOMMAND";
	case 0x031A: return "WM_THEMECHANGED";
	case 0x031D: return "WM_CLIPBOARDUPDATE";
	case 0x031E: return "WM_DWMCOMPOSITIONCHANGED";
	case 0x031F: return "WM_DWMNCRENDERINGCHANGED";
	case 0x0320: return "WM_DWMCOLORIZATIONCOLORCHANGED";
	case 0x0321: return "WM_DWMWINDOWMAXIMIZEDCHANGE";
	case 0x0323: return "WM_DWMSENDICONICTHUMBNAIL";
	case 0x0326: return "WM_DWMSENDICONICLIVEPREVIEWBITMAP";
	case 0x033F: return "WM_GETTITLEBARINFOEX";
	case 0x0358: return "WM_HANDHELDFIRST";
	case 0x035F: return "WM_HANDHELDLAST";
	case 0x0360: return "WM_AFXFIRST";
	case 0x037F: return "WM_AFXLAST";
	case 0x0380: return "WM_PENWINFIRST";
	case 0x038F: return "WM_PENWINLAST";
	case 0x8000: return "WM_APP";
	case 0x0400: return "WM_USER";
	}
	return "";
}

bool consoleExists = false;


void EnsureConsoleExists()
{
	if (!consoleExists)
	{
		BOOL attachThisOkay = AttachConsole(GetCurrentProcessId());
		BOOL attachParentOkay = AttachConsole(ATTACH_PARENT_PROCESS);
		if (!attachThisOkay && !attachParentOkay)
		{
			AllocConsole();
		}
		consoleExists = true;
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
	}
}

void LogWindowsMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//filter WM_TIMER, WM_NCHITTEST, WM_NCMOUSEMOVE
	if (uMsg == WM_TIMER ||
		uMsg == WM_NCHITTEST ||
		uMsg == WM_NCMOUSEMOVE ||
		uMsg == WM_MOUSEMOVE ||
		uMsg == WM_SETCURSOR)
	{
		return;
	}

	EnsureConsoleExists();
	const char* msgName = GetWindowsMessageName(uMsg);

	char buffer[32];
	time_t timer;
	timer = time(NULL);
	struct tm* tm_info = localtime(&timer);
	strftime(buffer, 32, "%H:%M:%S", tm_info);


	printf("%s - hwnd=0x%08x, msg=0x%04x (%s), wParam=0x%08x, lParam=0x%08x\n", buffer, hwnd, uMsg, msgName, wParam, lParam);
}

#endif
