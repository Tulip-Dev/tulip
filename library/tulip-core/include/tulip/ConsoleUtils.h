/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef CONSOLE_UTILS
#define CONSOLE_UTILS

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <map>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#endif

#if defined(__unix__) || defined(__APPLE__)

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

static struct termios stored_settings;

static void setConsoleSettingsForAnsiRequest(void) {
  struct termios new_settings;
  tcgetattr(0, &stored_settings);
  new_settings = stored_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &new_settings);
}

static void resetConsoleSettings(void) {
  tcsetattr(0, TCSANOW, &stored_settings);
}

static bool processInForeground(void) {
  pid_t fg = tcgetpgrp(STDIN_FILENO);
  return fg == getpgrp();
}


static std::pair<int, int> getConsoleCursorPosition() {
  int row=0, col=0;

  if (isatty(fileno(stdin)) && processInForeground()) {
    setConsoleSettingsForAnsiRequest();
    fprintf(stdout, "\x1b[6n");

    if (fscanf(stdin, "\x1b[%d;%dR", &row, &col)) {}

    resetConsoleSettings();
  }

  return std::make_pair(row, col);
}

static std::pair<int, int> getConsoleSize() {
  int rows=-1, cols=-1;
  struct winsize ws;

  if (ioctl(0,TIOCGWINSZ,&ws)==0) {
    rows = ws.ws_row;
    cols =  ws.ws_col;
  }

  return std::make_pair(rows, cols);
}

#elif defined(WIN32)

inline std::pair<int, int> getConsoleCursorPosition() {
  int row=0, col=0;
  CONSOLE_SCREEN_BUFFER_INFO SBInfo;

  if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo)) {
    row = SBInfo.dwCursorPosition.Y;
    col = SBInfo.dwCursorPosition.X;
  }

  return std::make_pair(row, col);
}

inline std::pair<int, int> getConsoleSize() {
  int rows=200, cols=200;
  CONSOLE_SCREEN_BUFFER_INFO SBInfo;

  if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo)) {
    rows = SBInfo.dwSize.Y;
    cols = SBInfo.dwSize.X-1;
  }

  return std::make_pair(rows, cols);
}

#endif

enum TextEffect {NORMAL=1, BOLD=2, UNDERLINED=4, BLINK=8};

enum TextColor {BLACK, RED, GREEN, YELLOW, BLUE,
                MAGENTA, CYAN, LIGHT_GRAY, DARK_GRAY,
                LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW,
                LIGHT_BLUE, LIGHT_MAGENTA, LIGHT_CYAN, WHITE
               };

class TextFgColorSetup {

public :

  TextFgColorSetup(TextColor color) :
    color(color) {}

  TextColor color;
};

class TextBgColorSetup {

public :

  TextBgColorSetup(TextColor color) :
    color(color) {}

  TextColor color;
};

class TextEffectSetup {

public :

  TextEffectSetup(int effect) :
    effect(effect) {}

  int effect;
};



static std::map<TextColor, std::string> initAnsiFgColors() {
  const char *colorterm = getenv("COLORTERM");
  bool rxvt = colorterm && std::string(colorterm).find("rxvt") != std::string::npos;
  std::map<TextColor, std::string> ret;
  ret[BLACK] = "30";
  ret[RED] = "31";
  ret[GREEN] = "32";
  ret[YELLOW] = "33";
  ret[BLUE] = "34";
  ret[MAGENTA] = "35";
  ret[CYAN] = "36";
  ret[LIGHT_GRAY] = "37";
  ret[DARK_GRAY] = rxvt ? ret[BLACK] : "90";
  ret[LIGHT_RED] = rxvt ? ret[RED] : "91";
  ret[LIGHT_GREEN] = rxvt ? ret[GREEN] : "92";
  ret[LIGHT_YELLOW] = rxvt ? ret[YELLOW] : "93";
  ret[LIGHT_BLUE] = rxvt ? ret[BLUE] : "94";
  ret[LIGHT_MAGENTA] = rxvt ? ret[MAGENTA] : "95";
  ret[LIGHT_CYAN] = rxvt ? ret[CYAN] : "96";
  ret[WHITE] = rxvt ? ret[LIGHT_GRAY] : "97";
  return ret;
}

