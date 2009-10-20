////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_UNICODE_HPP
#define SFML_UNICODE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <iterator>
#include <locale>
#include <string>
#include <stdlib.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Provides utility functions to convert from and to
/// any unicode and ASCII encoding
////////////////////////////////////////////////////////////
class SFML_API Unicode
{
public :

    ////////////////////////////////////////////////////////////
    /// Define a string type for each encoding
    /// Warning : in UTF8 and UTF16 strings, one element doesn't
    /// necessarily maps to one character ; only an UTF32 element
    /// is wide enough to hold all possible unicode values
    ////////////////////////////////////////////////////////////
    typedef std::basic_string<Uint8>  UTF8String;
    typedef std::basic_string<Uint16> UTF16String;
    typedef std::basic_string<Uint32> UTF32String;

    ////////////////////////////////////////////////////////////
    /// This class is an abstract definition of a unicode text,
    /// it can be converted from and to any kind of string
    /// and encoding
    ////////////////////////////////////////////////////////////
    class SFML_API Text
    {
    public :

        ////////////////////////////////////////////////////////////
        /// Default constructor (empty text)
        ///
        ////////////////////////////////////////////////////////////
        Text();

        ////////////////////////////////////////////////////////////
        /// Construct the unicode text from any type of string
        ///
        /// \param str : String to convert
        ///
        ////////////////////////////////////////////////////////////
        Text(const char*                 str);
        Text(const wchar_t*              str);
        Text(const Uint8*                str);
        Text(const Uint16*               str);
        Text(const Uint32*               str);
        Text(const std::string&          str);
        Text(const std::wstring&         str);
        Text(const Unicode::UTF8String&  str);
        Text(const Unicode::UTF16String& str);
        Text(const Unicode::UTF32String& str);

        ////////////////////////////////////////////////////////////
        /// Operator to cast the text to any type of string
        ///
        /// \return Converted string
        ///
        ////////////////////////////////////////////////////////////
        operator       std::string          () const;
        operator       std::wstring         () const;
        operator       Unicode::UTF8String  () const;
        operator       Unicode::UTF16String () const;
        operator const Unicode::UTF32String&() const;

    private :

        ////////////////////////////////////////////////////////////
        // Data member
        ////////////////////////////////////////////////////////////
        sf::Unicode::UTF32String myUTF32String; ///< UTF-32 unicode text
    };

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-32 characters range
    /// to an ANSI characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    /// \param locale :      Locale to use for conversion (uses the current one by default)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF32ToANSI(In begin, In end, Out output, char replacement = '?', const std::locale& locale = GetDefaultLocale());

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an ANSI characters range
    /// to an UTF-32 characters range, using the given locale
    ///
    /// \param begin :  Iterator pointing to the beginning of the input sequence
    /// \param end :    Iterator pointing to the end of the input sequence
    /// \param output : Iterator pointing to the beginning of the output sequence
    /// \param locale : Locale to use for conversion (uses the current one by default)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out ANSIToUTF32(In begin, In end, Out output, const std::locale& locale = GetDefaultLocale());

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-8 characters range
    /// to an UTF-16 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF8ToUTF16(In begin, In end, Out output, Uint16 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-8 characters range
    /// to an UTF-32 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF8ToUTF32(In begin, In end, Out output, Uint32 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-16 characters range
    /// to an UTF-8 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF16ToUTF8(In begin, In end, Out output, Uint8 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-16 characters range
    /// to an UTF-32 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF16ToUTF32(In begin, In end, Out output, Uint32 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-32 characters range
    /// to an UTF-8 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF32ToUTF8(In begin, In end, Out output, Uint8 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Generic function to convert an UTF-32 characters range
    /// to an UTF-16 characters range, using the given locale
    ///
    /// \param begin :       Iterator pointing to the beginning of the input sequence
    /// \param end :         Iterator pointing to the end of the input sequence
    /// \param output :      Iterator pointing to the beginning of the output sequence
    /// \param replacement : Replacement character for characters not convertible to output encoding ('?' by default -- use 0 to use no replacement character)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out UTF32ToUTF16(In begin, In end, Out output, Uint16 replacement = '?');

    ////////////////////////////////////////////////////////////
    /// Get the number of characters composing an UTF-8 string
    ///
    /// \param begin : Iterator pointing to the beginning of the input sequence
    /// \param end :   Iterator pointing to the end of the input sequence
    ///
    /// \return Count of the characters in the string
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t GetUTF8Length(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// Get the number of characters composing an UTF-16 string
    ///
    /// \param begin : Iterator pointing to the beginning of the input sequence
    /// \param end :   Iterator pointing to the end of the input sequence
    ///
    /// \return Count of the characters in the string
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t GetUTF16Length(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// Get the number of characters composing an UTF-32 string
    ///
    /// \param begin : Iterator pointing to the beginning of the input sequence
    /// \param end :   Iterator pointing to the end of the input sequence
    ///
    /// \return Count of the characters in the string
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t GetUTF32Length(In begin, In end);

private :

    ////////////////////////////////////////////////////////////
    /// Get the default system locale
    ///
    /// \return Reference to the default system locale
    ///
    ////////////////////////////////////////////////////////////
    static const std::locale& GetDefaultLocale();

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const int    UTF8TrailingBytes[256]; ///< Lookup table to find the length of an UTF-8 sequence
    static const Uint32 UTF8Offsets[6];         ///< Magic values to subtract during UTF-8 conversions
    static const Uint8  UTF8FirstBytes[7];      ///< First bytes for UTF-8 sequences
};

#include <SFML/System/Unicode.inl>

} // namespace sf


#endif // SFML_UNICODE_HPP
