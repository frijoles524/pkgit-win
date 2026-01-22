#include <string>

std::string name_from_url(std::string url) {
  return url.substr(url.find_last_of('/') + 1);
}
