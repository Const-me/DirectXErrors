#include "stdafx.h"
#include <iostream>

#include <ddraw.h>
#include <d3d9.h>
#include <Mmreg.h>
#include <dsound.h>
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include <dinputd.h>
#include <d3d10_1.h>
#include <d3d11_1.h>
#include <wincodec.h>
#include <d2derr.h>
#include <dwrite.h>
#include <Mferror.h>
#include "DirectXErrors/codes.h"

#include "DirectXErrors/DirectXErrors.h"

inline const char* errorString( HRESULT hr )
{
	const char* const res = getDxErrorStringA( hr );
	if( nullptr != res )
		return res;
	return "<unknown code>";
}

int main()
{
#pragma warning (disable:4838)	// warning C4838: conversion from 'unsigned int' to 'const HRESULT' requires a narrowing conversion
	static const HRESULT codes[] = 
	{
		D2DERR_WRONG_STATE,	// D2D
		D3DERR_WRONGTEXTUREFORMAT,	// D3D9
		D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS,	// D3D10
		DDERR_ALREADYINITIALIZED,	// DDraw
		DIERR_INPUTLOST,	// DirectInput
		DS_NO_VIRTUALIZATION, // DirectSound
		DXUTERR_NODIRECT3D,	// DXUT
		WINCODEC_ERR_WRONGSTATE,	// WIC
		MF_E_NO_SAMPLE_TIMESTAMP,	// MF
		E_UNEXPECTED,	// Win32
		XAUDIO2_E_INVALID_CALL,	// Audio
		S_OK,
		0xCCCCCCCC,	// Completely broken one
	};

	for( HRESULT hr : codes )
	{
		std::cout << errorString( hr ) << "\t" << formatDxMessageA( hr ) << std::endl;
	}
	return 0;
}