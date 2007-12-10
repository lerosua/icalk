/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef WIN32
#include <malloc.h>
#endif  // WIN32
#ifdef POSIX
#include <alloca.h>
#define _alloca alloca
#endif  // POSIX

#include "talk/base/basictypes.h"
#include "talk/base/common.h"
#include "talk/base/stringencode.h"
#include "talk/base/stringutils.h"

namespace talk_base {

/////////////////////////////////////////////////////////////////////////////
// String Encoding Utilities
/////////////////////////////////////////////////////////////////////////////

static const char HEX[] = "0123456789abcdef";

char hex_encode(unsigned char val) {
  ASSERT(val < 16);
  return (val < 16) ? HEX[val] : '!';
}

unsigned char hex_decode(char ch) {
  char lower = tolower(ch);
  ASSERT(((ch >= '0') && (ch <= '9')) || ((lower >= 'a') && (lower <= 'z')));
  return (ch <= '9') ? (ch - '0') : ((lower - 'a') + 10);
}

size_t escape(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              const char * illegal, char escape) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    char ch = source[srcpos++];
    if ((ch == escape) || ::strchr(illegal, ch)) {
      if (bufpos + 2 >= buflen)
        break;
      buffer[bufpos++] = escape;
    }
    buffer[bufpos++] = ch;
  }

  buffer[bufpos] = '\0';
  return bufpos;
}