static std::map<TextColor, std::string> initAnsiBgColors() {
  const char *colorterm = getenv("COLORTERM");
  bool rxvt = colorterm && std::string(colorterm).find("rxvt") != std::string::npos;
  std::map<TextColor, std::string> ret;
  ret[BLACK] = "40";
  ret[RED] = "41";
  ret[GREEN] = "42";
  ret[YELLOW] = "43";
  ret[BLUE] = "44";
  ret[MAGENTA] = "45";
  ret[CYAN] = "46";
  ret[LIGHT_GRAY] = "47";
  ret[DARK_GRAY] = rxvt ? ret[BLACK] : "100";
  ret[LIGHT_RED] = rxvt ? ret[RED] : "101";
  ret[LIGHT_GREEN] = rxvt ? ret[GREEN] : "102";
  ret[LIGHT_YELLOW] = rxvt ? ret[YELLOW] : "103";
  ret[LIGHT_BLUE] = rxvt ? ret[BLUE] : "104";
  ret[LIGHT_MAGENTA] = rxvt ? ret[MAGENTA] : "105";
  ret[LIGHT_CYAN] = rxvt ? ret[CYAN] : "106";
  ret[WHITE] = rxvt ? ret[LIGHT_GRAY] : "107";
  return ret;
}

static std::map<TextColor, std::string> ansiFgColors = initAnsiFgColors();
static std::map<TextColor, std::string> ansiBgColors = initAnsiBgColors();

#ifdef WIN32
static std::map<TextColor, int> initCrtFgColors() {
  std::map<TextColor, int> ret;
  ret[BLACK] = 0;
  ret[RED] = FOREGROUND_RED;
  ret[GREEN] = FOREGROUND_GREEN;
  ret[YELLOW] = FOREGROUND_RED | FOREGROUND_GREEN;
  ret[BLUE] = FOREGROUND_BLUE;
  ret[MAGENTA] = FOREGROUND_BLUE | FOREGROUND_RED;
  ret[CYAN] = FOREGROUND_GREEN | FOREGROUND_BLUE;
  ret[LIGHT_GRAY]= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
  ret[DARK_GRAY] = FOREGROUND_INTENSITY;
  ret[LIGHT_RED] = FOREGROUND_RED | FOREGROUND_INTENSITY;
  ret[LIGHT_GREEN] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
  ret[LIGHT_YELLOW] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
  ret[LIGHT_BLUE] = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  ret[LIGHT_MAGENTA] = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
  ret[LIGHT_CYAN] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  ret[WHITE] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  return ret;
}

static std::map<TextColor, int> initCrtBgColors() {
  std::map<TextColor, int> ret;
  ret[BLACK] = 0;
  ret[RED] = BACKGROUND_RED;
  ret[GREEN] = BACKGROUND_GREEN;
  ret[YELLOW] = BACKGROUND_RED | BACKGROUND_GREEN;
  ret[BLUE] = BACKGROUND_BLUE;
  ret[MAGENTA] = BACKGROUND_BLUE | BACKGROUND_RED;
  ret[CYAN] = BACKGROUND_GREEN | BACKGROUND_BLUE;
  ret[LIGHT_GRAY]= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
  ret[DARK_GRAY] = BACKGROUND_INTENSITY;
  ret[LIGHT_RED] = BACKGROUND_RED | BACKGROUND_INTENSITY;
  ret[LIGHT_GREEN] = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
  ret[LIGHT_YELLOW] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
  ret[LIGHT_BLUE] = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  ret[LIGHT_MAGENTA] = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
  ret[LIGHT_CYAN] = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  ret[WHITE] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  return ret;
}

static WORD getCurrentBgColor() {
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(hStdout, &info);
  return info.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}

