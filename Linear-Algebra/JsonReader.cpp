// Requires libjsoncpp-dev
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h> //This library is used for interacting with JSON file

#include <fstream> // This library is used for reading and writing data to the files
#include <iostream> // This library defines standard input/ output objects
#include <string>   // This library is used to store text

/*
Json::Value read(const char *f) {
  // Using fstream to get the file pointer in file
  std::ifstream file(f);
  Json::Value actualJson;
  Json::Reader reader;

  // Using the reader, we are parsing the json file
  reader.parse(file, actualJson);

  // The actual json has the json data
  // cout << "Total json data:\n" << actualJson << '\n';
  return actualJson;
}
*/
