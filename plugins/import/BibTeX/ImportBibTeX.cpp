/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/StringCollection.h>

#include <xdkbibtex/file.h>
#include <xdkbibtex/formatter.h>
#include <xdkbibtex/author.h>

#if defined(_MSC_VER) && !defined(strtok_r)
#define strtok_r strtok_s
#endif
#if defined(strtok_r) && defined(_WIN32) && defined(__GNUC__) &&                                   \
    ((__GNUC__ * 100 + __GNUC__MINOR) < 409)
// in MINGW environment
// strtok_r is declared in pthread.h instead of string.h
// and is a not reentrant macro so use a public domain version
#undef strtok_r
/*
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */
static char *strtok_r(char *str, const char *delim, char **nextp) {
  char *ret;

  if (str == NULL) {
    str = *nextp;
  }

  str += strspn(str, delim);

  if (*str == '\0') {
    return NULL;
  }

  ret = str;

  str += strcspn(str, delim);

  if (*str) {
    *str++ = '\0';
  }

  *nextp = str;

  return ret;
}
#endif

using namespace std;
using namespace tlp;

// the intent of this function is to detect
// ill-formed utf8 chars (ascii or windows 1252 chars)
// latex special chars
// and to replace them by the corresponding utf8 sequence
static char *utf8seq;
static char *seq(char cc[2]) {
  return utf8seq = cc;
}
static string &forceUtf8String(string &str) {
  char c3[2] = {'\303', ' '};
  char c4[2] = {'\304', ' '};
  char c5[2] = {'\305', ' '};
  char charComposing = 0;

  for (unsigned int i = 0; i < str.size(); ++i) {
    if ((str[i] > '\301') && (str[i] < '\340') && (i + 1) < str.size() &&
        // begin of a 2 chars utf8 sequence
        // skip next char if it is valid utf8
        (uchar(str[i + 1]) > '\177') && (str[i + 1] < '\300')) {
      ++i;
      continue;
    }

    if ((str[i] > '\337') && (str[i] < '\360') && (i + 2) < str.size() &&
        // begin of a 3 chars utf8 sequence
        // skip next 2 chars if it is valid utf8
        (uchar(str[i + 1]) > '\177') && (str[i + 1] < '\300') && (uchar(str[i + 2]) > '\177') &&
        (str[i + 2] < '\300')) {
      i += 2;
      continue;
    }

    switch (/*(unsigned char)*/ str[i]) {
    // é
    case '\202': // extended ascii
    case '\351': // windows 1252
      seq(c3)[1] = '\251';
      break;

    // ê
    case '\210': // extended ascii
    case '\352': // windows 1252
      seq(c3)[1] = '\252';
      break;

    // ë
    case '\211': // extended ascii
    case '\353': // windows 1252
      seq(c3)[1] = '\253';
      break;

    // è
    case '\212': // extended ascii
    case '\350': // windows 1252
      seq(c3)[1] = '\250';
      break;

    // á
    case '\240': // extended ascii
    case '\341': // windows 1252
      seq(c3)[1] = '\241';
      break;

    // â
    case '\203': // extended ascii
    case '\342': // windows 1252
      seq(c3)[1] = '\242';
      break;

    // ä
    case '\204': // extended ascii
    case '\344': // windows 1252
      seq(c3)[1] = '\244';
      break;

    // à
    case '\205': // extended ascii
    case '\340': // windows 1252
      seq(c3)[1] = '\240';
      break;

    // å
    case '\206': // extended ascii
    case '\345': // windows 1252
      seq(c3)[1] = '\245';
      break;

    // ã
    case '\343': // windows 1252
      seq(c3)[1] = '\243';
      break;

    // ï
    case '\213': // extended ascii
    case '\357': // windows 1252
      seq(c3)[1] = '\257';
      break;

    // î
    case '\214': // extended ascii
    case '\356': // windows 1252
      seq(c3)[1] = '\256';
      break;

    // ì
    case '\215': // extended ascii
    case '\354': // windows 1252
      seq(c3)[1] = '\254';
      break;

    // í
    case '\241': // extended ascii
    case '\355': // windows 1252
      seq(c3)[1] = '\255';
      break;

    // ô
    case '\223': // extended ascii
    case '\364': // windows 1252
      seq(c3)[1] = '\264';
      break;

    // ö
    case '\224': // extended ascii
    case '\366': // windows 1252
      seq(c3)[1] = '\266';
      break;

    // ò
    case '\225': // extended ascii
    case '\362': // windows 1252
      seq(c3)[1] = '\262';
      break;

    // ø
    case '\233': // extended ascii
    case '\370': // windows 1252
      seq(c3)[1] = '\270';
      break;

    // õ
    case '\365': // windows 1252
      seq(c3)[1] = '\265';
      break;

    // ó
    case '\242': // extended ascii
    case '\363': // windows 1252
      seq(c3)[1] = '\263';
      break;

    // ü
    case '\201': // extended ascii
    case '\374': // windows 1252
      seq(c3)[1] = '\274';
      break;

    // û
    case '\226': // extended ascii
    case '\373': // windows 1252
      seq(c3)[1] = '\273';
      break;

    // ù
    case '\227': // extended ascii
    case '\371': // windows 1252
      seq(c3)[1] = '\271';
      break;

    // ú
    case '\243': // extended ascii
    case '\372': // windows 1252
      seq(c3)[1] = '\272';
      break;

    // ÿ
    case '\230': // extended ascii
    case '\377': // windows 1252
      seq(c3)[1] = '\277';
      break;

    // ç
    case '\347': // extended ascii & windows 1252
      seq(c3)[1] = '\247';
      break;

    // ñ
    case '\244': // extended ascii
    case '\361': // windows 1252
      seq(c3)[1] = '\261';
      break;

    // É
    case '\220': // extended ascii
    case '\311': // windows 1252
      seq(c3)[1] = '\211';
      break;

    // Ê
    case '\322': // extended ascii
    case '\312': // windows 1252
      seq(c3)[1] = '\212';
      break;

    // Ë
    case '\323': // extended ascii
    case '\313': // windows 1252
      seq(c3)[1] = '\213';
      break;

    // È
    case '\324': // extended ascii
    case '\310': // windows 1252
      seq(c3)[1] = '\210';
      break;

    // ß
    // '\341' is the extended ascii code of ß
    // but is also the windows 1252 code of á
    // case '\341': // extended ascii
    case '\337': // windows 1252
      seq(c3)[1] = '\237';
      break;

    case '\303': // begin of a 2 bytes utf8 char
      ++i;
      continue;

    case '}': // end of latex command
    case '{': // begin of latex command
      // remove {
      str.replace(i, 1, 0, ' ');
      --i;
      continue;

    case '\\': // character composition
      if (charComposing) {
        // next one should be an i
        if (str[i + 1] == 'i')
          switch (charComposing) {
          case '`': // ì
            seq(c3)[1] = '\254';
            break;

          case '\'': // í
            seq(c3)[1] = '\255';
            break;

          case '^': // î
            seq(c3)[1] = '\256';
            break;

          case '"': // ï
            seq(c3)[1] = '\257';
            break;

          case '~': // ĩ
            seq(c4)[1] = '\251';
            break;

          case '=': // ī
            seq(c4)[1] = '\253';
            break;

          case 'u': // ĭ
            seq(c4)[1] = '\254';
            break;

          default:
            continue;
          }

        str.replace(i, 2, utf8seq, 2);
        ++i;
        continue;
      }

      switch (str[i + 1]) {
      case '`':  // grave
      case '\'': // acute
      case '^':  // circumflex
      case '"':  // diaeresis
      case '~':  // tilde
      case '=':  // macron
      case '.':  // dot over
      case 'b':  // bar under
      case 'c':  // cedilla
      case 'k':  // ogonek
      case 'r':  // ring over
      case 's':  // sharp s
      case 'u':  // breve over
      case 'v':  // caron
        // record char
        charComposing = str[i + 1];
        // remove \\+char
        str.replace(i, 2, 0, ' ');
        --i;
        continue;

      case 'a':
        if (str.size() > i + 2 && str[i + 2] == 'e') {
          // replace by æ
          c3[1] = '\246';
          str.replace(i, 3, c3, 2);
          ++i;
        }

        continue;

      case 'A':
        if (str.size() > i + 2 && str[i + 2] == 'E') {
          // replace by Æ
          c3[1] = '\206';
          str.replace(i, 3, c3, 2);
          ++i;
        }

        continue;

      case 'd':
        // replace by đ
        c4[1] = '\221';
        str.replace(i, 2, c4, 2);
        ++i;
        continue;

      case 'D':
        // replace by Đ
        c4[1] = '\220';
        str.replace(i, 2, c4, 2);
        ++i;
        continue;

      case 'h':
        // replace by ħ
        c4[1] = '\247';
        str.replace(i, 2, c4, 2);
        ++i;
        continue;

      case 'H':
        if (str.size() > i + 2) {
          switch (str[i + 2]) {
          case '0': // Ő
            seq(c5)[1] = '\220';
            str.replace(i, 3, c5, 2);
            ++i;
            continue;

          case 'o': // ő
            seq(c5)[1] = '\221';
            str.replace(i, 3, c5, 2);
            ++i;
            continue;

          case 'U': // Ű
            seq(c5)[1] = '\260';
            str.replace(i, 3, c5, 2);
            ++i;
            continue;

          case 'u': // ű
            seq(c5)[1] = '\261';
            str.replace(i, 3, c5, 2);
            ++i;
            continue;

          default:
            break;
          }
        }

        // replace by Ħ
        c4[1] = '\246';
        str.replace(i, 2, c4, 2);
        ++i;
        continue;

      case 'l':
        // replace by ł
        c5[1] = '\202';
        str.replace(i, 2, c5, 2);
        ++i;
        continue;

      case 'L':
        // replace by Ł
        c5[1] = '\201';
        str.replace(i, 2, c5, 2);
        ++i;
        continue;

      case 'o':
        if (str.size() > i + 2 && str[i + 2] == 'e') {
          // replace by œ
          c5[1] = '\223';
          str.replace(i, 3, c5, 2);
        } else {
          // replace by ø
          c3[1] = '\270';
          str.replace(i, 2, c3, 2);
        }

        ++i;
        continue;

      case 'O':
        if (str.size() > i + 2 && str[i + 2] == 'E') {
          // replace by Œ
          c5[1] = '\222';
          str.replace(i, 3, c5, 2);
        } else {
          // replace by Ø
          c3[1] = '\230';
          str.replace(i, 2, c3, 2);
        }

        ++i;
        continue;

      case 't':
        // replace by ŧ
        c5[1] = '\245';
        str.replace(i, 2, c5, 2);
        ++i;
        continue;

      case 'T':
        // replace by Ŧ
        c5[1] = '\246';
        str.replace(i, 2, c5, 2);
        ++i;
        continue;

      default:
        break;
      }

    case ' ':
      if (charComposing) {
        str.replace(i, 1, 0, ' ');
        --i;
        continue;
      }

    default:
      if (charComposing) {
        switch (str[i]) {
        case 'a':
          switch (charComposing) {
          case '`': // à
            seq(c3)[1] = '\240';
            break;

          case '\'': // á
            seq(c3)[1] = '\241';
            break;

          case '^': // â
            seq(c3)[1] = '\242';
            break;

          case '~': // ã
            seq(c3)[1] = '\243';
            break;

          case '"': // ä
            seq(c3)[1] = '\244';
            break;

          case 'r': // å
            seq(c3)[1] = '\245';
            break;

          case '=': // ā
            seq(c4)[1] = '\201';
            break;

          case 'u': // ă
            seq(c4)[1] = '\203';
            break;

          case 'k': // ą
            seq(c4)[1] = '\205';
            break;

          default:
            continue;
          }

          break;

        case 'c':
          switch (charComposing) {
          case 'c': // ç
            seq(c3)[1] = '\247';
            break;

          case '\'': // ć
            seq(c4)[1] = '\207';
            break;

          case '^': // ĉ
            seq(c4)[1] = '\211';
            break;

          case '.': // ċ
            seq(c4)[1] = '\213';
            break;

          case 'v': // č
            seq(c4)[1] = '\215';
            break;

          default:
            continue;
          }

          break;

        case 'd':
          if (charComposing == 'v') {
            // ď
            seq(c4)[1] = '\217';
            break;
          } else
            continue;

        case 'e':
          switch (charComposing) {
          case '`': // è
            seq(c3)[1] = '\250';
            break;

          case '\'': // é
            seq(c3)[1] = '\251';
            break;

          case '^': // ê
            seq(c3)[1] = '\252';
            break;

          case '"': // ë
            seq(c3)[1] = '\253';
            break;

          case '=': // ë
            seq(c4)[1] = '\223';
            break;

          case 'u': // ĕ
            seq(c4)[1] = '\225';
            break;

          case '.': // ė
            seq(c4)[1] = '\227';
            break;

          case 'k': // ę
            seq(c4)[1] = '\231';
            break;

          case 'v': // ě
            seq(c4)[1] = '\233';
            break;

          default:
            continue;
          }

          break;

        case 'g':
          switch (charComposing) {
          case '^': // ĝ
            seq(c4)[1] = '\235';
            break;

          case 'u': // ğ
            seq(c4)[1] = '\237';
            break;

          case '.': // ġ
            seq(c4)[1] = '\241';
            break;

          case 'c': // ģ
            seq(c4)[1] = '\243';
            break;

          default:
            continue;
          }

          break;

        case 'h':
          if (charComposing == '^') {
            seq(c4)[1] = '\245';
            break;
          } else
            continue;

        case 'j':
          if (charComposing == '^') {
            // ĵ
            seq(c4)[1] = '\265';
            break;
          } else
            continue;

        case 'k':
          if (charComposing == 'c') {
            // ķ
            seq(c4)[1] = '\267';
            break;
          } else
            continue;

        case 'l':
          switch (charComposing) {
          case '\'': // ĺ
            seq(c4)[1] = '\272';
            break;

          case 'c': // ļ
            seq(c4)[1] = '\274';
            break;

          case 'v': // ľ
            seq(c4)[1] = '\276';
            break;

          default:
            continue;
          }

          break;

        case 'n':
          switch (charComposing) {
          case '~': //  ñ
            seq(c3)[1] = '\261';
            break;

          case '\'': // ń
            seq(c5)[1] = '\204';
            break;

          case 'c': // ņ
            seq(c5)[1] = '\206';
            break;

          case 'v': // ň
            seq(c5)[1] = '\210';
            break;

          default:
            continue;
          }

          break;

        case 'o':
          switch (charComposing) {
          case '`': // ò
            seq(c3)[1] = '\262';
            break;

          case '\'': // ó
            seq(c3)[1] = '\263';
            break;

          case '^': // ô
            seq(c3)[1] = '\264';
            break;

          case '~': // õ
            seq(c3)[1] = '\265';
            break;

          case '"': // ö
            seq(c3)[1] = '\266';
            break;

          case '=': // ō
            seq(c5)[1] = '\215';
            break;

          case 'u': // ŏ
            seq(c5)[1] = '\217';
            break;

          default:
            continue;
          }

          break;

        case 'r':
          switch (charComposing) {
          case '\'': // ŕ
            seq(c5)[1] = '\225';
            break;

          case 'c': // ŗ
            seq(c5)[1] = '\227';
            break;

          case 'v': // ř
            seq(c5)[1] = '\230';
            break;

          default:
            continue;
          }

          break;

        case 's':
          switch (charComposing) {
          case 's': // ß
            seq(c3)[1] = '\237';
            break;

          case '\'': // ś
            seq(c5)[1] = '\233';
            break;

          case '^': // ŝ
            seq(c5)[1] = '\235';
            break;

          case 'c': // ş
            seq(c5)[1] = '\237';
            break;

          case 'v': // š
            seq(c5)[1] = '\241';
            break;

          default:
            continue;
          }

          break;

        case 't':
          switch (charComposing) {
          case 'c': // ţ
            seq(c5)[1] = '\243';
            break;

          case 'v': // ť
            seq(c5)[1] = '\245';
            break;

          default:
            continue;
          }

          break;

        case 'u':
          switch (charComposing) {
          case '`': // ù
            seq(c3)[1] = '\271';
            break;

          case '\'': // ú
            seq(c3)[1] = '\272';
            break;

          case '^': // û
            seq(c3)[1] = '\273';
            break;

          case '"': // ü
            seq(c3)[1] = '\274';
            break;

          case '~': // ũ
            seq(c5)[1] = '\251';
            break;

          case '=': // ū
            seq(c5)[1] = '\253';
            break;

          case 'u': // ŭ
            seq(c5)[1] = '\255';
            break;

          case 'r': // ů
            seq(c5)[1] = '\257';
            break;

          case 'k': // ų
            seq(c5)[1] = '\263';
            break;

          default:
            continue;
          }

          break;

        case 'w':
          if (charComposing == '^') {
            // ŵ
            seq(c5)[1] = '\265';
            break;
          } else
            continue;

        case 'y':
          switch (charComposing) {
          case '\'': // ý
            seq(c3)[1] = '\275';
            break;

          case '"': // ÿ
            seq(c3)[1] = '\276';
            break;

          case '^': // ŷ
            seq(c5)[1] = '\267';
            break;

          default:
            continue;
          }

          break;

        case 'z':
          switch (charComposing) {
          case '\'': // ź
            seq(c5)[1] = '\272';
            break;

          case '.': // ż
            seq(c5)[1] = '\274';
            break;

          case 'v': // ž
            seq(c5)[1] = '\276';
            break;

          default:
            continue;
          }

          break;

        case 'A':
          switch (charComposing) {
          case '`': // À
            seq(c3)[1] = '\200';
            break;

          case '\'': // Á
            seq(c3)[1] = '\201';
            break;

          case '^': // Â
            seq(c3)[1] = '\202';
            break;

          case '~': // Ã
            seq(c3)[1] = '\203';
            break;

          case '"': // Ä
            seq(c3)[1] = '\204';
            break;

          case 'r': // Å
            seq(c3)[1] = '\205';
            break;

          case '=': // Ā
            seq(c4)[1] = '\200';
            break;

          case 'u': // Ă
            seq(c4)[1] = '\202';
            break;

          case 'k': // Ą
            seq(c4)[1] = '\204';
            break;

          default:
            continue;
          }

          break;

        case 'C':
          switch (charComposing) {
          case 'c':
            // Ç
            seq(c3)[1] = '\207';
            break;

          case '\'': // Ć
            seq(c4)[1] = '\206';
            break;

          case '^': // Ĉ
            seq(c4)[1] = '\210';
            break;

          case '.': // Ċ
            seq(c4)[1] = '\212';
            break;

          case 'v': // Č
            seq(c4)[1] = '\214';

          default:
            continue;
          }

          break;

        case 'D':
          if (charComposing == 'v') {
            // Ď
            seq(c4)[1] = '\216';
            break;
          } else
            continue;

        case 'E':
          switch (charComposing) {
          case '`': // È
            seq(c3)[1] = '\210';
            break;

          case '\'': // É
            seq(c3)[1] = '\211';
            break;

          case '^': // Ê
            seq(c3)[1] = '\212';
            break;

          case '"': // Ë
            seq(c3)[1] = '\213';
            break;

          case '=': // Ē
            seq(c4)[1] = '\222';
            break;

          case 'u': // Ĕ
            seq(c4)[1] = '\224';
            break;

          case '.': // Ė
            seq(c4)[1] = '\226';
            break;

          case 'k': // Ę
            seq(c4)[1] = '\230';
            break;

          case 'v': // Ě
            seq(c4)[1] = '\232';
            break;

          default:
            continue;
          }

          break;

        case 'G':
          switch (charComposing) {
          case '^': // Ĝ
            seq(c4)[1] = '\234';
            break;

          case 'u': // Ğ
            seq(c4)[1] = '\236';
            break;

          case '.': // Ġ
            seq(c4)[1] = '\240';
            break;

          case 'c': // Ģ
            seq(c4)[1] = '\242';
            break;

          default:
            continue;
          }

          break;

        case 'H':
          if (charComposing == '^') {
            // Ĥ
            seq(c4)[1] = '\244';
            break;
          } else
            continue;

        case 'I':
          switch (charComposing) {
          case '`': // Ì
            seq(c3)[1] = '\214';
            break;

          case '\'': // Í
            seq(c3)[1] = '\215';
            break;

          case '^': // Î
            seq(c3)[1] = '\216';
            break;

          case '"': // Ï
            seq(c3)[1] = '\217';
            break;

          case '~': // Ĩ
            seq(c4)[1] = '\250';
            break;

          default:
            continue;
          }

          break;

        case 'J':
          if (charComposing == '^') {
            // Ĵ
            seq(c4)[1] = '\264';
            break;
          } else
            continue;

        case 'K':
          if (charComposing == 'c') {
            // Ķ
            seq(c4)[1] = '\266';
            break;
          } else
            continue;

        case 'L':
          switch (charComposing) {
          case '\'': // Ĺ
            seq(c4)[1] = '\271';
            break;

          case 'c': // Ļ
            seq(c4)[1] = '\273';
            break;

          case 'v': // Ľ
            seq(c4)[1] = '\275';
            break;

          default:
            continue;
          }

          break;

        case 'N':
          switch (charComposing) {
          case '~': // Ñ
            seq(c3)[1] = '\221';
            break;

          case '\'': // Ń
            seq(c5)[1] = '\203';
            break;

          case 'c': // Ņ
            seq(c5)[1] = '\205';
            break;

          case 'v': // Ň
            seq(c5)[1] = '\207';
            break;

          default:
            continue;
          }

          break;

        case 'O':
          switch (charComposing) {
          case '`': // Ò
            seq(c3)[1] = '\222';
            break;

          case '\'': // Ó
            seq(c3)[1] = '\223';
            break;

          case '^': // Ô
            seq(c3)[1] = '\224';
            break;

          case '~': // Õ
            seq(c3)[1] = '\225';
            break;

          case '"': // Ö
            seq(c3)[1] = '\226';
            break;

          case '=': // Ō
            seq(c5)[1] = '\214';
            break;

          case 'u': // Ŏ
            seq(c5)[1] = '\216';
            break;

          default:
            continue;
          }

          break;

        case 'R':
          switch (charComposing) {
          case '\'': // Ŕ
            seq(c5)[1] = '\224';
            break;

          case 'c': // Ŗ
            seq(c5)[1] = '\226';
            break;

          case 'v': // Ř
            seq(c5)[1] = '\230';
            break;

          default:
            continue;
          }

          break;

        case 'S':
          switch (charComposing) {
          case '\'': // Ś
            seq(c5)[1] = '\232';
            break;

          case '^': // Ŝ
            seq(c5)[1] = '\234';
            break;

          case 'c': // Ş
            seq(c5)[1] = '\236';
            break;

          case 'v': // Š
            seq(c5)[1] = '\240';
            break;

          default:
            continue;
          }

          break;

        case 'T':
          switch (charComposing) {
          case 'c': // Ţ
            seq(c5)[1] = '\242';
            break;

          case 'v': // Ť
            seq(c5)[1] = '\244';
            break;

          default:
            continue;
          }

          break;

        case 'U':
          switch (charComposing) {
          case '`': // Ù
            seq(c3)[1] = '\231';
            break;

          case '\'': // Ú
            seq(c3)[1] = '\232';
            break;

          case '^': // Û
            seq(c3)[1] = '\233';
            break;

          case '"': // Ü
            seq(c3)[1] = '\234';
            break;

          case '~': // Ũ
            seq(c5)[1] = '\250';
            break;

          case '=': // Ū
            seq(c5)[1] = '\252';
            break;

          case 'u': // Ŭ
            seq(c5)[1] = '\254';
            break;

          case 'r': // Ů
            seq(c5)[1] = '\256';
            break;

          case 'k': // Ų
            seq(c5)[1] = '\262';
            break;

          default:
            continue;
          }

          break;

        case 'W':
          if (charComposing == '^') {
            // Ŵ
            seq(c5)[1] = '\264';
            break;
          } else
            continue;

        case 'Y':
          switch (charComposing) {
          case '\'': // Ý
            seq(c3)[1] = '\235';
            break;

          case '^': // Ŷ
            seq(c5)[1] = '\266';
            break;

          case '"': // Ÿ
            seq(c5)[1] = '\270';
            break;

          default:
            continue;
          }

          break;

        case 'Z':
          switch (charComposing) {
          case '\'': // Ź
            seq(c5)[1] = '\271';
            break;

          case '.': // Ż
            seq(c5)[1] = '\273';
            break;

          case 'v': // Ž
            seq(c5)[1] = '\275';
            break;

          default:
            continue;
          }

          break;

        default:
          continue;
        }
      } else
        continue;
    }

    str.replace(i, 1, utf8seq, 2);
    charComposing = 0;
    ++i;
  }

  return str;
}

