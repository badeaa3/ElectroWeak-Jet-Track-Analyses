#ifndef CUTCONFIGURATION
#define CUTCONFIGURATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <utility>      // std::pair

#include "../../CutConfigurations/interface/PhotonCuts.h"
#include "../../CutConfigurations/interface/ElectronCuts.h"
#include "../../CutConfigurations/interface/MuonCuts.h"
#include "../../CutConfigurations/interface/ZBosonCuts.h"
#include "../../CutConfigurations/interface/JetCuts.h"
#include "../../CutConfigurations/interface/TrackCuts.h"
#include "../../CutConfigurations/interface/GenParticleCuts.h"
#include "../../CutConfigurations/interface/GammaJetCuts.h"
#include "../../CutConfigurations/interface/ZJetCuts.h"
#include "../../CutConfigurations/interface/TrackJetCuts.h"
#include "../../CutConfigurations/interface/EventCuts.h"

#include "Configuration.h"
#include "ConfigurationParser.h"

#include "../systemUtil.h"

namespace CUTS {
struct ObjectCuts {
  std::vector<float> f;
  std::vector<int> i;
  std::vector<std::string> s;
  std::vector<std::string> str_i;     // string version of the integer input
  std::vector<std::string> str_f;     // string version of the float input

  std::vector<char*> c; // this is a c-string copy of s
};

enum OBJECT {
  kPHOTON,
  kELECTRON,
  kMUON,
  kZBOSON,
  kJET,
  kTRACK,
  kGENPARTICLE,
  kGAMMAJET,
  kZJET,
  kTRACKJET,
  kEVENT,
  kN_OBJECTS
};

const int SUMMARY_INFO_I[kN_OBJECTS] = {PHO::kN_I_CUTS,
                                        ELE::kN_I_CUTS,
                                        MUO::kN_I_CUTS,
                                        ZBO::kN_I_CUTS,
                                        JET::kN_I_CUTS,
                                        TRK::kN_I_CUTS,
                                        GEN::kN_I_CUTS,
                                        GJT::kN_I_CUTS,
                                        ZJT::kN_I_CUTS,
                                        TJT::kN_I_CUTS,
                                        EVT::kN_I_CUTS
                                       };
const std::string *SUMMARY_INFO_I_LABELS[kN_OBJECTS] = {PHO::I_CUTS_LABELS,
                                                        ELE::I_CUTS_LABELS,
                                                        MUO::I_CUTS_LABELS,
                                                        ZBO::I_CUTS_LABELS,
                                                        JET::I_CUTS_LABELS,
                                                        TRK::I_CUTS_LABELS,
                                                        GEN::I_CUTS_LABELS,
                                                        GJT::I_CUTS_LABELS,
                                                        ZJT::I_CUTS_LABELS,
                                                        TJT::I_CUTS_LABELS,
                                                        EVT::I_CUTS_LABELS
                                                       };

const int SUMMARY_INFO_F[kN_OBJECTS] = {PHO::kN_F_CUTS,
                                        ELE::kN_F_CUTS,
                                        MUO::kN_F_CUTS,
                                        ZBO::kN_F_CUTS,
                                        JET::kN_F_CUTS,
                                        TRK::kN_F_CUTS,
                                        GEN::kN_F_CUTS,
                                        GJT::kN_F_CUTS,
                                        ZJT::kN_F_CUTS,
                                        TJT::kN_F_CUTS,
                                        EVT::kN_F_CUTS
                                       };
const std::string *SUMMARY_INFO_F_LABELS[kN_OBJECTS] = {PHO::F_CUTS_LABELS,
                                                        ELE::F_CUTS_LABELS,
                                                        MUO::F_CUTS_LABELS,
                                                        ZBO::F_CUTS_LABELS,
                                                        JET::F_CUTS_LABELS,
                                                        TRK::F_CUTS_LABELS,
                                                        GEN::F_CUTS_LABELS,
                                                        GJT::F_CUTS_LABELS,
                                                        ZJT::F_CUTS_LABELS,
                                                        TJT::F_CUTS_LABELS,
                                                        EVT::F_CUTS_LABELS
                                                       };

const int SUMMARY_INFO_S[kN_OBJECTS] = {PHO::kN_S_CUTS,
                                        ELE::kN_S_CUTS,
                                        MUO::kN_S_CUTS,
                                        ZBO::kN_S_CUTS,
                                        JET::kN_S_CUTS,
                                        TRK::kN_S_CUTS,
                                        GEN::kN_S_CUTS,
                                        GJT::kN_S_CUTS,
                                        ZJT::kN_S_CUTS,
                                        TJT::kN_S_CUTS,
                                        EVT::kN_S_CUTS
                                       };
const std::string *SUMMARY_INFO_S_LABELS[kN_OBJECTS] = {PHO::S_CUTS_LABELS,
                                                        ELE::S_CUTS_LABELS,
                                                        MUO::S_CUTS_LABELS,
                                                        ZBO::S_CUTS_LABELS,
                                                        JET::S_CUTS_LABELS,
                                                        TRK::S_CUTS_LABELS,
                                                        GEN::S_CUTS_LABELS,
                                                        GJT::S_CUTS_LABELS,
                                                        ZJT::S_CUTS_LABELS,
                                                        TJT::S_CUTS_LABELS,
                                                        EVT::S_CUTS_LABELS
                                                       };

std::string OBJECT_LABELS[kN_OBJECTS] = {"photon",
                                         "electron",
                                         "muon",
                                         "zboson",
                                         "jet",
                                         "track",
                                         "genparticle",
                                         "gammajet",
                                         "zjet",
                                         "trackjet",
                                         "event"
                                        };

struct ProcessCuts {
  ObjectCuts obj[kN_OBJECTS];
};

enum PROCESS {
  kSKIM,
  kCORRECTION,
  kHISTOGRAM,
  kPLOTTING,
  kPERFORMANCE,
  kN_PROCESSES // must come last in enum
};

std::string PROCESS_LABELS[kN_PROCESSES] = {"skim",
                                            "correction",
                                            "histogram",
                                            "plotting",
                                            "performance"
                                           };
};

