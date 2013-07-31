#include "../NativeString.h"
#include <iostream>
#include <sstream>
#include <windows.h>

namespace die {
    
NativeString::Encoding const NativeString::nativeEncoding = NativeString::utf16;

std::wstring utf8_to_ws(char const * text, int size)
{
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    blah = converter.from_bytes(text);
    int total = MultiByteToWideChar(CP_UTF8,0,text,size,0,0);
    std::wstring converted(total,L'\0');
    if( MultiByteToWideChar(CP_UTF8,0,text,size,&converted[0],total) == 0 ) {
        std::cerr << "error converting string" << std::endl;
    }
    return converted;
}

std::wstring utf8_to_ws(char const * text)
{
    auto converted = utf8_to_ws(text,-1);
    converted.pop_back(); // remove the null terminated char
    return converted;
}

std::wstring utf8_to_ws(std::string const & text)
{
    if( text.empty() ) return std::wstring();
    return utf8_to_ws(text.c_str(),text.size());
}

std::wstring encodedToWs(NativeString::Encoding encoding, std::string const & strEncoded)
{
    switch(encoding) {
        case NativeString::utf16:
            return std::wstring(reinterpret_cast<wchar_t const *>(strEncoded.c_str()));
        case NativeString::utf8:
            return utf8_to_ws(strEncoded);
        default:
            std::cerr << "unsupported encoding" << std::endl;
            return std::wstring();
    }
}

NativeString::NativeString():
    wstr()
{}

NativeString::~NativeString()
{
    using std::wstring;
    wstr.~wstring();
}

NativeString::NativeString(NativeString const & other):
    wstr(other.wstr)
{}

NativeString & NativeString::operator=(NativeString const & other)
{
    wstr = other.wstr;
    return *this;
}

NativeString::NativeString(std::string const & strUTF8):
    wstr(utf8_to_ws(strUTF8))
{}

NativeString::NativeString(char const * strUTF8):
    wstr(utf8_to_ws(strUTF8))
{}

NativeString::NativeString(NativeString::Encoding encoding, std::string const & strEncoded):
    wstr(encodedToWs(encoding,strEncoded))
{}

std::string NativeString::toUTF8() const
{
    int total = WideCharToMultiByte(CP_UTF8,0,wstr.c_str(),wstr.size(),0,0,0,0);
    std::string converted(total,'\0');
    WideCharToMultiByte(CP_UTF8,0,wstr.c_str(),wstr.size(),&converted[0],total,0,0);
    return converted;    
}

NativeString::NativeString(std::wstring const & strUTF16):
    wstr(strUTF16)
{}

NativeString::NativeString(wchar_t const * strUTF16):
    wstr(strUTF16)
{}

std::wstring NativeString::toUTF16() const
{
    return wstr;
}

template<typename T>
T lexical_cast(std::wstring const & wstr)
{
    T result = T();
    std::basic_istringstream<wchar_t> iss(wstr);
    iss >> result;
    return result;
}

int NativeString::toInt() const
{
    return lexical_cast<int>(wstr);
}

bool NativeString::empty() const
{
    return wstr.empty();
}

NativeString & NativeString::operator+=(NativeString const & other)
{
    wstr += other.wstr;
    return *this;
}

bool NativeString::operator==(NativeString const & other) const
{
    return wstr == other.wstr;
}

bool NativeString::operator!=(NativeString const & other) const
{
    return wstr != other.wstr;
}

}