#define IMPORT_AUTHORS 0
#define IMPORT_ALL 1
#define IMPORT_PUBLICATIONS 2

#define NODES_TO_IMPORT "Authors;Authors & Publications;Publications"

class ImportBibTeX : public ImportModule {

public:
  PLUGININFORMATION("BibTeX", "Patrick Mary", "09/01/2014",
                    "<p>Supported extensions: bib</p><p>Import a co-authorship graph from a BibTeX "
                    "formatted file.</p>",
                    "1.1", "File")

  ImportBibTeX(const tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<string>(
        "file::filename", "This parameter indicates the pathname of the file(.bib) to import.", "");
    addInParameter<StringCollection>("Nodes to import",
                                     "The type of nodes to create: Authors <i>(Create nodes for "
                                     "authors only, publications are represented as edges between "
                                     "authors)</i><br/>Authors and Publications <i>(Create nodes "
                                     "for both authors and publications)</i><br/>Publications "
                                     "<i>(Create nodes for publications only)</i>",
                                     NODES_TO_IMPORT);
  }

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/import_bibtex.png";
  }

  std::list<std::string> fileExtensions() const {
    std::list<std::string> l;
    l.push_back("bib");
    return l;
  }

  bool importGraph() {
    string filename;
    int toImport = IMPORT_AUTHORS;

    if (dataSet) {
      dataSet->get<string>("file::filename", filename);
      tlp::StringCollection nodesToImport(NODES_TO_IMPORT);
      nodesToImport.setCurrent(toImport);

      if (dataSet->get("Nodes to import", nodesToImport))
        toImport = nodesToImport.getCurrent();
    }

    if (filename.empty()) {
      pluginProgress->setError("Filename is empty.");
      return false;
    }

    bool createAuthNodes = toImport != IMPORT_PUBLICATIONS;
    bool createPubliNodes = toImport != IMPORT_AUTHORS;

    // known properties to extract
    StringProperty *keyProp = graph->getProperty<StringProperty>("key");
    StringProperty *typeProp = graph->getProperty<StringProperty>("type");
    IntegerProperty *yearProp = graph->getProperty<IntegerProperty>("year");
    BooleanProperty *fromLabriProp =
        createAuthNodes ? graph->getProperty<BooleanProperty>("from LaBRI") : NULL;
    IntegerVectorProperty *labriAuthorsProp =
        createPubliNodes ? graph->getProperty<IntegerVectorProperty>("LaBRI authors") : NULL;
    StringVectorProperty *labriTeamsProp =
        createPubliNodes ? graph->getProperty<StringVectorProperty>("LaBRI teams") : NULL;
    StringVectorProperty *authProp =
        createPubliNodes ? graph->getProperty<StringVectorProperty>("authors") : NULL;
    StringProperty *authNameProp =
        createAuthNodes ? graph->getProperty<StringProperty>("name") : NULL;
    StringProperty *labriTeamProp =
        createAuthNodes ? graph->getProperty<StringProperty>("LaBRI team") : NULL;
    IntegerProperty *countProp =
        createAuthNodes ? graph->getProperty<IntegerProperty>("nbPublications") : NULL;

    // rendering properties
    ColorProperty *color = graph->getProperty<ColorProperty>("viewColor");
    StringProperty *icon = graph->getProperty<StringProperty>("viewIcon");
    StringProperty *label = graph->getProperty<StringProperty>("viewLabel");

    graph->getProperty<IntegerProperty>("viewLabelPosition")
        ->setAllNodeValue(LabelPosition::Bottom);
    graph->getProperty<IntegerProperty>("viewShape")
        ->setAllNodeValue(tlp::NodeShape::FontAwesomeIcon);

    TLP_HASH_MAP<std::string, node> authorsMap;
    TLP_HASH_MAP<std::string, bool> publisMap;

    try {
      xdkbib::File bibFile;

      // extract entries from BibTeX file
      bibFile.readFromFile(filename, xdkbib::File::StrictQuote);
      const std::vector<xdkbib::FileEntry> &entries = bibFile.entries();

      // first add nodes for publication
      vector<node> publis;

      if (createPubliNodes)
        graph->addNodes(entries.size(), publis);

      // loop on entries
      std::vector<xdkbib::FileEntry>::const_iterator it = entries.begin();
      std::vector<xdkbib::FileEntry>::const_iterator itEnd = entries.end();
      unsigned int i = 0;

      for (; it != itEnd; ++it, ++i) {
        xdkbib::FileEntry &fe = const_cast<xdkbib::FileEntry &>(*it);

        node publi;

        if (createPubliNodes)
          publi = publis[i];

        // set extracted properties
        std::string key = fe.key();

        // first check if publi does not already exist
        if (publisMap.find(key) != publisMap.end()) {
          --i;
          continue;
        }

        publisMap[key] = true;
        int year = atoi(fe.field("year").value().c_str());

        if (createPubliNodes) {
          // setup key, type and year
          keyProp->setNodeValue(publi, key);
          std::string type;
          std::transform(fe.type().begin(), fe.type().end(), type.begin(), ::tolower);
          typeProp->setNodeValue(publi, type);
          yearProp->setNodeValue(publi, year);
          // set rendering properties
          color->setNodeValue(publi, tlp::Color::Beige);
          label->setNodeValue(publi, key);
          icon->setNodeValue(publi, "fa-file-text");
        }

        // loop of entry fields
        std::map<std::string, xdkbib::Field>::const_iterator fit = fe.fields().begin();

        for (; fit != fe.fields().end(); ++fit) {
          string pName = fit->first;

          // year is already set
          if (pName == "year")
            continue;

          const xdkbib::Field &field = fit->second;
          bool isNumber =
              !field.valueParts().empty() && (field.valueParts()[0].type() == xdkbib::Number);
          bool isAuthor = (pName == "author");
          PropertyInterface *prop = NULL;

          if (createPubliNodes) {
            if (isNumber && (pName != "volume") && (pName != "number"))
              // create an IntegerProperty
              prop = graph->getProperty<IntegerProperty>(pName);
            else {
              if (isAuthor)
                prop = authProp;
              else
                // create a StringProperty
                prop = graph->getProperty<StringProperty>(pName);
            }
          }

          string value = fe.field(pName).value();

          if (createPubliNodes) {
            if (!isAuthor) {
              if (!isNumber)
                forceUtf8String(value);

              prop->setNodeStringValue(publi, value);
            }
          }

          if (isAuthor) {
            // author
            char *labriAuthors = NULL;
            char *labriTeams = NULL;
            std::string authorsComments;
            std::string teamsComments;
            vector<int> indices;
            // look for laBRI specific attributes in comments
            size_t pos = fe.comment().find("LaBRI: ");

            if (pos != string::npos) {
              authorsComments = fe.comment().substr(pos + 7);
              labriAuthors = const_cast<char *>(authorsComments.c_str());
              pos = fe.comment().find("LaBRI: ", pos + 7);

              if (pos != string::npos) {
                teamsComments = fe.comment().substr(pos + 7);
                labriTeams = const_cast<char *>(teamsComments.c_str());
              }
            }

            // get teams
            vector<string> teams;

            if (labriTeams) {
              char *teamsPtr;
              char *token = strtok_r(labriTeams, " \n", &teamsPtr);

              while (token) {
                teams.push_back(string(token));
                token = strtok_r(NULL, " \n", &teamsPtr);
              }

              if (createPubliNodes)
                labriTeamsProp->setNodeValue(publi, teams);
            }

            char *authorsPtr;
            unsigned int labriIndex =
                labriAuthors ? (atoi(strtok_r(labriAuthors, " \n", &authorsPtr)) - 1) : 0;
            unsigned int teamIndex = 0;
            // add authors
            xdkbib::AuthorList authors;
            authors.readFrom(fe.field("author").value().c_str());
            vector<string> authPropValue;
            vector<node> authorNodes;

            for (unsigned int j = 0; j < authors.size(); ++j) {
              bool labriAuthor = labriAuthors && j == labriIndex;

              if (labriAuthor) {
                indices.push_back(int(labriIndex));
                char *token = strtok_r(NULL, " \n", &authorsPtr);
                labriIndex = token ? (atoi(token) - 1) : 0;
              }

              xdkbib::Author &auth = authors[j];
              vector<std::string> firstNames = auth.first();
              string aName;
              string aKey;

              for (unsigned int k = 0; k < firstNames.size(); ++k) {
                string &firstName = firstNames[k];

                if (k) {
                  aName += " ";

                  if (labriAuthor == false)
                    aKey += " ";
                }

                aName += firstName;

                if (k == 0 || labriAuthor == false) {
                  // keep only the first letter and a dot for the key
                  firstName.resize(2);
                  firstName.replace(1, 1, ".");
                }

                aKey += firstName;
              }

              aName += " " + auth.lastJoin();
              forceUtf8String(aName);
              // author must be a unique person
              authPropValue.push_back(aName);

              if ((aName == "al. et") || (createAuthNodes == false))
                continue;

              aKey += " " + auth.lastJoin();
              forceUtf8String(aKey);

              // normalize aKey
              // remove any accented chars
              // we assume aKey is utf8 encoded
              for (unsigned int k = 0; k < aKey.size() - 1; ++k) {
                if (aKey[k] == '\303') {
                  switch (aKey[k + 1]) {
                  case '\200': // À
                  case '\201': // Á
                  case '\202': // Â
                  case '\203': // Ã
                  case '\204': // Ä
                  case '\205': // Å
                    aKey.replace(k, 2, "A");

                  case '\210': // È
                  case '\211': // É
                  case '\212': // Ê
                  case '\213': // Ë
                    aKey.replace(k, 2, "E");
                    continue;

                  case '\214': // Ì
                  case '\215': // Í
                  case '\216': // Î
                  case '\217': // Ï
                    aKey.replace(k, 2, "I");
                    continue;

                  case '\221': // Ñ
                    aKey.replace(k, 2, "N");
                    continue;

                  case '\222': // Ò
                  case '\223': // Ó
                  case '\224': // Ô
                  case '\225': // Õ
                  case '\226': // Ö
                  case '\230': // Ø
                    aKey.replace(k, 2, "O");
                    continue;

                  case '\231': // Ù
                  case '\232': // Ú
                  case '\233': // Û
                  case '\234': // Ü
                    aKey.replace(k, 2, "U");
                    continue;

                  case '\235': // Ý
                    aKey.replace(k, 2, "Y");
                    continue;

                  case '\237': // ß
                    aKey.replace(k, 2, "s");
                    continue;

                  case '\240': // à
                  case '\241': // á
                  case '\242': // â
                  case '\243': // ã
                  case '\244': // ä
                  case '\245': // å
                    aKey.replace(k, 2, "a");
                    continue;

                  case '\247': // ç
                    aKey.replace(k, 2, "c");
                    continue;

                  case '\250': // è
                  case '\251': // é
                  case '\252': // ê
                  case '\253': // ë
                    aKey.replace(k, 2, "e");
                    continue;

                  case '\254': // ì
                  case '\255': // í
                  case '\256': // î
                  case '\257': // ï
                    aKey.replace(k, 2, "i");
                    continue;

                  case '\261': // ñ
                    aKey.replace(k, 2, "n");
                    continue;

                  case '\262': // ò
                  case '\263': // ó
                  case '\264': // ô
                  case '\265': // õ
                  case '\266': // ö
                  case '\270': // ø
                    aKey.replace(k, 2, "o");
                    continue;

                  case '\271': // ù
                  case '\272': // ú
                  case '\273': // û
                  case '\274': // ü
                    aKey.replace(k, 2, "u");
                    continue;

                  case '\275': // ý
                  case '\276': // ÿ
                    aKey.replace(k, 2, "y");
                    continue;

                  default:
                    break;
                  }
                } else if (aKey[k] == '\304') {
                  switch (aKey[k + 1]) {
                  case '\200': // Ā
                  case '\202': // Ă
                  case '\204': // Ą
                    aKey.replace(k, 2, "A");
                    continue;

                  case '\201': // ā
                  case '\203': // ă
                  case '\205': // ą
                    aKey.replace(k, 2, "a");
                    continue;

                  case '\206': // Ć
                  case '\210': // Ĉ
                  case '\212': // Ċ
                  case '\214': // Č
                    aKey.replace(k, 2, "C");
                    continue;

                  case '\207': // ć
                  case '\211': // ĉ
                  case '\213': // ċ
                  case '\215': // č
                    aKey.replace(k, 2, "c");
                    continue;

                  case '\216': // Ď
                    aKey.replace(k, 2, "D");
                    continue;

                  case '\217': // ď
                    aKey.replace(k, 2, "d");
                    continue;

                  case '\220': // Đ
                    aKey.replace(k, 2, "D");
                    continue;

                  case '\221': // đ
                    aKey.replace(k, 2, "d");
                    continue;

                  case '\222': // Ē
                  case '\224': // Ĕ
                  case '\226': // Ė
                  case '\230': // Ę
                  case '\232': // Ě
                    aKey.replace(k, 2, "E");
                    continue;

                  case '\223': // ë
                  case '\225': // ĕ
                  case '\227': // ė
                  case '\231': // ę
                  case '\233': // ě
                    aKey.replace(k, 2, "e");
                    continue;

                  case '\234': // Ĝ
                  case '\236': // Ğ
                  case '\240': // Ġ
                  case '\242': // Ģ
                    aKey.replace(k, 2, "G");
                    continue;

                  case '\235': // ĝ
                  case '\237': // ğ
                  case '\241': // ġ
                  case '\243': // ģ
                    aKey.replace(k, 2, "g");
                    continue;

                  case '\245': // ĥ
                  case '\247': // ħ
                    aKey.replace(k, 2, "h");
                    continue;

                  case '\244': // Ĥ
                  case '\246': // Ħ
                    aKey.replace(k, 2, "H");
                    continue;

                  case '\250': // Ĩ
                    aKey.replace(k, 2, "I");
                    continue;

                  case '\251': // ĩ
                  case '\253': // ī
                  case '\255': // ĭ
                    aKey.replace(k, 2, "i");
                    continue;

                  case '\264': // Ĵ
                    aKey.replace(k, 2, "J");
                    continue;

                  case '\265': // ĵ
                    aKey.replace(k, 2, "j");
                    continue;

                  case '\266': // Ķ
                    aKey.replace(k, 2, "K");
                    continue;

                  case '\270': // ķ
                    aKey.replace(k, 2, "k");
                    continue;

                  case '\271': // Ĺ
                  case '\273': // Ļ
                  case '\275': // Ľ
                    aKey.replace(k, 2, "G");
                    continue;

                  case '\272': // ĺ
                  case '\274': // ļ
                  case '\276': // ľ
                    aKey.replace(k, 2, "l");
                    continue;

                  default:
                    break;
                  }
                } else if (aKey[k] == '\305') {
                  switch (aKey[k + 1]) {
                  case '\201': // Ł
                    continue;
                    aKey.replace(k, 2, "L");

                  case '\202': // ł
                    continue;
                    aKey.replace(k, 2, "l");

                  case '\204': // ń
                  case '\206': // ņ
                  case '\210': // ň
                    aKey.replace(k, 2, "n");
                    continue;

                  case '\215': // ō
                  case '\217': // ŏ
                  case '\221': // ő
                    aKey.replace(k, 2, "o");
                    continue;

                  case '\220': // Ő
                    aKey.replace(k, 2, "0");
                    continue;

                  case '\225': // ŕ
                  case '\227': // ŗ
                  case '\230': // ř
                    aKey.replace(k, 2, "r");
                    continue;

                  case '\233': // ś
                  case '\235': // ŝ
                  case '\237': // ş
                  case '\241': // š
                    aKey.replace(k, 2, "s");
                    continue;

                  case '\243': // ţ
                  case '\245': // ť
                    aKey.replace(k, 2, "t");
                    continue;

                  case '\246': // Ŧ
                    aKey.replace(k, 2, "T");
                    continue;

                  case '\251': // ũ
                  case '\253': // ū
                  case '\255': // ŭ
                  case '\257': // ů
                  case '\261': // ű
                  case '\263': // ų
                    aKey.replace(k, 2, "u");
                    continue;

                  case '\260': // Ű
                    aKey.replace(k, 2, "U");
                    continue;

                  case '\265': // ŵ
                    aKey.replace(k, 2, "w");
                    continue;

                  case '\267': // ŷ
                    aKey.replace(k, 2, "y");
                    continue;

                  case '\272': // ź
                  case '\274': // ż
                  case '\276': // ž
                    aKey.replace(k, 2, "z");
                    continue;

                  default:
                    break;
                  }
                }
              }

              node author;
              // check if the author already exists
              TLP_HASH_MAP<std::string, node>::const_iterator itAuth = authorsMap.find(aKey);

              if (itAuth != authorsMap.end()) {
                authorNodes.push_back(author = itAuth->second);
                countProp->setNodeValue(author, countProp->getNodeValue(author) + 1);
              } else {
                authorNodes.push_back(author = graph->addNode());
                authorsMap[aKey] = author;
                authNameProp->setNodeValue(author, aName);

                if (labriAuthor && labriTeams)
                  labriTeamProp->setNodeValue(author, teams[teamIndex]);

                label->setNodeValue(author, aName);
                icon->setNodeValue(author, "fa-user");
                countProp->setNodeValue(author, 1);
              }

              if (labriAuthor) {
                fromLabriProp->setNodeValue(author, true);
                color->setNodeValue(author, tlp::Color::Aquamarine);

                if (labriTeams)
                  ++teamIndex;
              }

              if (createPubliNodes) {
                // add edge from author to publi
                edge e = graph->addEdge(author, publi);
                // set year
                yearProp->setEdgeValue(e, year);
              }
            }

            if (createPubliNodes) {
              // store authors
              authProp->setNodeValue(publi, authPropValue);
              // store Labri authors
              labriAuthorsProp->setNodeValue(publi, indices);
            } else {
              // display a warning for publication without author
              if (authorNodes.empty()) {
                tlp::warning() << "Warning: found the publication '" << key.c_str()
                               << "' without author" << std::endl;
                // stop processing publication fields
                break;
              }

              // create edge between the authors of the publications
              if (authorNodes.size() == 1) {
                // create a loop to record publication information
                edge e = graph->addEdge(authorNodes[0], authorNodes[0]);
                // setup key, type and year
                keyProp->setEdgeValue(e, key);
                typeProp->setEdgeValue(e, fe.type());
                yearProp->setEdgeValue(e, year);
              } else {
                for (unsigned int j = 0; j < authorNodes.size() - 1; ++j) {
                  for (unsigned int k = j + 1; k < authorNodes.size(); ++k) {
                    edge e = graph->addEdge(authorNodes[j], authorNodes[k]);
                    // setup key, type and year
                    keyProp->setEdgeValue(e, key);
                    typeProp->setEdgeValue(e, fe.type());
                    yearProp->setEdgeValue(e, year);
                  }
                }
              }
            }
          }
        }
      }

      // in case of duplicate
      // delete unused publi nodes
      if (createPubliNodes) {
        for (; i < entries.size(); ++i)
          graph->delNode(publis[i]);
      }
    } catch (xdkbib::parsing_error &e) {
      stringstream sstr;
      sstr << "BibTeX file parsing error at char " << e.column() << " of line " << e.line() << ": "
           << e.what() << std::endl;
      tlp::error() << sstr.str();
      pluginProgress->setError(sstr.str());
    }

    // remove unused properties
    if (createPubliNodes) {
      if (labriTeamsProp->numberOfNonDefaultValuatedNodes() == 0)
        graph->delLocalProperty(labriTeamsProp->getName());

      if (labriAuthorsProp->numberOfNonDefaultValuatedNodes() == 0)
        graph->delLocalProperty(labriAuthorsProp->getName());
    }

    // layout graph with a bubble tree
    if (createAuthNodes) {
      // delete labri specific properties if not used
      if (labriTeamProp->numberOfNonDefaultValuatedNodes() == 0)
        graph->delLocalProperty(labriTeamProp->getName());

      if (fromLabriProp->numberOfNonDefaultValuatedNodes() == 0)
        graph->delLocalProperty(fromLabriProp->getName());

      string err;
      return graph->applyPropertyAlgorithm(
          "FM^3 (OGDF)", graph->getProperty<LayoutProperty>("viewLayout"), err, pluginProgress);
    }

    return true;
  }
};

PLUGIN(ImportBibTeX)
