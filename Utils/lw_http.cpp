#include "lw_http.hpp"
#include "xor.h"
#include <iostream>

std::string lw_http_tools::url_encode(std::string const& s_input)
{
	std::string s_result;
	s_result.reserve(s_input.length());

	for (size_t I = 0; I < s_input.length(); ++I)
	{
		char ch = s_input[I];

		if ((ch < 33) || (ch > 126) || strchr(xorstr("!\"#%&'*,:;<=>?[]^`{|} "), ch))
		{
			char sz_buffer[4];
			sprintf_s(sz_buffer, xorstr("%%%02x"), ch & 0xFF);
			s_result += sz_buffer;
		}
		else
			s_result += ch;
	}

	return s_result;
}

std::string lw_http_tools::url_decode(std::string const& s_input)
{
	std::string s_result;
	s_result.reserve(s_input.length());

	char szBuffer[4];
	szBuffer[2] = '\0';

	const char* psz_input = s_input.c_str();

	while (*psz_input)
	{
		if (*psz_input == '%' && psz_input[1] && psz_input[2])
		{
			szBuffer[0] = psz_input[1];
			szBuffer[1] = psz_input[2];
			s_result += (char)(strtoul(szBuffer, NULL, 16));
			psz_input += 3;
		}
		else
		{
			s_result += *psz_input;
			++psz_input;
		}
	}

	return s_result;
}

void c_lw_httpd::fmt_out(const PCHAR pszFieldName, const PCHAR pszFmt, ...)
{
	static char sz_value[2048];
	ZeroMemory(sz_value, sizeof(sz_value));

	va_list VAList;
	va_start(VAList, pszFmt);
	_vsnprintf_s(sz_value, sizeof(sz_value), pszFmt, VAList);
	va_end(VAList);

	std::string sValueEncoded = lw_http_tools::url_encode(sz_value);

	static char szOut[2048];
	ZeroMemory(szOut, sizeof(szOut));
	//sprintf_s( szOut, "&%s=%s", pszFieldName, szValue );
	sprintf_s(szOut, (xorstr("&%s=%s")), pszFieldName, sValueEncoded.c_str());

	m_s_data_ += szOut;
}

void c_lw_httpd::add_field(const PCHAR pszName, const char* pszValue)
{
	fmt_out(pszName, (PCHAR)xorstr("%s"), pszValue);
}

const char* c_lw_httpd::get_data(void) const
{
	return &(m_s_data_.data()[1]);
}

DWORD c_lw_httpd::get_size(void) const
{
	return m_s_data_.length() - 1;
}

void c_lw_httpd::clear(void)
{
	m_s_data_.clear();
}

c_lw_http::c_lw_http(void) : m_dw_last_reply_size_(0)
{
	m_h_session_ = nullptr;
	m_psz_referer_ = (PWCHAR)L"";
}

c_lw_http::~c_lw_http(void)
{
}

void c_lw_http::parse_url_a(std::wstring& s_url, std::wstring& s_srv, std::wstring& s_obj, INTERNET_PORT& w_port)
{
	s_srv = s_url;

	size_t uSrv = s_srv.find(xorstr(L"://"));
	if (uSrv != -1)
		s_srv.erase(0, uSrv + 3);

	s_obj = s_srv;

	uSrv = s_srv.find(L"/");
	if (uSrv != -1)
		s_srv.erase(uSrv);

	size_t uObj = s_obj.find(xorstr(L"/"));
	if (uObj != -1)
		s_obj.erase(0, uObj);
	else
		s_obj = L"/";

	if (s_url.find(xorstr(L"https://")) == -1)
		w_port = INTERNET_DEFAULT_PORT;
	else
		w_port = INTERNET_DEFAULT_HTTPS_PORT;
}

bool c_lw_http::send_request(std::wstring s_url, std::vector<BYTE>& bt_reply, const PWCHAR psz_type, const LPVOID p_data, const DWORD dw_data_len)
{
	bool b_result = false;

	if (!m_h_session_) return b_result;

	INTERNET_PORT w_srv_port = 0;
	std::wstring s_server, s_object;
	parse_url_a(s_url, s_server, s_object, w_srv_port);

	const HINTERNET h_connect = LI_FN(WinHttpConnect)((HINTERNET)m_h_session_, (LPCWSTR)s_server.c_str(), (INTERNET_PORT)w_srv_port, (DWORD)0);
	if (!h_connect) return b_result;

	LPCWSTR psz_accept_types[2];
	psz_accept_types[0] = xorstr(L"*/*");
	psz_accept_types[1] = NULL;

	const HINTERNET h_request = LI_FN(WinHttpOpenRequest)(h_connect, psz_type,
		s_object.c_str(), xorstr(L"HTTP/1.1"), m_psz_referer_, psz_accept_types,
		w_srv_port == INTERNET_DEFAULT_HTTPS_PORT ? WINHTTP_FLAG_SECURE : 0);
	if (!h_request) return b_result;

	DWORD dwOptionValue = WINHTTP_DISABLE_REDIRECTS;
	LI_FN(WinHttpSetOption)(h_request, WINHTTP_OPTION_DISABLE_FEATURE, &dwOptionValue, sizeof(dwOptionValue));

	// Custom Header: Content-Type
	BOOL b_http_result = LI_FN(WinHttpAddRequestHeaders)(h_request,
		LWHTTP_CONT_TYPE, -1, WINHTTP_ADDREQ_FLAG_ADD);
	if (!b_http_result) goto CleanUp;

	b_http_result = LI_FN(WinHttpSendRequest)((HINTERNET)h_request,
		(LPCWSTR)WINHTTP_NO_ADDITIONAL_HEADERS, (DWORD)0, (LPVOID)p_data, (DWORD)dw_data_len, (DWORD)dw_data_len, (DWORD_PTR)NULL);
	if (!b_http_result) goto CleanUp;

	b_http_result = LI_FN(WinHttpReceiveResponse)((HINTERNET)h_request, (LPVOID)NULL);
	if (!b_http_result) goto CleanUp;

	if ((m_dw_last_reply_size_ = read_req_reply(h_request, bt_reply)))
		b_result = true;

CleanUp:
	if (h_request)
		LI_FN(WinHttpCloseHandle)((HINTERNET)h_request);

	if (h_connect)
		LI_FN(WinHttpCloseHandle)((HINTERNET)h_connect);

	return b_result;
}

