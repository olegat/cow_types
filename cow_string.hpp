/**
 * Copyright (c) 2021 Oli Legat
 * Licensed under the BSD 3-Clause License.
 */

#pragma once

#include <string>
#include <memory>

namespace cow {

//----------------------------------------------------------------------------
// Template declaration : Copy-On-Write (COW) Basic String
//----------------------------------------------------------------------------
template < class charT,
           class traits = std::char_traits<charT>,    // std::basic_string::traits_type
           class Alloc = std::allocator<charT>        // std::basic_string::allocator_type
           >
class basic_string
{
public:
  static const std::size_t npos = std::basic_string<charT,traits,Alloc>::npos;

  typedef std::string::size_type size_type;
  typedef void* iterator;               // stub;
  typedef void* const_iterator;         // stub;
  typedef void* reverse_iterator;       // stub;
  typedef void* const_reverse_iterator; // stub;


  //----------------------------------------------------------------------------
  // Construct string object
  //----------------------------------------------------------------------------
  // default (1)
  basic_string ();
  // copy (2)
  basic_string (const cow::basic_string<charT,traits,Alloc>& str);
  // copy (2.1)
#ifndef COWSTRING_NO_EXPLICIT_STDSTRING
  explicit
#endif
  basic_string (const std::basic_string<charT,traits,Alloc>& str);
  // substring (3)
  basic_string (const cow::basic_string<charT,traits,Alloc>& str, std::size_t pos, std::size_t len = npos);
  basic_string (const std::basic_string<charT,traits,Alloc>& str, std::size_t pos, std::size_t len = npos);
  // from c-string (4)
  basic_string (const char* nul_terminated_c_str);
  // from buffer (5)
  basic_string (const char* s, std::size_t n);
  // fill (6)
  basic_string (std::size_t n, char c);
  // range (7)
  template <class InputIterator>
  basic_string (InputIterator first, InputIterator last);
#if __cplusplus >= 201103L
  // initializer list (8)
  basic_string (std::initializer_list<char> il);
  // move (9)
  basic_string (const cow::basic_string<charT,traits,Alloc>&& str);
  // move (9.1)
# ifndef COWSTRING_NO_EXPLICIT_STDSTRING
  explicit
# endif
  basic_string (const std::basic_string<charT,traits,Alloc>&& str);
#endif


  //----------------------------------------------------------------------------
  // String destructor
  //----------------------------------------------------------------------------
  ~basic_string();


  //----------------------------------------------------------------------------
  // String assignment : cow::string::operator=
  //----------------------------------------------------------------------------
  // string (1)
  cow::basic_string<charT,traits,Alloc>& operator= (const std::basic_string<charT,traits,Alloc>& str);
  // cow::string (1.1)
  cow::basic_string<charT,traits,Alloc>& operator= (const cow::basic_string<charT,traits,Alloc>& str);
  // c-string (2)
  cow::basic_string<charT,traits,Alloc>& operator= (const char* s);
  // character (3)
  cow::basic_string<charT,traits,Alloc>& operator= (char c);
#if __cplusplus >= 201103L
  // initializer list (4)
  cow::basic_string<charT,traits,Alloc>& operator= (std::initializer_list<char> il);
  // move (5)
  cow::basic_string<charT,traits,Alloc>& operator= (std::basic_string<charT,traits,Alloc>&& str) noexcept;
  // move (5.1)
  cow::basic_string<charT,traits,Alloc>& operator= (cow::basic_string<charT,traits,Alloc>&& str) noexcept;
#endif


  //----------------------------------------------------------------------------
  // Return iterator to beginning : cow::string::begin(..)
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
        iterator begin() noexcept;
  const_iterator begin() const noexcept;
#else
        iterator begin();
  const_iterator begin() const;
#endif


  //----------------------------------------------------------------------------
  // Return iterator to end : cow::string::end(..)
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
        iterator end() noexcept;
  const_iterator end() const noexcept;
#else
        iterator end();
  const_iterator end() const;
#endif