size_t unescape(char * buffer, size_t buflen,
                const char * source, size_t srclen,
                char escape) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    char ch = source[srcpos++];
    if ((ch == escape) && (srcpos < srclen)) {
      ch = source[srcpos++];
    }
    buffer[bufpos++] = ch;
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t encode(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              const char * illegal, char escape) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    char ch = source[srcpos++];
    if ((ch != escape) && !::strchr(illegal, ch)) {
      buffer[bufpos++] = ch;
    } else if (bufpos + 3 >= buflen) {
      break;
    } else {
      buffer[bufpos+0] = escape;
      buffer[bufpos+1] = hex_encode((static_cast<unsigned char>(ch) >> 4) & 0xF);
      buffer[bufpos+2] = hex_encode((static_cast<unsigned char>(ch)     ) & 0xF);
      bufpos += 3;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t decode(char * buffer, size_t buflen,
              const char * source, size_t srclen,
              char escape) {
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    char ch = source[srcpos++];
    if ((ch == escape) && (srcpos + 1 < srclen)) {
      buffer[bufpos++] = (hex_decode(source[srcpos]) << 4)
                         | hex_decode(source[srcpos+1]);
      srcpos += 2;
    } else {
      buffer[bufpos++] = ch;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

const char* unsafe_filename_characters() {
  // It might be better to have a single specification which is the union of
  // all operating systems, unless one system is overly restrictive.
#ifdef WIN32
  return "\\/:*?\"<>|";
#else  // !WIN32
  // TODO
#endif  // !WIN23
}

const unsigned char URL_UNSAFE  = 0x1; // 0-33 "#$%&+,/:;<=>?@[\]^`{|} 127
const unsigned char XML_UNSAFE  = 0x2; // "&'<>
const unsigned char HTML_UNSAFE = 0x2; // "&'<>

//  ! " # $ % & ' ( ) * + , - . / 0 1 2 3 4 6 5 7 8 9 : ; < = > ?
//@ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _
//` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~ 

const unsigned char ASCII_CLASS[128] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,3,1,1,1,3,2,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,3,1,3,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,
};

size_t url_encode(char * buffer, size_t buflen,
                  const char * source, size_t srclen) {
  if (NULL == buffer)
    return srclen * 3 + 1;
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    unsigned char ch = source[srcpos++];
    if ((ch < 128) && (ASCII_CLASS[ch] & URL_UNSAFE)) {
      if (bufpos + 3 >= buflen) {
        break;
      }
      buffer[bufpos+0] = '%';
      buffer[bufpos+1] = hex_encode((ch >> 4) & 0xF);
      buffer[bufpos+2] = hex_encode((ch     ) & 0xF);
      bufpos += 3;
    } else {
      buffer[bufpos++] = ch;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t url_decode(char * buffer, size_t buflen,
                  const char * source, size_t srclen) {
  if (NULL == buffer)
    return srclen + 1;
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    unsigned char ch = source[srcpos++];
    if (ch == '+') {
      buffer[bufpos++] = ' ';
    } else if ((ch == '%') && (srcpos + 1 < srclen)) {
      buffer[bufpos++] = (hex_decode(source[srcpos]) << 4)
                         | hex_decode(source[srcpos+1]);
      srcpos += 2;
    } else {
      buffer[bufpos++] = ch;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t utf8_decode(const char* source, size_t srclen, unsigned long* value) {
  const unsigned char* s = reinterpret_cast<const unsigned char*>(source);
  if ((s[0] & 0x80) == 0x00) {                    // Check s[0] == 0xxxxxxx
    *value = s[0];
    return 1;
  }
  if ((srclen < 2) || ((s[1] & 0xC0) != 0x80)) {  // Check s[1] != 10xxxxxx
    return 0;
  }
  // Accumulate the trailer byte values in value16, and combine it with the
  // relevant bits from s[0], once we've determined the sequence length.
  unsigned long value16 = (s[1] & 0x3F);
  if ((s[0] & 0xE0) == 0xC0) {                    // Check s[0] == 110xxxxx
    *value = ((s[0] & 0x1F) << 6) | value16;
    return 2;
  }
  if ((srclen < 3) || ((s[2] & 0xC0) != 0x80)) {  // Check s[2] != 10xxxxxx
    return 0;
  }
  value16 = (value16 << 6) | (s[2] & 0x3F);
  if ((s[0] & 0xF0) == 0xE0) {                    // Check s[0] == 1110xxxx 
    *value = ((s[0] & 0x0F) << 12) | value16;
    return 3;
  }
  if ((srclen < 4) || ((s[3] & 0xC0) != 0x80)) {  // Check s[3] != 10xxxxxx
    return 0;
  }
  value16 = (value16 << 6) | (s[3] & 0x3F);
  if ((s[0] & 0xF8) == 0xF0) {                    // Check s[0] == 11110xxx 
    *value = ((s[0] & 0x07) << 18) | value16;
    return 4;
  }
  return 0;
}

size_t utf8_encode(char* buffer, size_t buflen, unsigned long value) {
  if ((value <= 0x7F) && (buflen >= 1)) {
    buffer[0] = static_cast<unsigned char>(value);
    return 1;
  }
  if ((value <= 0x7FF) && (buflen >= 2)) {
    buffer[0] = 0xC0 | static_cast<unsigned char>(value >> 6);
    buffer[1] = 0x80 | static_cast<unsigned char>(value & 0x3F);
    return 2;
  }
  if ((value <= 0xFFFF) && (buflen >= 3)) {
    buffer[0] = 0xE0 | static_cast<unsigned char>(value >> 12);
    buffer[1] = 0x80 | static_cast<unsigned char>((value >> 6) & 0x3F);
    buffer[2] = 0x80 | static_cast<unsigned char>(value & 0x3F);
    return 3;
  }
  if ((value <= 0x1FFFFF) && (buflen >= 4)) {
    buffer[0] = 0xF0 | static_cast<unsigned char>(value >> 18);
    buffer[1] = 0x80 | static_cast<unsigned char>((value >> 12) & 0x3F);
    buffer[2] = 0x80 | static_cast<unsigned char>((value >> 6) & 0x3F);
    buffer[3] = 0x80 | static_cast<unsigned char>(value & 0x3F);
    return 4;
  }
  return 0;
}

size_t html_encode(char * buffer, size_t buflen,
                   const char * source, size_t srclen) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    unsigned char ch = source[srcpos];
    if (ch < 128) {
      srcpos += 1;
      if (ASCII_CLASS[ch] & HTML_UNSAFE) {
        const char * escseq = 0;
        size_t esclen = 0;
        switch (ch) {
          case '<':  escseq = "&lt;";   esclen = 4; break;
          case '>':  escseq = "&gt;";   esclen = 4; break;
          case '\'': escseq = "&#39;";  esclen = 5; break;
          case '\"': escseq = "&quot;"; esclen = 6; break;
          case '&':  escseq = "&amp;";  esclen = 5; break;
          default: ASSERT(false);
        }
        if (bufpos + esclen >= buflen) {
          break;
        }
        memcpy(buffer + bufpos, escseq, esclen);
        bufpos += esclen;
      } else {
        buffer[bufpos++] = ch;
      }
    } else {
      // Largest value is 0x1FFFFF => &#2097151;  (10 characters)
      char escseq[11];
      unsigned long val;
      if (size_t vallen = utf8_decode(&source[srcpos], srclen - srcpos, &val)) {
        srcpos += vallen;
      } else {
        // Not a valid utf8 sequence, just use the raw character.
        val = static_cast<unsigned char>(source[srcpos++]);
      }
      size_t esclen = sprintfn(escseq, ARRAY_SIZE(escseq), "&#%lu;", val);
      if (bufpos + esclen >= buflen) {
        break;
      }
      memcpy(buffer + bufpos, escseq, esclen);
      bufpos += esclen;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t html_decode(char * buffer, size_t buflen,
                   const char * source, size_t srclen) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  return xml_decode(buffer, buflen, source, srclen);
}

size_t xml_encode(char * buffer, size_t buflen,
                  const char * source, size_t srclen) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    unsigned char ch = source[srcpos++];
    if ((ch < 128) && (ASCII_CLASS[ch] & XML_UNSAFE)) {
      const char * escseq = 0;
      size_t esclen = 0;
      switch (ch) {
        case '<':  escseq = "&lt;";   esclen = 4; break;
        case '>':  escseq = "&gt;";   esclen = 4; break;
        case '\'': escseq = "&apos;"; esclen = 6; break;
        case '\"': escseq = "&quot;"; esclen = 6; break;
        case '&':  escseq = "&amp;";  esclen = 5; break;
        default: ASSERT(false);
      }
      if (bufpos + esclen >= buflen) {
        break;
      }
      memcpy(buffer + bufpos, escseq, esclen);
      bufpos += esclen;
    } else {
      buffer[bufpos++] = ch;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t xml_decode(char * buffer, size_t buflen,
                  const char * source, size_t srclen) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
    unsigned char ch = source[srcpos++];
    if (ch != '&') {
      buffer[bufpos++] = ch;
    } else if ((srcpos + 2 < srclen)
               && (memcmp(source + srcpos, "lt;", 3) == 0)) {
      buffer[bufpos++] = '<';
      srcpos += 3;
    } else if ((srcpos + 2 < srclen)
               && (memcmp(source + srcpos, "gt;", 3) == 0)) {
      buffer[bufpos++] = '>';
      srcpos += 3;
    } else if ((srcpos + 4 < srclen)
               && (memcmp(source + srcpos, "apos;", 5) == 0)) {
      buffer[bufpos++] = '\'';
      srcpos += 5;
    } else if ((srcpos + 4 < srclen)
               && (memcmp(source + srcpos, "quot;", 5) == 0)) {
      buffer[bufpos++] = '\"';
      srcpos += 5;
    } else if ((srcpos + 3 < srclen)
               && (memcmp(source + srcpos, "amp;", 4) == 0)) {
      buffer[bufpos++] = '&';
      srcpos += 4;
    } else if ((srcpos < srclen) && (source[srcpos] == '#')) {
      int int_base = 10;
      if ((srcpos + 1 < srclen) && (source[srcpos+1] == 'x')) {
        int_base = 16;
        srcpos += 1;
      }
      char * ptr;
      // TODO: Fix hack (ptr may go past end of data)
      unsigned long val = strtoul(source + srcpos + 1, &ptr, int_base);
      if ((static_cast<size_t>(ptr - source) < srclen) && (*ptr == ';')) {
        srcpos = ptr - source + 1;
      } else {
        // Not a valid escape sequence.
        break;
      }
      if (size_t esclen = utf8_encode(buffer + bufpos, buflen - bufpos, val)) {
        bufpos += esclen;
      } else {
        // Not enough room to encode the character, or illegal character
        break;
      }
    } else {
      // Unrecognized escape sequence.
      break;
    }
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t hex_encode(char * buffer, size_t buflen,
                  const char * csource, size_t srclen) {
  ASSERT(NULL != buffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  const unsigned char * bsource =
    reinterpret_cast<const unsigned char *>(csource);

  size_t srcpos = 0, bufpos = 0;
  srclen = _min(srclen, (buflen - 1) / 2);
  while (srcpos < srclen) {
    unsigned char ch = bsource[srcpos++];
    buffer[bufpos  ] = hex_encode((ch >> 4) & 0xF);
    buffer[bufpos+1] = hex_encode((ch     ) & 0xF);
    bufpos += 2;
  }
  buffer[bufpos] = '\0';
  return bufpos;
}

size_t hex_decode(char * cbuffer, size_t buflen,
                  const char * source, size_t srclen) {
  ASSERT(NULL != cbuffer);  // TODO: estimate output size
  if (buflen <= 0)
    return 0;

  unsigned char * bbuffer = reinterpret_cast<unsigned char *>(cbuffer);

  size_t srcpos = 0, bufpos = 0;
  while ((srcpos + 1 < srclen) && (bufpos + 1 < buflen)) {
    unsigned char v1 = (hex_decode(source[srcpos]) << 4);
    unsigned char v2 = hex_decode(source[srcpos+1]);
    bbuffer[bufpos++] =  v1 | v2;
    srcpos += 2;
  }
  bbuffer[bufpos] = '\0';
  return bufpos;
}

void transform(std::string& value, size_t maxlen, const std::string& source,
               Transform t) {
  char * buffer = static_cast<char *>(_alloca(maxlen + 1));
  value.assign(buffer, t(buffer, maxlen + 1, source.data(), source.length()));
}

std::string s_transform(const std::string& source, Transform t) {
  // Ask transformation function to approximate the destination size (returns upper bound)
  size_t maxlen = t(NULL, 0, source.data(), source.length());
  char * buffer = static_cast<char *>(_alloca(maxlen));
  size_t len = t(buffer, maxlen, source.data(), source.length());
  std::string result(buffer, len);
  return result;
}

char make_char_safe_for_filename(char c) {
  if (c < 32)
    return '_';

  switch (c) {
    case '<':
    case '>':
    case ':':
    case '"':
    case '/':
    case '\\':
    case '|':
    case '*':
    case '?':
      return '_';

    default:
      return c;
  }
}

/*
void sprintf(std::string& value, size_t maxlen, const char * format, ...) {
  char * buffer = static_cast<char *>(alloca(maxlen + 1));
  va_list args;
  va_start(args, format);
  value.assign(buffer, vsprintfn(buffer, maxlen + 1, format, args));
  va_end(args);
}
*/

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG

static int utf8_unittest() {
  const struct Utf8Test {
    const char* encoded;
    size_t encsize, enclen;
    unsigned long decoded;
  } kTests[] = {
    { "a    ",             5, 1, 'a' },
    { "\x7F    ",          5, 1, 0x7F },
    { "\xC2\x80   ",       5, 2, 0x80 },
    { "\xDF\xBF   ",       5, 2, 0x7FF },
    { "\xE0\xA0\x80  ",    5, 3, 0x800 },
    { "\xEF\xBF\xBF  ",    5, 3, 0xFFFF },
    { "\xF0\x90\x80\x80 ", 5, 4, 0x10000 },
    { "\xF0\x90\x80\x80 ", 3, 0, 0x10000 },
    { "\xF0\xF0\x80\x80 ", 5, 0, 0 },
    { "\xF0\x90\x80  ",    5, 0, 0 },
    { "\x90\x80\x80  ",    5, 0, 0 },
    { NULL, 0, 0 },
  };
  for (size_t i=0; kTests[i].encoded; ++i) {
    unsigned long val = 0;
    ASSERT(kTests[i].enclen == utf8_decode(kTests[i].encoded,
                                           kTests[i].encsize,
                                           &val));
    unsigned long result = (kTests[i].enclen == 0) ? 0 : kTests[i].decoded;
    ASSERT(val == result);

    if (kTests[i].decoded == 0) {
      // Not an interesting encoding test case
      continue;
    }

    char buffer[5];
    memset(buffer, 0x01, ARRAY_SIZE(buffer));
    ASSERT(kTests[i].enclen == utf8_encode(buffer,
                                           kTests[i].encsize,
                                           kTests[i].decoded));
    ASSERT(memcmp(buffer, kTests[i].encoded, kTests[i].enclen) == 0);
    // Make sure remainder of buffer is unchanged
    ASSERT(memory_check(buffer + kTests[i].enclen,
                        0x1,
                        ARRAY_SIZE(buffer) - kTests[i].enclen));
  }
  return 1;
}

int test = utf8_unittest();

#endif  // _DEBUG

/////////////////////////////////////////////////////////////////////////////

}  // namespace talk_base
