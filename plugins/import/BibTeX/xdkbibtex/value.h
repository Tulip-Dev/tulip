// Copyright (C) 2004 Xavier Décoret <Xavier.Decoret@imag.fr>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef XDKBIB_VALUE_H
#define XDKBIB_VALUE_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <xdkbibtex/base.h>

#include <string>
#include <vector>
#include <iostream>
#include <map>

class valueparser;

namespace xdkbib {
class Text;
class Word;
class Letter;
class SingleLetter;
class PseudoLetter;
class TokenLetter;
class Dictionary;

typedef enum {
  Raw             = 0,
  BracesStripped  = 1,
  CommandStripped = 2
}
ContentOptions;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Text
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Text {
public:
  Text();
  Text* clone() const;
  Text& operator=(const Text&);
  Text(const Text&);
  ~Text();

  void readFrom(const std::string& t,const std::string& split="")
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
  throw (xdkbib::parsing_error)
#endif
  ;

  std::string content(ContentOptions options=Raw) const;
  bool hasContent(const std::string& s,ContentOptions options=Raw) const;

  unsigned int nbWords() const;
  const Word* operator[](unsigned int i) const;
  const std::vector<Word*>& words() const;

  Word* add(Word* w=NULL);
  void clear();
  void translate(const Dictionary& dict);
protected:
  friend class ::valueparser;
private:
  std::vector<Word*> words_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Word
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Word {
public:
  Word();
  Word(const Word& w);
  Word& operator=(const Word& w);
  ~Word();
  Word* clone() const;

  std::string content(ContentOptions options=Raw) const;
  bool hasContent(const std::string& s,ContentOptions options=Raw) const;

  unsigned int nbLetters() const;
  const Letter* operator[](unsigned int i) const;
  const std::vector<Letter*>& letters() const;

  Letter* add(Letter* w);
  void clear();

  bool hasPseudoLetters() const;
protected:
  friend class ::valueparser;
  friend class Text;
private:
  std::vector<Letter*> letters_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Word
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Letter {
public:
  virtual ~Letter() {}
  virtual Letter* clone() const = 0;

  virtual std::string content(ContentOptions options=Raw) const = 0;
  bool hasContent(const std::string& s,ContentOptions options=Raw) const;

  virtual bool isSingle() const;
  virtual bool isPseudo() const;
  virtual bool isToken() const;

  virtual const SingleLetter* asSingle() const;
  virtual const PseudoLetter* asPseudo() const;
  virtual const TokenLetter* asToken() const;

  virtual bool isTheSingle(char) const;
  virtual bool isTheToken(const std::string& s) const;

protected:
  friend class Text;
  friend class Word;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface or SingleLetter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SingleLetter : public Letter {
public:
  SingleLetter(char c);
  virtual Letter* clone() const;
  char character() const;

  virtual std::string content(ContentOptions options) const;

  virtual bool isSingle() const;
  virtual const SingleLetter* asSingle() const;
  virtual bool isTheSingle(char) const;
protected:
  friend class ::valueparser;
  friend class Text;
  friend class Word;
private:
  char c_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface or PseudoLetter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class PseudoLetter : public Letter {
public:
  PseudoLetter();
  virtual Letter* clone() const;
  virtual ~PseudoLetter();
  virtual const Text* text() const;

  virtual std::string content(ContentOptions options) const;

  virtual bool isPseudo() const;
  virtual const PseudoLetter* asPseudo() const;
protected:
  friend class ::valueparser;
  friend class Text;
  friend class Word;
private:
  Text* text_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface or TokenLetter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TokenLetter : public Letter {
public:
  TokenLetter(const std::string&);
  virtual Letter* clone() const;
  const std::string& token() const;

  virtual std::string content(ContentOptions options) const;

  virtual bool isToken() const;
  virtual const TokenLetter* asToken() const;
  virtual bool isTheToken(const std::string& s) const;
protected:
  friend class ::valueparser;
  friend class Text;
  friend class Word;
private:
  std::string token_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Dictionary
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Dictionary : public std::map<std::pair<std::string,std::string>,std::string> {
public:
};
extern const Dictionary& isoaccents();
}

#endif // XDKBIB_VALUE_H