struct CutConfiguration : public Configuration {
  CUTS::ProcessCuts proc[CUTS::kN_PROCESSES];

};

class CutConfigurationParser : public ConfigurationParser {
 public:
  CutConfigurationParser() {};
  ~CutConfigurationParser() {};

  static CutConfiguration Parse(std::string inFile);
  static void copyConfiguration(CutConfiguration& config, CutConfiguration configCopy);
};

/*
 * copy values from "configCopy" to "config".
 * if the original config has a value for a field, then do not copy.
 */
void CutConfigurationParser::copyConfiguration(CutConfiguration& config, CutConfiguration configCopy) {
  for (int i = 0 ; i < CUTS::kN_PROCESSES; ++i) {
    for (int j = 0; j < CUTS::kN_OBJECTS; ++j) {
      for (int k = 0; k < CUTS::SUMMARY_INFO_I[j]; ++k) {
        if (config.proc[i].obj[j].i[k] == 0) {
            config.proc[i].obj[j].i[k] = configCopy.proc[i].obj[j].i[k];
            config.proc[i].obj[j].str_i[k] = configCopy.proc[i].obj[j].str_i[k];
        }
      }
      for (int k = 0; k < CUTS::SUMMARY_INFO_F[j]; ++k) {
        if (config.proc[i].obj[j].f[k] == 0) {
            config.proc[i].obj[j].f[k] = configCopy.proc[i].obj[j].f[k];
            config.proc[i].obj[j].str_f[k] = configCopy.proc[i].obj[j].str_f[k];
        }
      }
      for (int k = 0; k < CUTS::SUMMARY_INFO_S[j]; ++k) {
        if (config.proc[i].obj[j].s[k].size() == 0) {
          config.proc[i].obj[j].s[k] = configCopy.proc[i].obj[j].s[k];
          char * cstr = new char [config.proc[i].obj[j].s[k].length()+1];
          std::strcpy(cstr, config.proc[i].obj[j].s[k].c_str());
          config.proc[i].obj[j].c[k] = cstr;
        }
      }
    }
  }
}