  //----------------------------------------------------------------------------
  // Return reverse iterator to reverse beginning : cow::string::rbegin(..)
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
        reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
#else
        reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
#endif


  //----------------------------------------------------------------------------
  // Return reverse iterator to reverse end : cow::string::rend(..)
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
        reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
#else
        reverse_iterator rend();
  const_reverse_iterator rend() const;
#endif


  //----------------------------------------------------------------------------
  // C++11 explicit const_iterator functions.
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
  const_iterator         cbegin()  const noexcept;
  const_iterator         cend()    const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  const_reverse_iterator crend()   const noexcept;
#endif


  //----------------------------------------------------------------------------
  // Return length of string
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
  std::size_t size() const noexcept;
  std::size_t length() const noexcept;
  std::size_t max_size() const noexcept;
  std::size_t capacity() const noexcept;
  bool        empty() const noexcept;
#else
  std::size_t size() const;
  std::size_t length() const;
  std::size_t max_size() const;
  std::size_t capacity() const;
  bool        empty() const;
#endif


  //----------------------------------------------------------------------------
  // Change string size
  //----------------------------------------------------------------------------
  void resize(std::size_t n);
  void resize(std::size_t n, char c);
  void reserve(std::size_t n = 0);
#if __cplusplus >= 201103L
  void clear() noexcept;
  void shrink_to_fit();
#else
  void clear();
#endif

  //----------------------------------------------------------------------------
  // Get character of string
  //----------------------------------------------------------------------------
  // TODO(olegat) should return a class that override write-operators.
        char& operator[] (std::size_t pos);
  const char& operator[] (std::size_t pos) const;
        char& at   (std::size_t pos);
  const char& at   (std::size_t pos) const;
#if __cplusplus >= 201103L
        char& back (std::size_t pos);
  const char& back (std::size_t pos) const;
        char& front(std::size_t pos);
  const char& front(std::size_t pos) const;
#endif


  //----------------------------------------------------------------------------
  // Append to string : cow::string::operator+=
  //----------------------------------------------------------------------------
  // string (1)
  cow::basic_string<charT,traits,Alloc>& operator+= (const cow::basic_string<charT,traits,Alloc>& str);
  cow::basic_string<charT,traits,Alloc>& operator+= (const std::basic_string<charT,traits,Alloc>& str);
  // c-string (2)
  cow::basic_string<charT,traits,Alloc>& operator+= (const charT* s);
  // character (3)
  cow::basic_string<charT,traits,Alloc>& operator+= (charT c);
#if __cplusplus >= 201103L
  // initializer list (4)
  cow::basic_string<charT,traits,Alloc>& operator+= (std::initializer_list<charT> il);
#endif

  // string (1)
  cow::basic_string<charT,traits,Alloc>& append (const cow::basic_string<charT,traits,Alloc>& str);
  // substring (2)
  cow::basic_string<charT,traits,Alloc>& append (const cow::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen);
  // c-string (3)
  cow::basic_string<charT,traits,Alloc>& append (const charT* s);
  // buffer (4)
  cow::basic_string<charT,traits,Alloc>& append (const charT* s, std::size_t n);
  // fill (5)
  cow::basic_string<charT,traits,Alloc>& append (std::size_t n, charT c);
#if __cplusplus >= 201103L
  // range (6)
  template <class InputIterator>
  cow::basic_string<charT,traits,Alloc>& append (InputIterator first, InputIterator last);
  // initializer list(7)
  cow::basic_string<charT,traits,Alloc>& append (std::initializer_list<charT> il);
#endif


  //----------------------------------------------------------------------------
  // Modifiers
  //----------------------------------------------------------------------------
  void push_back(charT c);
  void swap(cow::basic_string<charT,traits,Alloc>& str);
  void swap(std::basic_string<charT,traits,Alloc>& str);
#if __cplusplus >= 201103L
  void pop_back();
#endif


