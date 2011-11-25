#include <tulip/YajlFacade.h>

#include <QFile>
#include <malloc.h>

#include <iostream>

static int parse_null(void *ctx) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseNull();
  return 1;
}

static int parse_boolean(void * ctx, int boolVal) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseBoolean(boolVal);
  return 1;
}

static int parse_integer(void *ctx, long long integerVal) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseInteger(integerVal);
  return 1;
}

static int parse_double(void *ctx, double doubleVal) {
  YajlFacade* facade = (YajlFacade*) ctx;
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
  YajlFacade* facade = (YajlFacade*) ctx;

  char * str = (char *) malloc(stringLen + 1);
  str[stringLen] = 0;
  memcpy(str, stringVal, stringLen);
  std::string key((char*)str);

  facade->parseString(key);
  return 1;
}

static int parse_map_key(void *ctx, const unsigned char * stringVal, size_t stringLen) {
  YajlFacade* facade = (YajlFacade*) ctx;

  char * str = (char *) malloc(stringLen + 1);
  str[stringLen] = 0;
  memcpy(str, stringVal, stringLen);
  std::string key((char*)str);

  facade->parseMapKey(key);
  return 1;
}

static int parse_start_map(void *ctx) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseStartMap();
  return 1;
}

static int parse_end_map(void *ctx) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseEndMap();
  return 1;
}

static int parse_start_array(void *ctx) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseStartArray();
  return 1;
}

static int parse_end_array(void *ctx) {
  YajlFacade* facade = (YajlFacade*) ctx;
  facade->parseEndArray();
  return 1;
}

void YajlFacade::parse(std::string filename) {
  QFile file(filename.c_str());
  file.open(QIODevice::ReadOnly);
  QByteArray contents = file.readAll();

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
  unsigned char* fileData = (unsigned char*)contents.data();

  yajl_status stat  = yajl_parse(hand, fileData, contents.length());

  if (stat != yajl_status_ok) {
    unsigned char * str = yajl_get_error(hand, 1, fileData, contents.length());
    std::cout << (const char *)str << std::endl;
    yajl_free_error(hand, str);
  }

//   yajl_gen_free(g);
  yajl_free(hand);
}

void YajlFacade::parseBoolean(bool) {
}

void YajlFacade::parseDouble(double) {
}

void YajlFacade::parseEndArray() {
}

void YajlFacade::parseEndMap() {
}

void YajlFacade::parseInteger(long long) {
}

void YajlFacade::parseMapKey(const std::string&) {
}

void YajlFacade::parseNull() {
}

void YajlFacade::parseNumber(const char*, size_t) {
}

void YajlFacade::parseStartArray() {
}

void YajlFacade::parseStartMap() {
}

void YajlFacade::parseString(const std::string&) {
}
