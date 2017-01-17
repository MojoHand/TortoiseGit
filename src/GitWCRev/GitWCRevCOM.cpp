// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2017 - TortoiseGit
// Copyright (C) 2007-2015 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#include "stdafx.h"
#include <objbase.h>
#include "GitWCRevCOM_h.h"
#include "GitWCRevCOM_i.c"
#include "GitWCRevCOM.h"

#include <tchar.h>
#include <shlwapi.h>
#include <Shellapi.h>
#include <comutil.h>

#include "Register.h"
#include "UnicodeUtils.h"
#include <atlbase.h>

STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();
static void AutomationMain();
static void RunOutprocServer();
static void ImplWinMain();

int APIENTRY _tWinMain(HINSTANCE /*hInstance*/,
					   HINSTANCE /*hPrevInstance*/,
					   LPTSTR    /*lpCmdLine*/,
					   int       /*nCmdShow*/)
{
	ImplWinMain();
	return 0;
}

static void ImplWinMain()
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (!argv)
		return;

	if ((argc >= 2) && (argc <= 5))
	{
		if (wcscmp(argv[1], L"/automation")==0)
			AutomationMain();
		else if (wcscmp(argv[1], L"unregserver")==0)
			DllUnregisterServer();
		else if (wcscmp(argv[1], L"regserver")==0)
			DllRegisterServer();
	}
	LocalFree(argv);
}

static void AutomationMain()
{
	// initialize the COM library
	HRESULT hr = ::CoInitialize(nullptr);
	if (FAILED(hr))
		return;

	git_libgit2_init();

	RunOutprocServer();

	git_libgit2_shutdown();

	::CoUninitialize();
}

// Count of locks to the server - one for each non-factory live object
// plus one for each call of IClassFactory::LockServer(true) not yet
// followed by IClassFactory::LockServer(false)
static long g_cServerLocks = 0;

static void LockServer(bool doLock)
{
	if (doLock)
	{
		InterlockedIncrement(&g_cServerLocks);
		return;
	}
	long newLockCount = InterlockedDecrement(&g_cServerLocks);
	if (newLockCount == 0)
		::PostMessage(nullptr, WM_QUIT, 0, 0);
}

//
// Constructor
//
GitWCRev::GitWCRev() :
	m_cRef(1),
	SubStat(),
	m_ptinfo(0)
{
	LockServer(true);
	LoadTypeInfo(&m_ptinfo, LIBID_LibGitWCRev, IID_IGitWCRev, 0);
}

//
// Destructor
//
GitWCRev::~GitWCRev()
{
	LockServer(false);
	if (m_ptinfo != 0)
		m_ptinfo->Release();
}

//
// IUnknown implementation
//
HRESULT __stdcall GitWCRev::QueryInterface(const IID& iid, void** ppv)
{
	if (!ppv)
		return E_POINTER;

	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IGitWCRev) || IsEqualIID(iid, IID_IDispatch))
		*ppv = static_cast<IGitWCRev*>(this) ;
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE ;
	}
	AddRef();
	return S_OK ;
}

ULONG __stdcall GitWCRev::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

ULONG __stdcall GitWCRev::Release()
{
	const LONG refCount = InterlockedDecrement(&m_cRef);
	if (refCount == 0)
		delete this;
	return refCount;
}

//HRESULT GitWCRev::GetWCInfoInternal(/*[in]*/ BSTR wcPath, /*[in]*/VARIANT_BOOL folders, /*[in]*/VARIANT_BOOL externals)
/*{
	SubStat.bFolders = folders;
	SubStat.bExternals = externals;

	// clear all possible previous data
	SubStat.MinRev = 0;
	SubStat.MaxRev = 0;
	SubStat.CmtRev = 0;
	SubStat.CmtDate = 0;
	SubStat.HasMods = FALSE;
	SubStat.HasUnversioned = FALSE;
	SubStat.bHexPlain = FALSE;
	SubStat.bHexX = FALSE;
	SubStat.bIsSvnItem = FALSE;
	SubStat.bIsExternalsNotFixed = FALSE;
	SubStat.bIsExternalMixed = FALSE;
	SubStat.bIsTagged = FALSE;
	SecureZeroMemory(SubStat.Url, sizeof(SubStat.Url));
	SecureZeroMemory(SubStat.RootUrl, sizeof(SubStat.RootUrl));
	SecureZeroMemory(SubStat.Author, sizeof(SubStat.Author));
	SecureZeroMemory(&SubStat.LockData, sizeof(SubStat.LockData));


	apr_pool_t * pool;
	apr_pool_create_ex (&pool, NULL, NULL, NULL);

	size_t ret = 0;
	getenv_s(&ret, NULL, 0, "SVN_ASP_DOT_NET_HACK");
	if (ret)
	{
		svn_wc_set_adm_dir("_svn", pool);
	}

	char *wc_utf8 = Utf16ToUtf8((WCHAR*)wcPath, pool);
	const char * internalpath = svn_path_internal_style (wc_utf8, pool);

	svn_client_ctx_t * ctx;
	svn_client_create_context2(&ctx, NULL, pool);

	svn_error_t * svnerr = nullptr;
	const char *wcroot;
	svnerr = svn_client_get_wc_root(&wcroot, internalpath, ctx, pool, pool);
	if ((svnerr == SVN_NO_ERROR) && wcroot)
		LoadIgnorePatterns(wcroot, &SubStat);
	svn_error_clear(svnerr);
	LoadIgnorePatterns(internalpath, &SubStat);

	svnerr = svn_status(  internalpath,   //path
		&SubStat,       //status_baton
		TRUE,           //noignore
		ctx,
		pool);

	HRESULT hr = S_OK;
	if (svnerr)
	{
		hr = S_FALSE;
		svn_error_clear(svnerr);
	}
	apr_pool_destroy(pool);
	return hr;
}*/