static WORD getCurrentFgColor() {
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(hStdout, &info);
  return info.wAttributes & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
static std::map<TextColor, int> crtFgColors = initCrtFgColors();
static std::map<TextColor, int> crtBgColors = initCrtBgColors();
#endif

static void escapeAnsiCode(std::ostream &os, const std::string &ansiCode, const std::string &endEscape ="m") {
  static const char *stdOutNoAnsiEscapes = getenv("STDOUT_NO_ANSI_ESCAPES");
  static const char *stdErrNoAnsiEscapes = getenv("STDERR_NO_ANSI_ESCAPES");
#ifndef WIN32

  if ((&os == &std::cout && !stdOutNoAnsiEscapes && isatty(fileno(stdout))) ||
      (&os == &std::cerr && !stdErrNoAnsiEscapes && isatty(fileno(stderr))))
#else
  if ((&os == &std::cout && !stdOutNoAnsiEscapes) ||
      (&os == &std::cerr && !stdErrNoAnsiEscapes))
#endif
    os << "\x1b[" << ansiCode << endEscape;
}

/*
  This set of stream manipulation operators allow to produce
  a colored console ouput. Except for windows console application (based on cmd.exe),
  the colors are activated through ANSI escape sequences. The writing of these sequences
  can be disabled by setting the STDOUT_N0_ANSI_ESCAPES and STDERR_N0_ANSI_ESCAPES
  environment variables (for instance, when working with a terminal who does not understand
  these sequences).

  Below is an example on how to use it :

  // Output a bold text colored in red on a white background
  std::cout << setTextEffect(BOLD) << setTextColor(RED) << setTextBackgroundColor(WHITE) << "Hello World!" << std::endl;

  // Reset the text colors to the default ones
  std::cout << defaultTextColor;

  // You can also use some predefined color schemes (only affects text color) : black, white, red, green,
  // blue, yellow, white, cyan, magenta, darkGray, lightGray, lightRed, lightGreen, ...
  std::cout << red << "Hello " << blue << World !!" << defaulTextColor << std::endl;

 */

inline std::ostream& defaultTextColor(std::ostream &s) {
  if (&s == &std::cout || &s == &std::cerr) {
#ifndef WIN32
    escapeAnsiCode(s, "0");
    escapeAnsiCode(s, "39");
    escapeAnsiCode(s, "49");
#else
    // on windows, if the TERM environment variable is not defined
    // it means that the shell used is cmd.exe, whose does not understand ANSI escape sequences
    static const char *term = getenv("TERM");

    if (term && std::string(term) != "cygwin") {
      escapeAnsiCode(s, "0");
      escapeAnsiCode(s, "39");
      escapeAnsiCode(s, "49");
    }
    else {
      HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hStdout, crtBgColors[BLACK] | crtFgColors[WHITE]);
    }

#endif
  }

  return s;
}

inline std::ostream& operator<<(std::ostream &s, const TextFgColorSetup &tgfcs) {
  if (&s == &std::cout || &s == &std::cerr) {
#ifndef WIN32
    escapeAnsiCode(s, ansiFgColors[tgfcs.color]);
#else
    // on windows, if the TERM environment variable is not defined
    // it means that the shell used is cmd.exe, whose does not understand ANSI escape sequences
    static const char *term = getenv("TERM");

    if (term && std::string(term) != "cygwin") {
      escapeAnsiCode(s, ansiFgColors[tgfcs.color]);
    }
    else {
      HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hStdout, crtFgColors[tgfcs.color] | getCurrentBgColor());
    }

#endif
  }

  return s;
}

inline std::ostream& operator<<(std::ostream &s, const TextBgColorSetup &tbgcs) {
  if (&s == &std::cout || &s == &std::cerr) {
#ifndef WIN32
    escapeAnsiCode(s, ansiBgColors[tbgcs.color]);
#else
    // on windows, if the TERM environment variable is not defined
    // it means that the shell used is cmd.exe, whose does not understand ANSI escape sequences
    static const char *term = getenv("TERM");

    if (term && std::string(term) != "cygwin") {
      escapeAnsiCode(s, ansiBgColors[tbgcs.color]);
    }
    else {
      HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hStdout, getCurrentFgColor() | crtBgColors[tbgcs.color]);
    }

#endif
  }

  return s;
}