  //----------------------------------------------------------------------------
  // Assign content to string : cow::string::assign(..)
  //----------------------------------------------------------------------------
  // string (1)
  cow::basic_string<charT,traits,Alloc>& assign (const cow::basic_string<charT,traits,Alloc>& str);
  // substring (2)
  cow::basic_string<charT,traits,Alloc>& assign (const cow::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen = npos);
  // c-string (3)
  cow::basic_string<charT,traits,Alloc>& assign (const charT* s);
  // buffer (4)
  cow::basic_string<charT,traits,Alloc>& assign (const charT* s, std::size_t n);
  // fill (5)
  cow::basic_string<charT,traits,Alloc>& assign (std::size_t n, charT c);
  // range (6)
  template <class InputIterator>
  cow::basic_string<charT,traits,Alloc>& assign (InputIterator first, InputIterator last);
#if __cplusplus >= 201103L
  // initializer list(7)
  cow::basic_string<charT,traits,Alloc>& assign (std::initializer_list<charT> il);
  // move (8)
  cow::basic_string<charT,traits,Alloc>& assign (cow::basic_string<charT,traits,Alloc>&& str) noexcept;
#endif


  //----------------------------------------------------------------------------
  // Insert into string : cow::string::insert(..)
  //----------------------------------------------------------------------------
  // string (1)
  cow::basic_string<charT,traits,Alloc>& insert (std::size_t pos, const cow::basic_string<charT,traits,Alloc>& str);
  // substring (2)
#if __cplusplus >= 201402L
  cow::basic_string<charT,traits,Alloc>& insert (std::size_t pos, const cow::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen = npos);
#endif
  // c-string (3)
  cow::basic_string<charT,traits,Alloc>& insert (std::size_t pos, const charT* s);
  // buffer (4)
  cow::basic_string<charT,traits,Alloc>& insert (std::size_t pos, const charT* s, std::size_t n);
  // fill (5)
  cow::basic_string<charT,traits,Alloc>& insert (std::size_t pos,   std::size_t n, charT c);
  iterator                               insert (const_iterator p, std::size_t n, charT c);
  // single character (6)
  iterator                               insert (const_iterator p, charT c);
#if __cplusplus >= 201103L
  // range (7)
  template <class InputIterator>
  iterator                               insert (iterator p, InputIterator first, InputIterator last);
  // initializer list (8)
  cow::basic_string<charT,traits,Alloc>& insert (const_iterator p, std::initializer_list<charT> il);
#endif


  //----------------------------------------------------------------------------
  // Erase characters from string : cow::string::erase(..)
  //----------------------------------------------------------------------------
  // sequence (1)
#if __cplusplus >= 201402L
  cow::basic_string<charT,traits,Alloc>& erase (std::size_t pos = 0, std::size_t len = npos);
#endif
#if __cplusplus >= 201103L
  // character (2)
  iterator erase (const_iterator p);
  // range (3)
  iterator erase (const_iterator first, const_iterator last);
#endif


  //----------------------------------------------------------------------------
  // Replace portion of string : cow::string::replace(..)
  //----------------------------------------------------------------------------
  // string (1)
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const std::basic_string<charT,traits,Alloc>& str);
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const cow::basic_string<charT,traits,Alloc>& str);
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, const std::basic_string<charT,traits,Alloc>& str);
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, const cow::basic_string<charT,traits,Alloc>& str);
  // substring (2)
#if __cplusplus >= 201402L
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const std::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen = npos);
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const cow::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen = npos);
#else
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const std::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen);
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const cow::basic_string<charT,traits,Alloc>& str, std::size_t subpos, std::size_t sublen);
#endif
  // c-string (3)
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const charT* s);
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, const charT* s);
  // buffer (4)
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   const charT* s, std::size_t n);
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, const charT* s, std::size_t n);
  // fill (5)
  cow::basic_string<charT,traits,Alloc>& replace (std::size_t pos,   std::size_t len,   std::size_t n, charT c);
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, std::size_t n, charT c);
  // range (6)
  template <class InputIterator>
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);
#if __cplusplus >= 201103L
  // initializer list (7)
  cow::basic_string<charT,traits,Alloc>& replace (const_iterator i1, const_iterator i2, std::initializer_list<charT> il);
