
AutoSvrps.dll: dlldata.obj AutoSvr_p.obj AutoSvr_i.obj
	link /dll /out:AutoSvrps.dll /def:AutoSvrps.def /entry:DllMain dlldata.obj AutoSvr_p.obj AutoSvr_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del AutoSvrps.dll
	@del AutoSvrps.lib
	@del AutoSvrps.exp
	@del dlldata.obj
	@del AutoSvr_p.obj
	@del AutoSvr_i.obj