inline void setEffects(std::ostream &s, const int &effect) {
  if (effect & NORMAL)
    escapeAnsiCode(s, "0");

  if (effect & BOLD)
    escapeAnsiCode(s, "1");

  if (effect & UNDERLINED)
    escapeAnsiCode(s, "4");

  if (effect & BLINK)
    escapeAnsiCode(s, "5");
}

inline std::ostream& operator<<(std::ostream &s, const TextEffectSetup &tes) {
  if (&s == &std::cout || &s == &std::cerr) {
#ifndef WIN32
    setEffects(s, tes.effect);
#else
    static const char *term = getenv("TERM");

    if (term && std::string(term) != "cygwin") {
      setEffects(s, tes.effect);
    }

#endif
  }

  return s;
}

inline TextBgColorSetup setTextBackgroundColor(TextColor color) {
  return TextBgColorSetup(color);
}

inline TextFgColorSetup setTextColor(TextColor color) {
  return TextFgColorSetup(color);
}

inline TextEffectSetup setTextEffect(int effect) {
  return TextEffectSetup(effect);
}

inline std::ostream& bold(std::ostream& s) {
  return s << setTextEffect(BOLD);
}

inline std::ostream& underlined(std::ostream& s) {
  return s << setTextEffect(UNDERLINED);
}

inline std::ostream& red(std::ostream& s) {
  return s << setTextColor(RED);
}

inline std::ostream& green(std::ostream& s) {
  return s << setTextColor(GREEN);
}

inline std::ostream& blue(std::ostream& s) {
  return s << setTextColor(BLUE);
}

inline std::ostream& yellow(std::ostream& s) {
  return s << setTextColor(YELLOW);
}

inline std::ostream& white(std::ostream& s) {
  return s << setTextColor(WHITE);
}

inline std::ostream& black(std::ostream& s) {
  return s << setTextColor(BLACK);
}

inline std::ostream& magenta(std::ostream& s) {
  return s << setTextColor(MAGENTA);
}

inline std::ostream& cyan(std::ostream& s) {
  return s << setTextColor(CYAN);
}

inline std::ostream& lightRed(std::ostream& s) {
  return s << setTextColor(LIGHT_RED);
}

inline std::ostream& lightGreen(std::ostream& s) {
  return s << setTextColor(LIGHT_GREEN);
}

inline std::ostream& lightBlue(std::ostream& s) {
  return s << setTextColor(LIGHT_BLUE);
}

inline std::ostream& lightYellow(std::ostream& s) {
  return s << setTextColor(LIGHT_YELLOW);
}

inline std::ostream& lightGray(std::ostream& s) {
  return s << setTextColor(LIGHT_GRAY);
}

inline std::ostream& darkGray(std::ostream& s) {
  return s << setTextColor(DARK_GRAY);
}

inline std::ostream& lightMagenta(std::ostream& s) {
  return s << setTextColor(LIGHT_MAGENTA);
}

inline std::ostream& lightCyan(std::ostream& s) {
  return s << setTextColor(LIGHT_CYAN);
}

// Fill the current line of the terminal with space characters from the
// current cursor position to the end of the line.
// The purpose is to have a constant background color on the line.
inline std::ostream& fillToEndOfLine(std::ostream& s) {
#ifndef WIN32

  if (processInForeground() && ((&s == &std::cout && isatty(fileno(stdout))) || (&s == &std::cerr && isatty(fileno(stderr))))) {
#endif
    std::pair<int, int> cursorPos = getConsoleCursorPosition();
    std::pair<int, int> consoleSize = getConsoleSize();

    if ((cursorPos.second + consoleSize.second) != 0) {
      for (int i = cursorPos.second ; i <= consoleSize.second ; ++i) {
        s << " ";
      }
    }
    else {
      s << std::endl;
    }

#ifndef WIN32
  }

#endif

  if ((&s == &std::cout && !isatty(fileno(stdout))) || (&s == &std::cerr && !isatty(fileno(stderr)))) {
    s << std::endl;
  }

#ifndef WIN32

  if (!processInForeground()) {
    s << defaultTextColor << std::endl;
  }

#endif

  return s;
}

#endif
///@endcond