#endif


  //----------------------------------------------------------------------------
  // Get C-string equivalent : cow::string::c_str()
  //----------------------------------------------------------------------------
#if __cplusplus >= 201103L
  const charT* c_str() const noexcept;
  const charT* data()  const noexcept;
  Alloc get_allocator() const noexcept;
#else
  const charT* c_str() const;
  const charT* data()  const;
  Alloc get_allocator() const;
#endif


  //----------------------------------------------------------------------------
  // Copy sequence of characters from string : cow::string::copy(..)
  //----------------------------------------------------------------------------
  size_type copy(charT* s, size_type len, size_type pos = 0) const;


  //----------------------------------------------------------------------------
  // Find content in string : cow::string::find(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  std::size_t find (const std::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const noexcept;
  std::size_t find (const cow::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const noexcept;
#else
  std::size_t find (const std::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const;
  std::size_t find (const cow::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const;
#endif
  // c-string (2)
  std::size_t find (const charT* s, std::size_t pos = 0) const;
  // buffer (3)
  std::size_t find (const charT* s, std::size_t pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  std::size_t find (charT c, std::size_t pos = 0) const noexcept;
#else
  std::size_t find (charT c, std::size_t pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Find last occurrence in string : cow::string::rfind(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  std::size_t rfind (const std::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const noexcept;
  std::size_t rfind (const cow::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const noexcept;
#else
  std::size_t rfind (const std::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const;
  std::size_t rfind (const cow::basic_string<charT,traits,Alloc>& str, std::size_t pos = 0) const;
#endif
  // c-string (2)
  std::size_t rfind (const charT* s, std::size_t pos = 0) const;
  // buffer (3)
  std::size_t rfind (const charT* s, std::size_t pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  std::size_t rfind (charT c, std::size_t pos = 0) const noexcept;
#else
  std::size_t rfind (charT c, std::size_t pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Find character in string : cow::string::find_first_of(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  size_type find_first_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
  size_type find_first_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
#else
  size_type find_first_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
  size_type find_first_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
#endif
  // c-string (2)
  size_type find_first_of (const charT* s, size_type pos = 0) const;
  // buffer (3)
  size_type find_first_of (const charT* s, size_type pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  size_type find_first_of (charT c, size_type pos = 0) const noexcept;
#else
  size_type find_first_of (charT c, size_type pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Find character in string from the end : cow::string::find_last_of(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  size_type find_last_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
  size_type find_last_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
#else
  size_type find_last_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
  size_type find_last_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
#endif
  // c-string (2)
  size_type find_last_of (const charT* s, size_type pos = 0) const;
  // buffer (3)
  size_type find_last_of (const charT* s, size_type pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  size_type find_last_of (charT c, size_type pos = 0) const noexcept;
#else
  size_type find_last_of (charT c, size_type pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Find non-matching character in string string : cow::string::find_first_not_of(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  size_type find_first_not_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
  size_type find_first_not_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
#else
  size_type find_first_not_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
  size_type find_first_not_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
#endif
  // c-string (2)
  size_type find_first_not_of (const charT* s, size_type pos = 0) const;
  // buffer (3)
  size_type find_first_not_of (const charT* s, size_type pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  size_type find_first_not_of (charT c, size_type pos = 0) const noexcept;
#else
  size_type find_first_not_of (charT c, size_type pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Find non-matching character in string from the end : cow::string::find_last_not_of(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  size_type find_last_not_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
  size_type find_last_not_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const noexcept;
#else
  size_type find_last_not_of (const std::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
  size_type find_last_not_of (const cow::basic_string<charT,traits,Alloc>& str, size_type pos = 0) const;
#endif
  // c-string (2)
  size_type find_last_not_of (const charT* s, size_type pos = 0) const;
  // buffer (3)
  size_type find_last_not_of (const charT* s, size_type pos, size_type n) const;
  // character (4)
#if __cplusplus >= 201103L
  size_type find_last_not_of (charT c, size_type pos = 0) const noexcept;
#else
  size_type find_last_not_of (charT c, size_type pos = 0) const;
#endif


  //----------------------------------------------------------------------------
  // Returns a substring : cow::string::substr(..)
  //----------------------------------------------------------------------------
#if __cplusplus > 201703L
  constexpr
#endif
  cow::basic_string<charT,traits,Alloc> substr( size_type pos = 0, size_type count = npos ) const;


  //----------------------------------------------------------------------------
  // Compare strings : cow::string::compare(..)
  //----------------------------------------------------------------------------
  // string (1)
#if __cplusplus >= 201103L
  int compare (const std::basic_string<charT,traits,Alloc>& str) const noexcept;
  int compare (const cow::basic_string<charT,traits,Alloc>& str) const noexcept;
#else
  int compare (const std::basic_string<charT,traits,Alloc>& str) const;
  int compare (const cow::basic_string<charT,traits,Alloc>& str) const;
#endif
  // substrings (2)
  int compare (size_type pos, size_type len, const std::basic_string<charT,traits,Alloc>& str) const;
  int compare (size_type pos, size_type len, const cow::basic_string<charT,traits,Alloc>& str) const;
#if __cplusplus >= 201402L
  int compare (size_type pos, size_type len, const std::basic_string<charT,traits,Alloc>& str, size_type subpos, size_type sublen = npos) const;
  int compare (size_type pos, size_type len, const cow::basic_string<charT,traits,Alloc>& str, size_type subpos, size_type sublen = npos) const;
#else
  int compare (size_type pos, size_type len, const std::basic_string<charT,traits,Alloc>& str, size_type subpos, size_type sublen) const;
  int compare (size_type pos, size_type len, const cow::basic_string<charT,traits,Alloc>& str, size_type subpos, size_type sublen) const;
#endif
  // c-string (3)
  int compare (const charT* s) const;
  int compare (size_type pos, size_type len, const charT* s) const;
  // buffer (4)
  int compare (size_type pos, size_type len, const charT* s, size_type n) const;
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Implicit std::basic_string conversion
  // (necessary to add fake overloads for std::basic_string)
  // Example: std::string s = cow::string("Hello world")
  //----------------------------------------------------------------------------
#ifndef COWSTRING_NO_EXPLICIT_STDSTRING
  explicit
#endif
  operator std::basic_string<charT,traits,Alloc>()  const;

private:
  /* Read-only string, copied-on-write to m_rw_string */
  std::shared_ptr< std::basic_string<charT,traits,Alloc> > m_ro_string;

  /* Read-write string, moved to m_ro_string when this is copied */
  std::unique_ptr< std::basic_string<charT,traits,Alloc> > m_rw_string;

  /* Is this cow_string mutable (RW)? */
  int m_type;
};


#if 0
//------------------------------------------------------------------------------
// Template specialization : std::hash<cow_string>
//------------------------------------------------------------------------------
namespace std {
  template<> struct hash<cow_string> {
    std::size_t operator()(const cow_string& s) const
#if __cplusplus >= 201103L
      noexcept
#endif
    {
      return s.hash();
    }
  };
}
#endif


//------------------------------------------------------------------------------
// Concatenate strings : operator+ (cow::basic_string)
//------------------------------------------------------------------------------

// string (1.1)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        const cow::basic_string<charT,t,A>& rhs);

// string (1.2)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const std::basic_string<charT,t,A>& lhs,
                                        const cow::basic_string<charT,t,A>& rhs);

// string (1.3)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        const std::basic_string<charT,t,A>& rhs);

// c-string (2)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        const charT*                        rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const charT*                        lhs,
                                        const cow::basic_string<charT,t,A>& rhs);

// character (3)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        charT                               rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (charT                               lhs,
                                        const cow::basic_string<charT,t,A>& rhs);

#if __cplusplus >= 201103L // move semantics

// string (1.1)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        const cow::basic_string<charT,t,A>& rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);

// string (1.2)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (std::basic_string<charT,t,A>&&      lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (std::basic_string<charT,t,A>&&      lhs,
                                        const cow::basic_string<charT,t,A>& rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const std::basic_string<charT,t,A>& lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);

// string (1.3)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        std::basic_string<charT,t,A>&&      rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        const std::basic_string<charT,t,A>& rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const cow::basic_string<charT,t,A>& lhs,
                                        std::basic_string<charT,t,A>&&      rhs);

// c-string (2)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        const charT*                        rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (const charT*                        lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);

// character (3)
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (cow::basic_string<charT,t,A>&&      lhs,
                                        charT                               rhs);
template < class charT, class t, class A >
cow::basic_string<charT,t,A> operator+ (charT                               lhs,
                                        cow::basic_string<charT,t,A>&&      rhs);
#endif


//------------------------------------------------------------------------------
// Insert string into stream : operator<< (cow::basic_string)
//------------------------------------------------------------------------------
template < class charT, class t, class A >
std::ostream& operator<< (std::ostream& os, const cow::basic_string<charT,t,A>& str);


//------------------------------------------------------------------------------
// Class instantiations
//------------------------------------------------------------------------------
typedef cow::basic_string<char>      string;
typedef cow::basic_string<char16_t>  u16string;
typedef cow::basic_string<char32_t>  u32string;
typedef basic_string<wchar_t>        wstring;


//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------
#define COWSTRING_UNIMPLEMENTED() \
  do { std::abort(); } while(0)

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string()
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const cow::basic_string<charT,traits,Alloc>& str)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const std::basic_string<charT,traits,Alloc>& str)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const cow::basic_string<charT,traits,Alloc>& str,
  std::size_t pos,
  std::size_t len)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const std::basic_string<charT,traits,Alloc>& str,
  std::size_t pos,
  std::size_t len)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const char* nul_terminated_c_str)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const char* s,
  std::size_t n)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  std::size_t n,
  char c)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
template < class InputIterator >
basic_string<charT,traits,Alloc>::basic_string(
  InputIterator first,
  InputIterator last)
{
  COWSTRING_UNIMPLEMENTED();
}

#if __cplusplus >= 201103L
template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  std::initializer_list<char> il)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const cow::basic_string<charT,traits,Alloc>&& str)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::basic_string(
  const std::basic_string<charT,traits,Alloc>&& str)
{
  COWSTRING_UNIMPLEMENTED();
}
#endif

template < class charT, class traits, class Alloc >
basic_string<charT,traits,Alloc>::~basic_string()
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::length() const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
cow::basic_string<charT,traits,Alloc>&
basic_string<charT,traits,Alloc>::replace(
  std::size_t pos,
  std::size_t len,
  const charT* s)
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::find(
  const std::basic_string<charT,traits,Alloc>& str,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::find(
  const cow::basic_string<charT,traits,Alloc>& str,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::find(
  const charT* s,
  std::size_t pos) const
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::find(
  const charT* s,
  std::size_t pos,
  size_type n) const
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::find(
  charT c,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::rfind(
  const std::basic_string<charT,traits,Alloc>& str,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::rfind(
  const cow::basic_string<charT,traits,Alloc>& str,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::rfind(
  const charT* s,
  std::size_t pos) const
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::rfind(
  const charT* s,
  std::size_t pos,
  size_type n) const
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class traits, class Alloc >
std::size_t
basic_string<charT,traits,Alloc>::rfind(
  charT c,
  std::size_t pos) const
#if __cplusplus >= 201103L
  noexcept
#endif
{
  COWSTRING_UNIMPLEMENTED();
}

template < class charT, class t, class A >
std::ostream& operator<< (
  std::ostream& os,
  const cow::basic_string<charT,t,A>& str)
{
  COWSTRING_UNIMPLEMENTED();
}

} // namespace cow::

