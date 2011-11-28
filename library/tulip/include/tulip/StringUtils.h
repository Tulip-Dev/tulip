/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
 
 #ifndef STRING_UTILS
 #define STRING_UTILS
 
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>

#ifdef WIN32
#include <windows.h>
#endif

enum FontType {NORMAL=0, BOLD=1};

enum TextColor {BLACK=0, RED=1, GREEN=2, YELLOW=3, BLUE=4,
                MAGENTA=5, CYAN=6, WHITE=7};

class TextColorSetup {
	
	public :
	
		TextColorSetup(FontType ft, TextColor fg, TextColor bg) :
			fontType(ft), fgColor(fg), bgColor(bg) {}
	
		FontType fontType;
		TextColor fgColor;
		TextColor bgColor;
};			   
			   
static std::map<TextColor, std::string> initAnsiFgColors() {
	std::map<TextColor, std::string> ret;
	ret[BLACK] = "30";
	ret[RED] = "31";
	ret[GREEN] = "32";
	ret[YELLOW] = "33";
	ret[BLUE] = "34";
	ret[MAGENTA] = "35";
	ret[CYAN] = "36";
	ret[WHITE] = "37";
	return ret;
}

static std::map<TextColor, std::string> initAnsiBgColors() {
	std::map<TextColor, std::string> ret;
	ret[BLACK] = "40";
	ret[RED] = "41";
	ret[GREEN] = "42";
	ret[YELLOW] = "43";
	ret[BLUE] = "44";
	ret[MAGENTA] = "45";
	ret[CYAN] = "46";
	ret[WHITE] = "47";
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
	ret[WHITE] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	return ret;
}

static std::map<TextColor, int> initCrtBgColors() {
	std::map<TextColor, int> ret;
	ret[BLACK] = 0;
	ret[RED] = BACKGROUND_RED | BACKGROUND_INTENSITY;
	ret[GREEN] = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	ret[YELLOW] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	ret[BLUE] = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	ret[MAGENTA] = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
	ret[CYAN] = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	ret[WHITE] = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	return ret;
}
static std::map<TextColor, int> crtFgColors = initCrtFgColors();
static std::map<TextColor, int> crtBgColors = initCrtBgColors();
#endif			   

/*
	This set of stream manipulation operators allow to produce
	a colored console ouput.
	
	Below is an example on how to use it :
	
	// Output a bold text colored in red on a white background
	std::cout << setTextColor(BOLD, RED, WHITE) << "Hello World!" << std::endl;

	// Reset the text colors to the default ones
	std::cout << defaultTextColor;
	
	// You can also use some predefined color schemes : red, green, blue, yellow, white, cyan or magenta (bold text on black background)
	std::cout << red << "Hello " << blue << World !!" << defaulTextColor << std::endl;
	
*/
			   
inline std::ostream& defaultTextColor(std::ostream &s) {
  if (s == std::cout || s == std::cerr) {
#ifndef WIN32
	s << "\x1b[0m";
#else 
    // on windows, if the TERM environment variable is not defined
	// it means that the shell used is cmd.exe, whose does not understand ANSI escape sequences 
	static const char *term = getenv("TERM");
	if (term) {
		s << "\x1b[0m";
	} else {
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(hStdout, crtFgColors[WHITE]);	
	}
#endif  
  }

  return s;
}

inline std::ostream& operator<<(std::ostream &s, const TextColorSetup &tcs) {
	if (s == std::cout || s == std::cerr) {
  #ifndef WIN32
    s << "\x1b[" << tcs.fontType << "m\x1b[" << ansiFgColors[tcs.fgColor] << "m\x1b[" << ansiBgColors[tcs.bgColor] << "m";
  #else 
	// on windows, if the TERM environment variable is not defined
	// it means that the shell used is cmd.exe, whose does not understand ANSI escape sequences
	static const char *term = getenv("TERM");
	if (term) {
		s << "\x1b[" << tcs.fontType << "m\x1b[" << ansiFgColors[tcs.fgColor] << "m\x1b[" << ansiBgColors[tcs.bgColor] << "m";
	} else {
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		int crtFg = crtFgColors[tcs.fgColor];
		if (tcs.fontType == BOLD)
			crtFg |= FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hStdout, crtFg | crtBgColors[tcs.bgColor]);	
	}
  #endif  
  }
  return s;
}

inline TextColorSetup setTextColor(FontType ft, TextColor fg, TextColor bg) {
  return TextColorSetup(ft, fg, bg);
}

inline std::ostream& red(std::ostream& s) {
  return s << setTextColor(BOLD, RED, BLACK);
}

inline std::ostream& green(std::ostream& s) {
  return s << setTextColor(BOLD, GREEN, BLACK);
}

inline std::ostream& blue(std::ostream& s) {
  return s << setTextColor(BOLD, BLUE, BLACK);
}

inline std::ostream& yellow(std::ostream& s) {
  return s << setTextColor(BOLD, YELLOW, BLACK);
}

inline std::ostream& white(std::ostream& s) {
  return s << setTextColor(BOLD, WHITE, BLACK);
}

inline std::ostream& magenta(std::ostream& s) {
  return s << setTextColor(BOLD, MAGENTA, BLACK);
}

inline std::ostream& cyan(std::ostream& s) {
  return s << setTextColor(BOLD, CYAN, BLACK);
} 
 

 #endif