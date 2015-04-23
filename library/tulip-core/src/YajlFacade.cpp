/**
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

#include <tulip/YajlFacade.h>
#include <tulip/TlpTools.h>

extern "C" {
#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>
}
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

YajlParseFacade::YajlParseFacade(tlp::PluginProgress* progress) : _progress(progress), _parsingSucceeded(true) {}

static int parse_null(void *ctx) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseNull();
  return 1;
}

static int parse_boolean(void * ctx, int boolVal) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseBoolean(boolVal);
  return 1;
}

static int parse_integer(void *ctx, long long integerVal) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseInteger(integerVal);
  return 1;
}

static int parse_double(void *ctx, double doubleVal) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseDouble(doubleVal);
  return 1;
}

// TODO check in which case this should be used instead of parse_integer and parse_double
// static int parse_number(void * ctx, const char * numberVal, size_t numberLen) {
//   YajlFacade* facade = (YajlFacade*) ctx;
//   facade->parseNumber(numberVal, numberLen);
//   return 1;
// }

static int parse_string(void *ctx, const unsigned char * stringVal, size_t stringLen) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  std::string key((char*)stringVal, stringLen);
  facade->parseString(key);
  return 1;
}

static int parse_map_key(void *ctx, const unsigned char * stringVal, size_t stringLen) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  std::string key((char*)stringVal, stringLen);
  facade->parseMapKey(key);
  return 1;
}

static int parse_start_map(void *ctx) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseStartMap();
  return 1;
}

static int parse_end_map(void *ctx) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseEndMap();
  return 1;
}

static int parse_start_array(void *ctx) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseStartArray();
  return 1;
}

static int parse_end_array(void *ctx) {
  YajlParseFacade* facade = (YajlParseFacade*) ctx;
  facade->parseEndArray();
  return 1;
}

void YajlParseFacade::parse(std::string filename) {
  // check if file exists
  tlp_stat_t infoEntry;
  bool result = (tlp::statPath(filename,&infoEntry) == 0);

  if (!result) {
    std::stringstream ess;
    ess << filename.c_str() << ": " << strerror(errno);
    _errorMessage = ess.str();
    _parsingSucceeded = false;
    return;
  }

  // open a stream
  std::istream *ifs = tlp::getInputFileStream(filename.c_str(),
                      std::ifstream::in |
                      // consider file is binary
                      // to avoid pb using tellg
                      // on the input stream
                      std::ifstream::binary);

  // get length of file:
  ifs->seekg (0, std::ios::end);
  int fileLength = ifs->tellg();
  ifs->seekg (0, std::ios::beg);

  // allocate memory:
  char* fileData = new char[fileLength];

  // read data as a block:
  ifs->read (fileData, fileLength);
  delete ifs;

  parse((const unsigned char *) fileData, fileLength);

  delete[] fileData;
}

void YajlParseFacade::parse(const unsigned char* data, int length) {
  const yajl_callbacks callbacks = {
    parse_null,
    parse_boolean,
    parse_integer,
    parse_double,
    NULL,
    parse_string,
    parse_start_map,
    parse_map_key,
    parse_end_map,
    parse_start_array,
    parse_end_array
  };
  yajl_handle hand = yajl_alloc(&callbacks, NULL, this);
  yajl_status stat  = yajl_parse(hand, data, length);

  if (stat != yajl_status_ok) {
    unsigned char * str = yajl_get_error(hand, 1, data, length);
    _parsingSucceeded = false;
    _errorMessage = std::string((const char *)str);
    yajl_free_error(hand, str);
  }

  //   yajl_gen_free(g);
  yajl_free(hand);
}

bool YajlParseFacade::parsingSucceeded() const {
  return _parsingSucceeded;
}

std::string YajlParseFacade::errorMessage() const {
  return _errorMessage;
}


void YajlParseFacade::parseBoolean(bool) {
}

void YajlParseFacade::parseDouble(double) {
}

void YajlParseFacade::parseEndArray() {
}

void YajlParseFacade::parseEndMap() {
}

void YajlParseFacade::parseInteger(long long) {
}

void YajlParseFacade::parseMapKey(const std::string&) {
}

void YajlParseFacade::parseNull() {
}

void YajlParseFacade::parseNumber(const char*, size_t) {
}

void YajlParseFacade::parseStartArray() {
}

void YajlParseFacade::parseStartMap() {
}

void YajlParseFacade::parseString(const std::string&) {
}

/** ============================================================= **/

YajlWriteFacade::YajlWriteFacade() {
  _generator = yajl_gen_alloc(NULL);
  yajl_gen_config(_generator, yajl_gen_validate_utf8, 1);
}

void YajlWriteFacade::beautifyString(bool beautify) {
  yajl_gen_config(_generator, yajl_gen_beautify, beautify);

  if (beautify) {
    yajl_gen_config(_generator, yajl_gen_indent_string, "  ");
  }
  else {
    yajl_gen_config(_generator, yajl_gen_indent_string, "");
  }
}

YajlWriteFacade::~YajlWriteFacade() {
  yajl_gen_free(_generator);
}

void YajlWriteFacade::writeInteger(long long int number) {
  yajl_gen_integer(_generator, number);
}

void YajlWriteFacade::writeDouble(double number) {
  yajl_gen_double(_generator, number);
}

void YajlWriteFacade::writeNumber(const char* str, size_t len) {
  yajl_gen_number(_generator, str, len);
}

void YajlWriteFacade::writeString(const std::string& text) {
  yajl_gen_string(_generator, (unsigned char*)text.c_str(), text.size());
}

void YajlWriteFacade::writeNull() {
  yajl_gen_null(_generator);
}

void YajlWriteFacade::writeBool(bool value) {
  yajl_gen_bool(_generator, value);
}

void YajlWriteFacade::writeMapOpen() {
  yajl_gen_map_open(_generator);
}

void YajlWriteFacade::writeMapClose() {
  yajl_gen_map_close(_generator);
}

void YajlWriteFacade::writeArrayOpen() {
  yajl_gen_array_open(_generator);
}

void YajlWriteFacade::writeArrayClose() {
  yajl_gen_array_close(_generator);
}

std::string YajlWriteFacade::generatedString() {
  size_t length;
  const unsigned char* buffer;
  yajl_gen_status status = yajl_gen_get_buf(_generator, &buffer, &length);

  if(status != yajl_gen_status_ok) {
    tlp::debug() << __PRETTY_FUNCTION__ << ": parse error.";
  }

  std::string result((const char*)buffer);
  return result;
}
