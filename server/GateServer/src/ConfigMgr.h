#ifndef CONFIGMGR_H
#define CONFIGMGR_H
#include "const.h"
struct SectionInfo {
  SectionInfo() {}
  ~SectionInfo() { _section_datas.clear(); }

  SectionInfo(const SectionInfo &src) { _section_datas = src._section_datas; }
  SectionInfo &operator=(const SectionInfo &src) {
    if (&src == this) {
      return *this;
    }
    this->_section_datas = src._section_datas;
  }
  std::map<std::string, std::string> _section_datas;
  std::string operator[](const std::string &key) {
    if (_section_datas.find(key) == _section_datas.end()) {
      return "";
    }
    return _section_datas[key];
  }
};

class ConfigMgr {
public:
  ~ConfigMgr();
  SectionInfo operator[](const std::string &section);

  ConfigMgr(const ConfigMgr &src);
  ConfigMgr &operator=(const ConfigMgr &src);
  ConfigMgr();

private:
  std::map<std::string, SectionInfo> _config_map;
};
#endif // CONFIGMGR_H