//
// IGitWCRev implementation
//
//HRESULT __stdcall GitWCRev::GetWCInfo2(/*[in]*/ BSTR wcPath, /*[in]*/VARIANT_BOOL folders, /*[in]*/VARIANT_BOOL externals, /*[in]*/VARIANT_BOOL externalsNoMixed)
/*{
	if (!wcPath)
		return E_INVALIDARG;

	SubStat.bExternalsNoMixedRevision = externalsNoMixed;
	return GetWCInfoInternal(wcPath, folders, externals);
}*/

//HRESULT __stdcall GitWCRev::GetWCInfo(/*[in]*/ BSTR wcPath, /*[in]*/VARIANT_BOOL folders, /*[in]*/VARIANT_BOOL externals)
/*{
	if (!wcPath)
		return E_INVALIDARG;

	return GetWCInfoInternal(wcPath, folders, externals);
}*/

HRESULT __stdcall GitWCRev::get_Revision(/*[out, retval]*/VARIANT* rev)
{
	return Utf8StringToVariant(SubStat.HeadHashReadable, rev);
}

HRESULT __stdcall GitWCRev::get_Date(/*[out, retval]*/VARIANT* date)
{
	if (!date)
		return E_POINTER;

	date->vt = VT_BSTR;

	WCHAR destbuf[32] = { 0 };
	HRESULT result = CopyDateToString(destbuf, _countof(destbuf), SubStat.HeadTime) ? S_OK : S_FALSE;
	if(S_FALSE == result)
		swprintf_s(destbuf, L"");

	date->bstrVal = SysAllocStringLen(destbuf, (UINT)wcslen(destbuf));
	return result;
}

HRESULT __stdcall GitWCRev::get_Author(/*[out, retval]*/VARIANT* author)
{
	return Utf8StringToVariant(SubStat.HeadAuthor.c_str(), author);
}

HRESULT GitWCRev::Utf8StringToVariant(const char* string, VARIANT* result )
{
	if (!result)
		return E_POINTER;

	result->vt = VT_BSTR;
	const size_t len = strlen(string);
	std::unique_ptr<WCHAR[]> buf(new WCHAR[len*4 + 1]);
	SecureZeroMemory(buf.get(), (len*4 + 1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, string, -1, buf.get(), (int)len*4);
	result->bstrVal = SysAllocString(buf.get());
	return S_OK;
}

HRESULT __stdcall GitWCRev::get_HasModifications(VARIANT_BOOL* modifications)
{
	return BoolToVariantBool(SubStat.HasMods, modifications);
}

HRESULT __stdcall GitWCRev::get_HasUnversioned(VARIANT_BOOL* modifications)
{
	return BoolToVariantBool(SubStat.HasUnversioned, modifications);
}

/*HRESULT __stdcall GitWCRev::get_HaveExternalsAllFixedRevision(VARIANT_BOOL* modifications)
{
	return BoolToVariantBool(!SubStat.bIsExternalsNotFixed, modifications);
}*/

HRESULT __stdcall GitWCRev::get_IsWcTagged(VARIANT_BOOL* modifications)
{
	return BoolToVariantBool(SubStat.bIsTagged, modifications);
}

HRESULT __stdcall GitWCRev::get_IsGitItem(/*[out, retval]*/VARIANT_BOOL* svn_item)
{
	return BoolToVariantBool(SubStat.bIsGitItem, svn_item);
}

HRESULT GitWCRev::BoolToVariantBool(BOOL value, VARIANT_BOOL* result)
{
	if(result == 0)
		return E_POINTER;
	*result = (value == 0) ? VARIANT_FALSE : VARIANT_TRUE;
	return S_OK;
}

HRESULT GitWCRev::LongToVariant(LONG value, VARIANT* result)
{
	if(result == 0)
		return E_POINTER;

	result->vt = VT_I4;
	result->lVal = value;
	return S_OK;
}

//
// Get a readable string from a apr_time_t date
//
BOOL GitWCRev::CopyDateToString(WCHAR *destbuf, int buflen, __time64_t ttime)
{
	const int min_buflen = 32;
	if (!destbuf || (min_buflen > buflen))
		return FALSE;

	struct tm newtime;
	if (_localtime64_s(&newtime, &ttime))
		return FALSE;
	// Format the date/time in international format as yyyy/mm/dd hh:mm:ss
	swprintf_s(destbuf, min_buflen, L"%04d/%02d/%02d %02d:%02d:%02d", newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec);
	return TRUE;
}

HRESULT GitWCRev::LoadTypeInfo(ITypeInfo ** pptinfo, const CLSID &libid, const CLSID &iid, LCID lcid)
{
	if (!pptinfo)
		return E_POINTER;
	*pptinfo = nullptr;

	// Load type library.
	CComPtr<ITypeLib> ptlib;
	HRESULT hr = LoadRegTypeLib(libid, 1, 0, lcid, &ptlib);
	if (FAILED(hr))
		return hr;

	// Get type information for interface of the object.
	LPTYPEINFO ptinfo = nullptr;
	hr = ptlib->GetTypeInfoOfGuid(iid, &ptinfo);
	if (FAILED(hr))
		return hr;

	*pptinfo = ptinfo;
	return S_OK;
}

HRESULT __stdcall GitWCRev::GetTypeInfoCount(UINT* pctinfo)
{
	if (!pctinfo)
		return E_POINTER;

	*pctinfo = 1;
	return S_OK;
}

HRESULT __stdcall GitWCRev::GetTypeInfo(UINT itinfo, LCID /*lcid*/, ITypeInfo** pptinfo)
{
	if (!pptinfo)
		return E_POINTER;

	*pptinfo = nullptr;

	if (itinfo)
		return ResultFromScode(DISP_E_BADINDEX);

	if (!m_ptinfo)
		return E_UNEXPECTED;

	m_ptinfo->AddRef();      // AddRef and return pointer to cached
	// typeinfo for this object.
	*pptinfo = m_ptinfo;

	return S_OK;
}

HRESULT __stdcall GitWCRev::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames,
											 LCID /*lcid*/, DISPID* rgdispid)
{
	return DispGetIDsOfNames(m_ptinfo, rgszNames, cNames, rgdispid);
}