DWORD c_lw_http::read_req_reply(HINTERNET hRequest, std::vector<BYTE>& btReply)
{
	if (!hRequest) return -1;

	DWORD dw_bytes_read = 0,
		dw_bytes_total = 0,
		dw_bytes_available = 0;

	char* psz_tmp_buffer = NULL;

	do
	{
		dw_bytes_available = dw_bytes_read = 0;

		if (!LI_FN(WinHttpQueryDataAvailable)(hRequest, &dw_bytes_available) ||
			(dw_bytes_available <= 0))
			goto CleanUp;

		psz_tmp_buffer = (char*)(malloc(dw_bytes_available + 1));
		ZeroMemory(psz_tmp_buffer, dw_bytes_available + 1);

		if (!LI_FN(WinHttpReadData)(hRequest, psz_tmp_buffer, dw_bytes_available, &dw_bytes_read))
			goto CleanUp;

		btReply.insert(btReply.end(), (PBYTE)(psz_tmp_buffer),
			(PBYTE)((uintptr_t)(psz_tmp_buffer)+dw_bytes_available));
		dw_bytes_total += dw_bytes_read;
		free(psz_tmp_buffer);
		psz_tmp_buffer = NULL;
	} while (dw_bytes_available > 0);

CleanUp:

	if (psz_tmp_buffer)
		free(psz_tmp_buffer);

	return dw_bytes_total;
}

std::string c_lw_http::GetSignatures(const char* str)
{
	c_lw_http	lw_http;
	c_lw_httpd	lw_http_d;

	if (!lw_http.open_session())
	{
		MessageBoxA(0, xorstr("check fail! contact owners"), xorstr("http"), 0);
		system(xorstr("exit"));
		exit(0);
	}

	std::string s_reply;
	lw_http_d.add_field((PCHAR)xorstr("Sig"), str);
	const auto b_lw_http = lw_http.post(xorstr(L"https://xmanloaders.cloud/men.php"), s_reply, lw_http_d);

	s_reply = encoder::Decode(s_reply.c_str());

	lw_http.close_session();

	if (s_reply.size() == 0 || s_reply.size() == 1)
	{
		while (true)
		{
			Sleep(100);

			MessageBoxA(0, xorstr("External Error. Please try going to https://1.1.1.1 and using WARP and retrying. If the error remains please contact support at our discord."), 0, 0);
		}
	}

	return s_reply.c_str();
}

bool c_lw_http::open_session(void)
{
	if (m_h_session_) return false;

	m_h_session_ = WinHttpOpen((LPCWSTR)LWHTTP_USER_AGENT, (DWORD)WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		(LPCWSTR)WINHTTP_NO_PROXY_NAME, (LPCWSTR)WINHTTP_NO_PROXY_BYPASS, (DWORD)0);

	return (m_h_session_ ? true : false);
}

void c_lw_http::close_session(void) const
{
	if (m_h_session_)
		LI_FN(WinHttpCloseHandle)((HINTERNET)m_h_session_);
}

bool c_lw_http::set_referer(PWCHAR pszReferer)
{
	if (!pszReferer) return false;

	m_psz_referer_ = pszReferer;

	return true;
}

PWCHAR c_lw_http::get_referer(void) const
{
	return m_psz_referer_;
}

bool c_lw_http::post(std::wstring sURL, std::string& sReply, c_lw_httpd& PostData)
{
	std::vector< BYTE > btReply;

	bool bResult = send_request(sURL, btReply, (PWCHAR)xorstr(L"POST"), (LPVOID)(PostData.get_data()), PostData.get_size());

	sReply.clear();
	sReply = std::string(btReply.begin(), btReply.end());
	return bResult;
}

bool c_lw_http::post(std::wstring sURL, std::vector<BYTE>& btReply, c_lw_httpd& PostData)
{
	return send_request(sURL, btReply, (PWCHAR)xorstr(L"POST"),
		LPVOID(PostData.get_data()), PostData.get_size());
}