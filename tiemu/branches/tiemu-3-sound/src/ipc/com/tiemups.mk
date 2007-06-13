
tiemups.dll: dlldata.obj oleaut_p.obj oleaut_i.obj
	link /dll /out:tiemups.dll /def:tiemups.def /entry:DllMain dlldata.obj oleaut_p.obj oleaut_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		-Dtiemu_ProxyFileInfo=oleaut_ProxyFileInfo \
		$<

clean:
	@del tiemups.dll
	@del tiemups.lib
	@del tiemups.exp
	@del dlldata.obj
	@del oleaut_p.obj
	@del oleaut_i.obj
