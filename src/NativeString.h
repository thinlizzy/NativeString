#ifndef NATIVE_STRING_H_32139idfjksfdjfsd
#define NATIVE_STRING_H_32139idfjksfdjfsd

#include <string>
#include <ostream>

namespace die {

class NativeString {
public:
    enum Encoding { utf8=0, utf16=1, utf32=2, };      // add more encodings according to the supported implementations

    union { 
        std::string str;
        std::wstring wstr;
    };
    
    NativeString();
    ~NativeString();
    NativeString(NativeString const & other);
    NativeString & operator=(NativeString const & other);
    NativeString(std::string const & strUTF8);
    NativeString(std::wstring const & strUTF16);
    NativeString(char const * strUTF8);
    NativeString(wchar_t const * strUTF16);
    NativeString(Encoding encoding, std::string const & strEncoded);
    
    std::string toUTF8() const;
    std::wstring toUTF16() const;
    
    bool empty() const;
    NativeString & operator+=(NativeString const & other);
    bool operator==(NativeString const & other) const;
    bool operator!=(NativeString const & other) const;
    
    static Encoding const nativeEncoding;
};

inline NativeString operator+(NativeString a, NativeString const & b) { return a+=b; }

inline std::ostream & operator<<(std::ostream & os, NativeString const & str) { return os << str.toUTF8(); }
inline std::wostream & operator<<(std::wostream & os, NativeString const & str) { return os << str.toUTF16(); }

}

#endif
