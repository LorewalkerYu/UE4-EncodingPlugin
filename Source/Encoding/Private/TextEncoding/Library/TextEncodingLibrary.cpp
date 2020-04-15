// Fill out your copyright notice in the Description page of Project Settings.
#include "TextEncoding/Library/TextEncodingLibrary.h"

#if PLATFORM_WINDOWS

#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include <string>
void UTF_8ToUnicode(wchar_t* pOut, char* pText)
{
	char* uchar = (char*)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}

void UnicodeToUTF_8(char* pOut, wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后 
	char* pchar = (char*)pText;
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
}

void UnicodeToGB2312(char* pOut, wchar_t uData)
{
	WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(wchar_t), NULL, NULL);
}

void Gb2312ToUnicode(wchar_t* pOut, char* gbBuffer)
{
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
}

void GB2312ToUTF_8(std::string& pOut, char* pText, int pLen)
{
	char buf[4] = { 0 };
	int nLength = pLen * 3;
	char* rst = new char[nLength];
	memset(rst, 0, nLength);
	int i = 0, j = 0;
	while (i < pLen)
	{
		//如果是英文直接复制就可以 
		if (*(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			Gb2312ToUnicode(&pbuffer, pText + i);
			UnicodeToUTF_8(buf, &pbuffer);
			rst[j] = buf[0];
			rst[j + 1] = buf[1];
			rst[j + 2] = buf[2];
			j += 3;
			i += 2;
		}
	}

	rst[j] = '\n';  	//返回结果  
	pOut = rst;
	delete[]rst;
	return;
}

void UTF_8ToGB2312(char* pOut, char* pText, int pLen)
{
	char Ctemp[4];
	memset(Ctemp, 0, 4);
	int i = 0, j = 0;
	while (i < pLen)
	{
		if (pText[i] >= 0)
		{
			pOut[j++] = pText[i++];
		}
		else
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp, pText + i);
			UnicodeToGB2312(Ctemp, Wtemp);
			pOut[j] = Ctemp[0];
			pOut[j + 1] = Ctemp[1];
			i += 3;
			j += 2;
		}
	}
	pOut[j] = '\n';
	return;
}
#endif //  PLATFORM_WINDOWS

//FString UTextEncodingLibrary::GB2312ToUtf8(const FString& GB2312Str)
//{
//	std::string Utf8Str;
//	GB2312Str.GetCharArray();
//#if PLATFORM_WINDOWS
//	GB2312ToUTF_8(Utf8Str, TCHAR_TO_ANSI(*GB2312Str), GB2312Str.Len());
//#endif
//	return FString(Utf8Str.c_str());
//}

FString UTextEncodingLibrary::GB2312ToUtf8(const FString& GB2312Str)
{
	std::string str;
	GB2312ToUTF_8(str, TCHAR_TO_UTF8(*GB2312Str), GB2312Str.Len());
	return FString(str.c_str());
}

FString UTextEncodingLibrary::GB2312BytesToUtf8String(const TArray<uint8>& GB2312Bytes)
{
	std::string str;
	const char* cpcData = reinterpret_cast<const char*>(GB2312Bytes.GetData());

	char* pc = new char[1024];
	strcpy(pc, cpcData);
	GB2312ToUTF_8(str, pc, GB2312Bytes.Num());
	return FString(UTF8_TO_TCHAR(str.c_str()));
}
