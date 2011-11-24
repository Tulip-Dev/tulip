#include <yajl_parse.h>
#include <string>

/**
 * @brief A Simple C++ wrapper around the C library 'yajl'.
 *
 * It uses callbacks to signal what is being parsed (map start and end, strings, ...), and this class is intended to be subclassed,
 * with the subclass overriding the callbacks to process the events.
 **/
class YajlFacade {
public:
  /**
   * @brief Parses a JSON file.
   * Once this function is called, the callbacks (all the parse* functions) will get called when the corresponding event happens.
   *
   * @param filename The file to parse.
   * @return void
   **/
  void parse(std::string filename);
  
  virtual void parseNull();
  virtual void parseBoolean(bool boolVal);
  virtual void parseInteger(long long integerVal);
  virtual void parseDouble(double doubleVal);
  virtual void parseNumber(const char * numberVal, size_t numberLen);
  virtual void parseString(const std::string& value);
  virtual void parseMapKey(const std::string& value);
  virtual void parseStartMap();
  virtual void parseEndMap();
  virtual void parseStartArray();
  virtual void parseEndArray();
};
