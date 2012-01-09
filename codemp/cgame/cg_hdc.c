#include "cg_local.h"

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>


/*void fixAccumBuffer()
{
	HDC hdc;
	HGLRC glrc;
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormat;
	static qboolean done=qfalse;
	if( !done )
	{
		hdc = wglGetCurrentDC();
		iPixelFormat = GetPixelFormat( hdc );
		if( !DescribePixelFormat(hdc, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) )
			CG_Printf("uhoh\n");
		pfd.cAccumRedBits = pfd.cAccumGreenBits = pfd.cAccumBlueBits = pfd.cAccumAlphaBits = 24;
		pfd.cAccumBits = 24 * 4;
		iPixelFormat = ChoosePixelFormat(hdc, &pfd);
		if( !iPixelFormat )
			CG_Printf("no ipixelformat dood\n");
		if( !DescribePixelFormat(hdc, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) )
			CG_Printf("uhoh\n");
		if( !pfd.cAccumBits )
			CG_Printf("dood u gay :/\n");
		CG_Printf("%d\n",pfd.cAccumBits);
		wglDeleteContext( wglGetCurrentContext() );
		if( SetPixelFormat(hdc, iPixelFormat, &pfd) )
			CG_Printf("all gud :sweat\n");
		else
			CG_Printf("u fale\n");
		if( glrc = wglCreateContext( hdc ) )
			CG_Printf( "tru\n" );
		else
			CG_Printf( "false\n" );
		if( wglMakeCurrent( hdc, glrc ) )
			CG_Printf ("yay\n");
		else
			CG_Printf("aww\n");
		
		LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;

    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)"sup")+40)*sizeof(TCHAR)); 
    wsprintf((LPTSTR)lpDisplayBuf, 
        TEXT("%s failed with error %d: %s"), 
        "sup", dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    
	}
	done = qtrue;
}*/
