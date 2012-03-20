#include <yajl_parse.h>
#include <yajl_gen.h>
#include <string>

namespace tlp {
class PluginProgress;
}

/**
 * @brief A Simple C++ wrapper around the C library 'yajl' parsing capabilities.
 *
 * It uses callbacks to signal what is being parsed (map start and end, strings, ...), and this class is intended to be subclassed,
 * with the subclass overriding the callbacks to process the events.
 **/
class YajlParseFacade {
public:
  YajlParseFacade(tlp::PluginProgress* progress) : _progress(progress), _parsingSucceeded(true) {}

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

  bool parsingSucceeded() const;
  std::string errorMessage() const;

protected:
  tlp::PluginProgress* _progress;
  bool _parsingSucceeded;
  std::string _errorMessage;
};

class YajlWriteFacade {
public:
  YajlWriteFacade();
  ~YajlWriteFacade();

  std::string generatedString();

  virtual void writeInteger(long long int number);
  virtual void writeDouble(double number);
  virtual void writeNumber(const char* str, size_t len);
  virtual void writeString(const std::string& text);
  virtual void writeNull();
  virtual void writeBool(bool value);
  virtual void writeMapOpen();
  virtual void writeMapClose();
  virtual void writeArrayOpen();
  virtual void writeArrayClose();
protected:
  yajl_gen _generator;
};
