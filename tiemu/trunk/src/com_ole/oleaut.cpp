/*  OLE Automation interface for TiEmu
 *
 *  Copyright (c) 2006 Kevin Kofler
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <cstdlib>
#include <wchar.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <windows.h>
#include <oleauto.h>

#include "oleaut.h"
#include "oleaut_c.h"
#include "ti68k_int.h"
#include "fs_misc.h"
#include "engine.h"
#include "dbg_all.h"
#include "m68k.h"

class TiEmuOLE : public ITiEmuOLE
{
  private:
    ITypeLib *typelib;
    unsigned refcount;
    bool initialized;
    bool registered;
    DWORD oleregister;

  public:
    TiEmuOLE();
    ~TiEmuOLE();
    STDMETHOD(QueryInterface)(REFIID,PVOID*);
    STDMETHOD_(ULONG,AddRef)();
    STDMETHOD_(ULONG,Release)();
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT*);
    STDMETHOD(GetTypeInfo)(UINT,LCID,LPTYPEINFO*);
    STDMETHOD(GetIDsOfNames)(REFIID,LPOLESTR*,UINT,LCID,DISPID*);
    STDMETHOD(Invoke)(DISPID,REFIID,LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*);
    STDMETHOD(image_loaded)(VARIANT_BOOL* ret);
    STDMETHOD(emulated_calc_type)(int* ret);
    STDMETHOD(emulated_hw_version)(int* ret);
    STDMETHOD(emulated_os_version)(BSTR* ret);
    STDMETHOD(ready_for_transfers)(VARIANT_BOOL* ret);
    STDMETHOD(send_file)(BSTR filename,VARIANT_BOOL* ret);
    STDMETHOD(debug_file)(BSTR filename,VARIANT_BOOL* ret);
    STDMETHOD(reset_calc)(VARIANT_BOOL clearmem,VARIANT_BOOL* ret);
    STDMETHOD(execute_command)(BSTR command,VARIANT_BOOL* ret);
    STDMETHOD(turn_calc_on)(VARIANT_BOOL* ret);
    STDMETHOD(enter_debugger)(VARIANT_BOOL* ret);
};

TiEmuOLE::TiEmuOLE() : typelib(NULL), refcount(1)
{
  // Initialize OLE
  if (CoInitialize(NULL) == S_OK) {
    // Load type library
    LoadRegTypeLib(LIBID_TiEmuOLELib, 1, 0, 0, &typelib);
    if (typelib) {
      // Register with OLE Automation
      registered = (RegisterActiveObject(this, CLSID_TiEmuOLE, ACTIVEOBJECT_STRONG, &oleregister) == S_OK);
    }
    initialized = true;
  } else initialized = false;
}

TiEmuOLE::~TiEmuOLE()
{
  if (initialized) {
    if (registered) {
      RevokeActiveObject(oleregister, NULL);
      CoDisconnectObject(this, 0);
    }
    CoUninitialize();
  }
  if (typelib)
    typelib->Release();
}

#define RETURN(retval) do {*ret=retval; return S_OK;} while (0)

// IUnknown methods

STDMETHODIMP TiEmuOLE::QueryInterface(REFIID iid,PVOID *ret)
{
  if (iid==IID_IUnknown || iid==IID_IDispatch || iid == IID_ITiEmuOLE)
    RETURN(this);
  else
    return E_NOINTERFACE;
}

ULONG TiEmuOLE::AddRef()
{
  return ++refcount;
}

ULONG TiEmuOLE::Release()
{
  if (refcount)
    refcount--;
  return refcount;
}

// IDispatch methods

STDMETHODIMP TiEmuOLE::GetTypeInfoCount(UINT *ret)
{
  RETURN(1);
}

STDMETHODIMP TiEmuOLE::GetTypeInfo(UINT interfaceno, LCID lcid, LPTYPEINFO *ret)
{
  return typelib->GetTypeInfoOfGuid(IID_ITiEmuOLE,ret);
}

STDMETHODIMP TiEmuOLE::GetIDsOfNames(REFIID iid, LPOLESTR *names, UINT count, LCID lcid, DISPID* ret)
{
  ITypeInfo *typeinfo;
  HRESULT result;
  result=typelib->GetTypeInfoOfGuid(IID_ITiEmuOLE,&typeinfo);
  if (result) return result;
  result=DispGetIDsOfNames(typeinfo,names,count,ret);
  typeinfo->Release();
  return result;
}

STDMETHODIMP TiEmuOLE::Invoke(DISPID dispid, REFIID iid, LCID lcid, WORD flags, DISPPARAMS *params, VARIANT *ret, EXCEPINFO *excepinfo, UINT *error_argnum)
{
  ITypeInfo *typeinfo;
  HRESULT result;
  result=typelib->GetTypeInfoOfGuid(IID_ITiEmuOLE,&typeinfo);
  if (result) return result;
  result=DispInvoke(this,typeinfo,dispid,flags,params,ret,excepinfo,error_argnum);
  typeinfo->Release();
  return result;
}

// ITiEmuOLE methods

STDMETHODIMP TiEmuOLE::image_loaded(VARIANT_BOOL *ret)
{
  RETURN(img_loaded);
}

STDMETHODIMP TiEmuOLE::emulated_calc_type(int *ret)
{
  if (img_loaded)
    RETURN(img_infos.calc_type);
  else
    RETURN(0);
}

STDMETHODIMP TiEmuOLE::emulated_hw_version(int *ret)
{
  if (img_loaded)
    RETURN(img_infos.hw_type);
  else
    RETURN(0);
}

STDMETHODIMP TiEmuOLE::emulated_os_version(BSTR *ret)
{
  if (img_loaded) {
    wchar_t *temp=(wchar_t *)g_utf8_to_utf16(img_infos.version,-1,NULL,NULL,NULL);
    BSTR result=SysAllocString(temp);
    g_free(temp);
    RETURN(result);
  } else
    RETURN(NULL);
}

STDMETHODIMP TiEmuOLE::ready_for_transfers(VARIANT_BOOL *ret)
{
  RETURN((img_loaded && !engine_is_stopped()));
}

STDMETHODIMP TiEmuOLE::send_file(BSTR filename, VARIANT_BOOL *ret)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    gchar *utf8=g_utf16_to_utf8((const gunichar2 *)filename,-1,NULL,NULL,NULL);
    ::send_file(utf8);
    g_free(utf8);
    engine_start();
    RETURN(TRUE);
  } else RETURN(FALSE);
}

STDMETHODIMP TiEmuOLE::debug_file(BSTR filename, VARIANT_BOOL *ret)
{
#ifndef NO_GDB
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    gchar *utf8=g_utf16_to_utf8((const gunichar2 *)filename,-1,NULL,NULL,NULL);
    send_file_and_debug_info(utf8);
    g_free(utf8);
    engine_start();
    RETURN(TRUE);
  } else RETURN(FALSE);
#else
  RETURN(FALSE);
#endif
}

STDMETHODIMP TiEmuOLE::reset_calc(VARIANT_BOOL clearram, VARIANT_BOOL *ret)
{
  if (img_loaded) {
    engine_stop();

    if(clearram)
      memset(tihw.ram, 0, tihw.ram_size);

    ti68k_reset();
    if (dbg_on)
      close_debugger();
    else
      engine_start();

    RETURN(TRUE);
  } else RETURN(FALSE);
}

STDMETHODIMP TiEmuOLE::execute_command(BSTR command, VARIANT_BOOL *ret)
{
  if (img_loaded) {
    VARIANT_BOOL result;
    gchar *temp;
#ifdef __GNUC__
    char ti[SysStringLen(command)+1];
	ticonv_charset_utf16_to_ti_s(CALC_TI92, (const unsigned short *)command, ti);
    temp=g_strconcat("\f",ti,"\r",NULL);
    result=ti68k_kbd_push_chars(temp);
#elif defined(_MSC_VER)
	char *ti=(char *)malloc(SysStringLen(command)+1);
	ticonv_charset_utf16_to_ti_s(CALC_TI92, (const unsigned short *)command, ti);
    temp=g_strconcat("\f",ti,"\r",NULL);
    result=ti68k_kbd_push_chars(temp);
    free(ti);
#else
    char *ti=std::malloc(SysStringLen(command)+1);
    ticonv_charset_utf16_to_ti_s(CALC_TI92, (const unsigned short *)command, ti);
    temp=g_strconcat("\f",ti,"\r",NULL);
    result=ti68k_kbd_push_chars(temp);
    std::free(ti);
#endif
    g_free(temp);
    RETURN(result);
  } else RETURN(FALSE);
}

STDMETHODIMP TiEmuOLE::turn_calc_on(VARIANT_BOOL *ret)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    hw_m68k_irq(6);
    while (ti68k_debug_is_supervisor())
      hw_m68k_run(1,0);
    engine_start();
    RETURN(TRUE);
  } else RETURN(FALSE);
}

STDMETHODIMP TiEmuOLE::enter_debugger(VARIANT_BOOL *ret)
{
  if (img_loaded && !engine_is_stopped()) {
    engine_stop();
    ti68k_debug_break();
    engine_start();
    RETURN(TRUE);
  } else RETURN(FALSE);
}

static TiEmuOLE *ole_interface;

void oleaut_init (void)
{
//  ole_interface = new TiEmuOLE;
}

void oleaut_finish (void)
{
//  delete ole_interface;
}

