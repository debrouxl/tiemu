#ifndef __TIEMUOLE_H__
#define __TIEMUOLE_H__

#include "oleaut.h"

class TiEmuOLE : public ITiEmuOLE
{
  private:
    ITypeLib *typelib;
    unsigned refcount;

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
};

#endif
