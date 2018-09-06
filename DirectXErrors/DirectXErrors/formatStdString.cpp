#include "stdafx.h"
#include "DirectXErrors.h"
#include "formatMessage.hpp"

#if !defined DXERR_ATL_STRING && defined DXERR_STD_STRING
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>

namespace
{
	// https://stackoverflow.com/a/217605/126995
	template<class tString>
	inline void ltrim( tString &s )
	{
		s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( int ch ) { return !std::isspace( ch ); } ) );
	}
	template<class tString>
	inline void rtrim( tString &s )
	{
		s.erase( std::find_if( s.rbegin(), s.rend(), []( int ch ) { return !std::isspace( ch ); } ).base(), s.end() );
	}
	template<class tString>
	inline void trim( tString &s )
	{
		ltrim( s );
		rtrim( s );
	}

	// https://stackoverflow.com/a/18374698/126995
	std::wstring s2ws( const std::string& str )
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.from_bytes( str );
	}
}

std::string formatDxMessageA( HRESULT hr )
{
	// Custom messages
	const char* const description = getDxErrorDescriptionA( hr );
	if( nullptr != description )
		return description;

	// Windows messages
	char *lpMsgBuf;
	const DWORD fmLength = formatMessageA( hr, &lpMsgBuf );
	if( 0 != fmLength )
	{
		std::string message{ lpMsgBuf, lpMsgBuf + fmLength };
		LocalFree( lpMsgBuf );
		trim( message );
		return message;
	}

	// Fallback message
	std::stringstream ss;
	// https://stackoverflow.com/a/25144137/126995
	ss << "Unknown error code " << (int)hr << " (0x" << std::uppercase << std::setfill( '0' ) << std::setw( 8 ) << std::hex << (int)hr << ")";
	return ss.str();
}

std::wstring formatDxMessageW( HRESULT hr )
{
	// Custom messages
	const char* const description = getDxErrorDescriptionA( hr );	// Using the "A" version saves binary size
	if( nullptr != description )
		return s2ws( description );

	// Windows messages
	wchar_t *lpMsgBuf;
	const DWORD fmLength = formatMessageW( hr, &lpMsgBuf );
	if( 0 != fmLength )
	{
		std::wstring message{ lpMsgBuf, lpMsgBuf + fmLength };
		LocalFree( lpMsgBuf );
		trim( message );
		return message;
	}

	// Fallback message
	std::wstringstream ss;
	// https://stackoverflow.com/a/25144137/126995
	ss << L"Unknown error code " << (int)hr << L" (0x" << std::uppercase << std::setfill( L'0' ) << std::setw( 8 ) << std::hex << (int)hr << L")";
	return ss.str();
}
#endif