HRESULT __stdcall GitWCRev::Invoke(DISPID dispidMember, REFIID /*riid*/,
									  LCID /*lcid*/, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
									  EXCEPINFO* pexcepinfo, UINT* puArgErr)
{
	return DispInvoke(
		this, m_ptinfo,
		dispidMember, wFlags, pdispparams,
		pvarResult, pexcepinfo, puArgErr);
}

//
// Class factory IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(const IID& iid, void** ppv)
{
	if (!ppv)
		return E_POINTER;

	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IClassFactory))
		*ppv = static_cast<IClassFactory*>(this) ;
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE ;
	}
	AddRef();
	return S_OK ;
}

ULONG __stdcall CFactory::AddRef()
{
	// No true reference counting in global object
	return 1;
}

ULONG __stdcall CFactory::Release()
{
	// No true reference counting in global object
	return 1;
}

//
// IClassFactory implementation
//
HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
										   const IID& iid,
										   void** ppv)
{
	if (!ppv)
		return E_POINTER;

	// Cannot aggregate.
	if (pUnknownOuter)
		return CLASS_E_NOAGGREGATION ;

	// Create component.
	ATL::CComPtr<GitWCRev> pA;
	pA.Attach(new (std::nothrow) GitWCRev());// refcount set to 1 in constructor
	if (!pA)
		return E_OUTOFMEMORY ;

	return pA->QueryInterface(iid, ppv);
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	::LockServer(bLock != 0);
	return S_OK ;
}

// Refcount set to 1 in constructor.
CFactory gClassFactory;


static void RunOutprocServer()
{
	// register ourself as a class object against the internal COM table
	DWORD nToken = 0;
	HRESULT hr = ::CoRegisterClassObject(CLSID_GitWCRev,
		&gClassFactory,
		CLSCTX_SERVER,
		REGCLS_MULTIPLEUSE,
		&nToken);

	// If the class factory is not registered no object will ever be instantiated,
	// hence no object will ever be released, hence WM_QUIT never appears in the
	// message queue and the message loop below will run forever.
	if (FAILED(hr))
		return;

	//
	// (loop ends if WM_QUIT message is received)
	//
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// unregister from the known table of class objects
	::CoRevokeClassObject(nToken);
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	const HMODULE hModule = ::GetModuleHandle(nullptr);

	HRESULT hr = RegisterServer(hModule,
		CLSID_GitWCRev,
		L"GitWCRev Server Object",
		L"GitWCRev.object",
		L"GitWCRev.object.1",
		LIBID_LibGitWCRev) ;
	if (SUCCEEDED(hr))
		RegisterTypeLib(hModule, nullptr);
	return hr;
}

//
// Server unregistration
//
STDAPI DllUnregisterServer()
{
	const HMODULE hModule = ::GetModuleHandle(nullptr);

	HRESULT hr = UnregisterServer(CLSID_GitWCRev,
		L"GitWCRev.object",
		L"GitWCRev.object.1",
		LIBID_LibGitWCRev) ;
	if (SUCCEEDED(hr))
		UnRegisterTypeLib(hModule, nullptr);
	return hr;
}