CutConfiguration CutConfigurationParser::Parse(std::string inFile) {
  using namespace CUTS;
  std::string endSignal = "#CUTS-END#";     // signals that cut configuration parsing is to be terminated.
  // another block of configuration parsing will start.

  CutConfiguration config;
  for (int i = 0 ; i < kN_PROCESSES; ++i) {
    for (int j = 0; j < kN_OBJECTS; ++j) {
      config.proc[i].obj[j].i.resize(SUMMARY_INFO_I[j]);
      config.proc[i].obj[j].f.resize(SUMMARY_INFO_F[j]);
      config.proc[i].obj[j].s.resize(SUMMARY_INFO_S[j]);
      config.proc[i].obj[j].str_i.resize(SUMMARY_INFO_I[j]);
      config.proc[i].obj[j].str_f.resize(SUMMARY_INFO_F[j]);
      config.proc[i].obj[j].c.resize(SUMMARY_INFO_S[j]);
    }
  }

  std::ifstream fin(inFile);
  if ((fin.rdstate() & std::ifstream::failbit) != 0) {
    std::cout << "I/O Error opening configuration file." << std::endl;
    config.isValid = false;
    return config;
  }

  std::map<std::string, std::string> mapVarString;    // map of variables of type string

  std::string line;
  unsigned int lineCounter = 0;
  while (getline(fin, line)) {
    lineCounter++;
    if (line.find(endSignal) != std::string::npos) break;
    bool isCommand = ConfigurationParser::isCommand(line);
    if (line.find("=") == std::string::npos) continue; //skip all lines without an =
    if (line.find(".") == std::string::npos) continue; //skip all lines without a dot
    if (ConfigurationParser::isComment(line)) continue;  //skip all lines starting with comment sign #
    size_t pos = line.find("=") + 1;
    size_t posLast = line.find(CONFIGPARSER::comment.c_str());    // allow inline comment signs with #
    std::string value = ConfigurationParser::ReadValue(fin, line.substr(pos, (posLast-pos)));   // read value over multiple lines if necessary
    value = ConfigurationParser::substituteVarString(value, mapVarString);
    std::istringstream sin(value);
    line = line.substr(0, pos-1);        // "line" becomes the LHS of the "=" sign (excluing the "=" sign)
    if (isCommand) {
      if (ConfigurationParser::isVarDefinitionString(line)) {
        mapVarString.insert(ConfigurationParser::ParseVarDefinitionString(line, value));
      } else if (ConfigurationParser::isImportCutStatement(line)) {
        CutConfiguration importedConfig = CutConfigurationParser::Parse(value);
        CutConfigurationParser::copyConfiguration(config, importedConfig);
      }
      continue;
    }
    bool success = false;
    PROCESS proc = kN_PROCESSES;
    for (int i = 0; i < kN_PROCESSES; ++i) {
      if (line.find(PROCESS_LABELS[i]) != std::string::npos) {
        proc = (PROCESS)i;
        break;
      }
    }
    OBJECT obj = kN_OBJECTS;
    for (int i = 0; i < kN_OBJECTS; ++i) {
      std::string label = Form(".%s.",OBJECT_LABELS[i].c_str());    // prevent substring matching, e.g. : "jet" and "gammajet"
      if (line.find(label) != std::string::npos) {
        obj = (OBJECT)i;

        for (int j = 0; j < SUMMARY_INFO_I[obj]; ++j) {
          std::string label_I = Form(".%s ",SUMMARY_INFO_I_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
          if (line.find(label_I) != std::string::npos) {
            int in;
            sin >> in;
            config.proc[proc].obj[obj].i[j] = in;
            config.proc[proc].obj[obj].str_i[j] = trim(sin.str());
            success = true;
            break;
          }
        }
        for (int j = 0; j < SUMMARY_INFO_F[obj]; ++j) {
          std::string label_F = Form(".%s ",SUMMARY_INFO_F_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
          if (line.find(label_F) != std::string::npos) {
            float in;
            sin >> in;
            config.proc[proc].obj[obj].f[j] = in;
            config.proc[proc].obj[obj].str_f[j] = trim(sin.str());
            success = true;
            break;
          }
        }
        for (int j = 0; j < SUMMARY_INFO_S[obj]; ++j) {
          std::string label_S = Form(".%s ",SUMMARY_INFO_S_LABELS[obj][j].c_str());    // prevent substring matching, e.g. : "et" and "trigger_gammaJet", "doPP" and "doPPMC"
          if (line.find(label_S) != std::string::npos) {
            std::string in;
            in = trim(sin.str());   // stringstream ignores characters after a whitespace, use the original string to read the value.
            config.proc[proc].obj[obj].s[j] = in;
            char * cstr = new char [in.length()+1];
            std::strcpy(cstr, in.c_str());
            config.proc[proc].obj[obj].c[j] = cstr;
            success = true;
            break;
          }
        }
        break;
      }
    }
    if (!success) {
      std::cout << "Malformed line in configuration." << std::endl;
      std::cout << "Line #" << lineCounter << " : " << std::endl;
      std::cout << line << std::endl;
      config.isValid = false;
      return config;
    }
  }
  config.isValid = true;
  return config;
}

#endif
