#ifndef UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_
#define UTILITIES_INTERFACE_CONFIGURATIONPARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>        // std::size_t
#include <map>
#include <utility>      // std::pair

#include "../systemUtil.h"
#include "../th1Util.h"

namespace CONFIGPARSER{

const std::string comment = "#";
const std::string nullInput = "$NULL$";    // explicit specification of no input
const std::string noTrim = "$NOTRIM$";     // element will not be trimmed from the line.
const std::string newLine = "$NEWLINE$";   // the value continues over the next line. useful when entering a long list of values.
const std::string multiLineBegin = "$MLB$";   // sign the beginning of a multi line value
const std::string multiLineEnd = "$MLE$";     // sign the end of a multi line value
const std::string times = "$TIMES$";       // operator for multiple lists
const std::string plus = "$PLUS$";       // operator for multiple lists
const std::string importStatement = "import.";
const std::string importInputStatement = "import.input";
const std::string importCutStatement = "import.cut";
const std::string varDefinition = "var.";
const std::string varDefinitionString = "var.string";

const std::string separator1 = ",";
const std::string separator2 = ";;";        // separator used as alternative to separator1
const std::string separator3 = ";;;";       // separator used for list of lists

struct RunLumiEvent {
  unsigned int run, lumi;
  unsigned long long event;
};

/*
 * object for reading TH1 axis information from configuration file
 */
struct TH1Axis {
    TH1Axis() : nBins(0), xLow(0), xUp(-1), bins({}), binsAreFromUser(false) {}
    void setBins() {
        bins = getTH1xBins(nBins, xLow, xUp);
    }

    int nBins;
    double xLow;
    double xUp;
    std::vector<double> bins;   // vector of size nBins+1
    bool binsAreFromUser;
};

/*
 * object for reading TH2D axis information from configuration file
 */
struct TH2DAxis {
    TH2DAxis() {}

    TH1Axis axisX;
    TH1Axis axisY;
};

}

class ConfigurationParser {

public :
    static bool isList(std::string str);
    static bool isMultipleList(std::string str);
    static bool isComment(std::string line);
    static bool isCommand(std::string line);
    static bool isImportStatement(std::string line);
    static bool isImportInputStatement(std::string line);
    static bool isImportCutStatement(std::string line);
    static bool isVarDefinition(std::string line);
    static bool isVarDefinitionString(std::string line);
    static bool isTH1D_BinsArray(std::string str);
    static std::string getMultiListOperator(std::string strList);
    static std::vector<std::string> getVecString(std::vector<std::pair<std::string, int>> vecStringIndex);
    static std::vector<int> getVecInteger(std::vector<std::pair<int, int>> vecIntegerIndex);
    static std::vector<float> getVecFloat(std::vector<std::pair<float, int>> vecFloatIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<std::string, int>> vecStringIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<int, int>> vecIntegerIndex);
    static std::vector<int> getVecIndex(std::vector<std::pair<float, int>> vecFloatIndex);
    static std::string varReference(std::string varName);
    static std::string trimComment(std::string line);
    static std::string ReadValue(std::ifstream& fin, std::string value);
    static std::string substituteVarString(std::string value, std::map<std::string, std::string> mapVarString);
    static std::pair<std::string, std::string> ParseVarDefinitionString(std::string command, std::string value);
    static std::vector<std::string> ParseList(std::string strList, std::string separator = "");
    static std::vector<std::string> ParseListOrString(std::string strListOrString);
    static std::vector<std::string> ParseMultipleLists(std::string strLists, std::string separator = "");
    static std::vector<std::string> ParseListWithoutBracket(std::string strList, std::string separator = "");
    static std::vector<int> ParseListWithoutBracketInteger(std::string strList, std::string separator = "");
    static std::vector<float> ParseListWithoutBracketFloat(std::string strList, std::string separator = "");
    static std::vector<int> ParseListInteger(std::string strList);
    static std::vector<int> ParseListOrInteger(std::string strListOrInteger);
    static std::vector<float> ParseListFloat(std::string strList);
    static std::vector<float> ParseListOrFloat(std::string strListOrFloat);
    static std::vector<std::pair<std::string, int>> ParseListOfList(std::string strListOfList);
    static std::vector<std::pair<std::string, int>> ParseListOfList(std::vector<std::string> listStrList);
    static std::vector<std::pair<int, int>> ParseListOfListInteger(std::string strListOfList);
    static std::vector<std::pair<int, int>> ParseListOfListInteger(std::vector<std::string> listStrList);
    static std::vector<std::pair<float, int>> ParseListOfListFloat(std::string strListOfList);
    static std::vector<std::pair<float, int>> ParseListOfListFloat(std::vector<std::string> listStrList);
    static CONFIGPARSER::RunLumiEvent ParseRunLumiEvent(std::string strRunLumiEvent);
    static unsigned int ParseRunNumber(std::string strRunLumiEvent);
    static unsigned int ParseLumiNumber(std::string strRunLumiEvent);
    static unsigned long long ParseEventNumber(std::string strRunLumiEvent);
    static std::vector<std::vector<float>> ParseListTH1D_Bins(std::string strList);
    static std::vector<CONFIGPARSER::TH1Axis> ParseListTH1D_Axis(std::string strList);
    static std::vector<std::vector<float>> ParseListTH2D_Bins(std::string strList);
    static std::vector<CONFIGPARSER::TH2DAxis> ParseListTH2D_Axis(std::string strList);
    static std::string verboseTH1D_Axis(CONFIGPARSER::TH1Axis th1Axis);
    static std::string verboseTH2D_Axis(CONFIGPARSER::TH2DAxis th2DAxis);
    static std::string ParseLatex(std::string str);
    static std::vector<std::string> ParseListLatex(std::string strList, std::string separator = "");
    static std::vector<std::vector<std::string>> ParseListTF1(std::string strList);
    static std::vector<std::string> ParseListTF1Formula(std::string strList);
    static std::vector<std::vector<double>> ParseListTF1Range(std::string strList);

};

/*
 * a string is a list if it starts with "{" and ends with "}"
 */
bool ConfigurationParser::isList(std::string str)
{
    std::string tmp = trim(str);
    return (tmp.find("{") == 0 && tmp.rfind("}") == tmp.size()-1);
}

/*
 * a string is a multiple list if it there are multiple lists separated by special delimiters
 */
bool ConfigurationParser::isMultipleList(std::string str)
{
    std::string tmp = trim(str);
    std::string operatorStr = getMultiListOperator(str);
    if (operatorStr.size() == 0)  return false;

    std::vector<std::string> tmpVec = split(str, operatorStr);
    if (tmpVec.size() <= 1) return false;
    else {
        for (std::vector<std::string>::iterator it = tmpVec.begin(); it != tmpVec.end(); ++it) {
            if (!isList((*it)))  return false;
        }
    }
    return true;
}

bool ConfigurationParser::isComment(std::string line)
{
    return (trim(line).find(CONFIGPARSER::comment.c_str()) == 0);
}

bool ConfigurationParser::isCommand(std::string line)
{
    return (isImportStatement(line) || isVarDefinition(line));
}

bool ConfigurationParser::isImportStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importStatement.c_str()) == 0);
}

bool ConfigurationParser::isImportInputStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importInputStatement.c_str()) == 0);
}

bool ConfigurationParser::isImportCutStatement(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::importCutStatement.c_str()) == 0);
}

bool ConfigurationParser::isVarDefinition(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::varDefinition.c_str()) == 0);
}

bool ConfigurationParser::isVarDefinitionString(std::string line)
{
    std::string tmp = trim(line);
    return (tmp.find(CONFIGPARSER::varDefinitionString.c_str()) == 0);
}

/*
 * a string is an array of TH1D bins if it starts with "[" and ends with "]"
 */
bool ConfigurationParser::isTH1D_BinsArray(std::string str)
{
    std::string tmp = trim(str);
    return (tmp.find("[") == 0 && tmp.rfind("]") == tmp.size()-1);
}

std::string ConfigurationParser::getMultiListOperator(std::string strList)
{
    std::string res;
    if (strList.find(CONFIGPARSER::times) != std::string::npos)
        res = CONFIGPARSER::times;
    else if (strList.find(CONFIGPARSER::plus) != std::string::npos)
        res = CONFIGPARSER::plus;

    return res;
}

std::vector<std::string> ConfigurationParser::getVecString(std::vector<std::pair<std::string, int>> vecStringIndex)
{
    std::vector<std::string> list;

    int n = vecStringIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecStringIndex.at(i).first.c_str());
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecInteger(std::vector<std::pair<int, int>> vecIntegerIndex)
{
    std::vector<int> list;

    int n = vecIntegerIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecIntegerIndex.at(i).first);
    }

    return list;
}

std::vector<float> ConfigurationParser::getVecFloat(std::vector<std::pair<float, int>> vecFloatIndex)
{
    std::vector<float> list;

    int n = vecFloatIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecFloatIndex.at(i).first);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<std::string, int>> vecStringIndex)
{
    std::vector<int> list;

    int n = vecStringIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecStringIndex.at(i).second);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<int, int>> vecIntegerIndex)
{
    std::vector<int> list;

    int n = vecIntegerIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecIntegerIndex.at(i).second);
    }

    return list;
}

std::vector<int> ConfigurationParser::getVecIndex(std::vector<std::pair<float, int>> vecFloatIndex)
{
    std::vector<int> list;

    int n = vecFloatIndex.size();
    for (int i = 0; i < n; ++i) {
        list.push_back(vecFloatIndex.at(i).second);
    }

    return list;
}

/*
 * constructs the syntax for referencing a variable
 */
std::string ConfigurationParser::varReference(std::string varName)
{
    return "$"+varName+"$";
}

std::string ConfigurationParser::trimComment(std::string line)
{
    size_t pos = line.find(CONFIGPARSER::comment.c_str());
    return line.substr(0,pos);
}

/*
 * read the value over multiple lines
 * 1. as long as the lines finish with "CONFIGPARSER::newLine" key.
 * OR
 * 2. if the value has started with "CONFIGPARSER::multiLineBegin", then read over multiple lines until "CONFIGPARSER::multiLineEnd" is found.
 */
std::string ConfigurationParser::ReadValue(std::ifstream& fin, std::string value)
{
    std::string result = trim(value);
    // ends with newLine
    bool endsWithNL = endsWith(result, CONFIGPARSER::newLine.c_str());

    // started with multiLineBegin
    bool startedWithMLB = startsWith(result, CONFIGPARSER::multiLineBegin.c_str());
    bool endsWithMLE = endsWith(result, CONFIGPARSER::multiLineEnd.c_str());

    if (endsWithNL || (startedWithMLB && !endsWithMLE)) {

        result = replaceAll(result, CONFIGPARSER::newLine.c_str(), "");
        result = replaceAll(result, CONFIGPARSER::multiLineBegin.c_str(), "");
        result = trim(result);  // need to trim the line again after removing newLine string
        std::string nextLine;
        while(getline(fin, nextLine)) {
            std::string trimmedLine = trimComment(nextLine);
            trimmedLine = trim(trimmedLine);
            if (trimmedLine.size() == 0) continue;  //skip all empty lines or the ones starting with comment sign

            endsWithNL = endsWith(trimmedLine, CONFIGPARSER::newLine.c_str());
            trimmedLine = replaceAll(trimmedLine, CONFIGPARSER::newLine.c_str(), "");
            trimmedLine = trim(trimmedLine);  // need to trim the line again after removing newLine string

            endsWithMLE = endsWith(trimmedLine, CONFIGPARSER::multiLineEnd.c_str());
            trimmedLine = replaceAll(trimmedLine, CONFIGPARSER::multiLineEnd.c_str(), "");
            trimmedLine = trim(trimmedLine);  // need to trim the line again after removing multiLineEnd string

            result.append(trimmedLine);
            if(!startedWithMLB && !endsWithNL) {
                break;
                /*
                 * input.someField = AAA    $NEWLINE$
                                     BBB    $NEWLINE$
                                     CCC
                    ==> someField = AAABBBCCC
                 */
            }
            else if (startedWithMLB && endsWithMLE) {
                break;
                /*
                 * input.someField = $MLB$ AAA
                                           BBB
                                           CCC $MLE$
                    ==> someField = AAABBBCCC
                 */
            }
        }
    }
    return result;
}

/*
 * substitute the values of all current string variables in "value"
 */
std::string ConfigurationParser::substituteVarString(std::string value, std::map<std::string, std::string> mapVarString)
{
    std::map<std::string, std::string>::const_iterator iterator;
    for (iterator = mapVarString.begin(); iterator != mapVarString.end(); ++iterator) {
        std::string varName = iterator->first.c_str();
        std::string varRef = varReference(varName);
        value = replaceAll(value, varRef, iterator->second.c_str());
    }

    return value;
}

/*
 * parse the name and value of a string variable.
 */
std::pair<std::string, std::string> ConfigurationParser::ParseVarDefinitionString(std::string command, std::string value)
{
    std::string varName = replaceAll(command,CONFIGPARSER::varDefinitionString,"");
    varName = trim(varName);
    value = trim(value);
    return (std::pair<std::string, std::string>(varName, value));
}

std::vector<std::string> ConfigurationParser::ParseList(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    if(isMultipleList(strList))  return ParseMultipleLists(strList, separator);

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator1.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        // make trimming optional, trimming can be skipped if "noTrim" key appears in the element
        bool doTrim = (tmp.find(CONFIGPARSER::noTrim) == std::string::npos);
        std::string element;
        if (doTrim) {
            element = trim(tmp);
        }
        else {  // do not trim, allow  leading and trailing white space characters to appear in the element.
            // remove the "do not trim" option from the element
            element = replaceAll(tmp, CONFIGPARSER::noTrim, "");
        }

        if(element.size() > 0 ) list.push_back(element.c_str());
        posStart = pos + separator.size();
    }

    return list;
}

/*
 * if the string is a list that starts with '{' and ends with '}', then parse it as list.
 * Otherwise, return a vector whose only element is the string itself.
 *
 * This function is to be used for configurations that can be a string or a list
 * Ex.
 * input.plotting.legendPosition = NE             # string
 * input.plotting.legendPosition = {NE, SW, NW}   # list
 */
std::vector<std::string> ConfigurationParser::ParseListOrString(std::string strListOrString)
{
    if (!isList(strListOrString))
        return {strListOrString};
    else
        return ParseList(strListOrString);
}

/*
 * parse multiple lists separated by special operators
 * Ex.
 * strLists = {AA, BB} $TIMES$ {11, 22} $TIMES$ {CC, DD, EE}
 * returns
 * {AA11CC, BB11CC, AA22CC, BB22CC, AA11DD, BB11DD, AA22DD, BB22DD, AA11EE, BB11EE, AA22EE, BB22EE}
 * Ex.
 * strLists = {AA, BB} $PLUS$ {11, 22} $PLUS$ {CC, DD}
 * returns
 * {AA, 11, CC, BB, 11, CC, AA, 22, CC, BB, 22, CC, AA, 11, DD, BB, 11, DD, AA, 22, DD, BB, 22, DD}
 */
std::vector<std::string> ConfigurationParser::ParseMultipleLists(std::string strLists, std::string separator)
{
    std::string operatorStr = getMultiListOperator(strLists);
    std::vector<std::string> strListsVec = split(strLists, operatorStr);

    int nLists = strListsVec.size();
    std::vector<std::string> lists[nLists];

    for (int iList = 0; iList < nLists; ++iList) {
        lists[iList] = ParseList(strListsVec[iList], separator);
    }

    std::vector<std::string> list;
    if (operatorStr == CONFIGPARSER::times) {
        for (int iList = 0; iList < nLists; ++iList) {

            // take the list from previous iteration
            std::vector<std::string> listTmp = list;

            // the list from previous iteration is no more the one we want.
            list.clear();

            int n = lists[iList].size();
            for (int i = 0; i < n; ++i) {

                int nTmp = listTmp.size();
                std::vector<std::string> listTmp2 = listTmp;

                // append strings from the current list to strings from the previous iteration
                if (nTmp == 0) listTmp2.push_back(lists[iList][i]);
                else {
                    for (int j = 0; j < nTmp; ++j) {
                        listTmp2[j].append(lists[iList][i].c_str());
                    }
                }

                list.insert(list.end(), listTmp2.begin(), listTmp2.end());
            }
        }
    }
    else if (operatorStr == CONFIGPARSER::plus) {

        int nTot = 0;
        for (int iList = 0; iList < nLists; ++iList) {
            if (iList == 0)  nTot  = lists[iList].size();
            else             nTot *= lists[iList].size();
        }

        list.clear();
        list.resize(nTot*nLists);
        for (int i = 0; i < nTot; ++i) {

            for (int iList = nLists -1 ; iList >= 0; --iList) {

                int nTmp = nTot;
                for (int jList = iList+1 ; jList < nLists; ++jList) {
                    nTmp /= lists[jList].size();
                }
                int iTmp = i % nTmp;
                nTmp /= lists[iList].size();
                iTmp /= nTmp;

                list[i * nLists + iList] = lists[iList][iTmp];
            }
        }
    }

    return list;
}

/*
 * parse a list of strings where the list is not enclosed with brackets.
 */
std::vector<std::string> ConfigurationParser::ParseListWithoutBracket(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = 0;
    size_t posEnd   = strList.size();

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator1.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

    size_t pos;
    bool listFinished = false;
    while (!listFinished) {

        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        // make trimming optional, trimming can be skipped if "noTrim" key appears in the element
        bool doTrim = (tmp.find(CONFIGPARSER::noTrim) == std::string::npos);
        std::string element;
        if (doTrim) {
            element = trim(tmp);
        }
        else {  // do not trim, allow  leading and trailing white space characters to appear in the element.
            // remove the "do not trim" option from the element
            element = replaceAll(tmp, CONFIGPARSER::noTrim, "");
        }

        if(element.size() > 0 ) list.push_back(element.c_str());
        posStart = pos + separator.size();
    }

    return list;
}


/*
 * parse a list of integers where the list is not enclosed with brackets.
 */
std::vector<int> ConfigurationParser::ParseListWithoutBracketInteger(std::string strList, std::string separator)
{
    std::vector<int> list;
    std::vector<std::string> listStr = ParseListWithoutBracket(strList, separator);

    for (std::vector<std::string>::const_iterator it = listStr.begin(); it < listStr.end(); ++it) {

        std::string element = trim((*it).c_str());
        std::istringstream sin(element);

        int val;
        sin >> val;

        list.push_back(val);
    }

    return list;
}

/*
 * parse a list of floats where the list is not enclosed with brackets.
 */
std::vector<float> ConfigurationParser::ParseListWithoutBracketFloat(std::string strList, std::string separator)
{
    std::vector<float> list;
    std::vector<std::string> listStr = ParseListWithoutBracket(strList, separator);

    for (std::vector<std::string>::const_iterator it = listStr.begin(); it < listStr.end(); ++it) {

        std::string element = trim((*it).c_str());
        std::istringstream sin(element);

        float val;
        sin >> val;

        list.push_back(val);
    }

    return list;
}

std::vector<int> ConfigurationParser::ParseListInteger(std::string strList)
{
    std::vector<int> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);
        std::istringstream sin(element);

        int val;
        sin >> val;

        if(element.size() > 0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

std::vector<int> ConfigurationParser::ParseListOrInteger(std::string strListOrInteger)
{
    if (!isList(strListOrInteger)) {
        std::istringstream sin(strListOrInteger);
        int val;
        sin >> val;
        return {val};
    }
    else
        return ParseListInteger(strListOrInteger);
}

std::vector<float> ConfigurationParser::ParseListFloat(std::string strList)
{
    std::vector<float> list;

    if(strList.empty())
        return list;

    size_t posStart = strList.find("{");     // a valid list starts with '{' and ends with '}'
    if (posStart == std::string::npos) return list;

    size_t posEnd   = strList.rfind("}");     // a valid list starts with '{' and ends with '}'
    if (posEnd == std::string::npos) return list;

    // elements of the list are separated by ','
    size_t pos;
    bool listFinished = false;
    posStart++;     // exclude '{'
    while (!listFinished) {

        pos = strList.find(",", posStart);
        if (pos == std::string::npos) {    // this must be the last element. nothing after '}' is accepted.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        std::string element = trim(tmp);
        std::istringstream sin(element);

        float val;
        sin >> val;

        if(element.size() > 0 ) list.push_back(val);
        posStart = pos + 1;
    }

    return list;
}

std::vector<float> ConfigurationParser::ParseListOrFloat(std::string strListOrFloat)
{
    if (!isList(strListOrFloat)) {
        std::istringstream sin(strListOrFloat);
        float val;
        sin >> val;
        return {val};
    }
    else
        return ParseListFloat(strListOrFloat);
}

std::vector<std::pair<std::string, int>> ConfigurationParser::ParseListOfList(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfList(strList);
}

std::vector<std::pair<std::string, int>> ConfigurationParser::ParseListOfList(std::vector<std::string> listStrList)
{
    std::vector<std::pair<std::string, int>> listStringIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<std::string> tmpList = ConfigurationParser::ParseListWithoutBracket(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listStringIndex.push_back(std::pair<std::string, int>(tmpList.at(j).c_str(), i));
        }
    }

    return listStringIndex;
}

std::vector<std::pair<int, int>> ConfigurationParser::ParseListOfListInteger(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfListInteger(strList);
}

std::vector<std::pair<int, int>> ConfigurationParser::ParseListOfListInteger(std::vector<std::string> listStrList)
{
    std::vector<std::pair<int, int>> listIntegerIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<int> tmpList = ConfigurationParser::ParseListWithoutBracketInteger(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listIntegerIndex.push_back(std::pair<int, int>(tmpList.at(j), i));
        }
    }

    return listIntegerIndex;
}

std::vector<std::pair<float, int>> ConfigurationParser::ParseListOfListFloat(std::string strListOfList)
{
    // strListOfList is a list of lists where lists are separated by separator3
    std::vector<std::string> strList = ParseList(strListOfList, CONFIGPARSER::separator3);

    return ParseListOfListFloat(strList);
}

std::vector<std::pair<float, int>> ConfigurationParser::ParseListOfListFloat(std::vector<std::string> listStrList)
{
    std::vector<std::pair<float, int>> listFloatIndex;

    int nLists = listStrList.size();
    for (int i = 0; i < nLists; ++i) {

        std::vector<float> tmpList = ConfigurationParser::ParseListWithoutBracketFloat(listStrList.at(i).c_str());
        int nElements = tmpList.size();

        for (int j = 0; j < nElements; ++j) {
            listFloatIndex.push_back(std::pair<float, int>(tmpList.at(j), i));
        }
    }

    return listFloatIndex;
}

/*
 * parse a string with run, lumi, and event info
 */
CONFIGPARSER::RunLumiEvent ConfigurationParser::ParseRunLumiEvent(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle;
    rle.run = 0;
    rle.lumi = 0;
    rle.event = 0;

    if(strRunLumiEvent.empty())
        return rle;

    // by default run, lumi, and event are separated by " "
    std::string separator = " ";
    // if the string contains ":", then it becomes the default separator.
    if (strRunLumiEvent.find(":") != std::string::npos)  separator = ":";

    std::vector<std::string> vRunLumiEvent = split(strRunLumiEvent, separator);

    int vSize = vRunLumiEvent.size();
    unsigned int run, lumi;
    unsigned long long event;
    if (vSize == 3) {
        std::istringstream sin0(vRunLumiEvent.at(0));
        std::istringstream sin1(vRunLumiEvent.at(1));
        std::istringstream sin2(vRunLumiEvent.at(2));

        sin0 >> run;
        sin1 >> lumi;
        sin2 >> event;

        rle.run = run;
        rle.lumi = lumi;
        rle.event = event;
    }
    else if (vSize == 2) {
        std::istringstream sin0(vRunLumiEvent.at(0));
        std::istringstream sin1(vRunLumiEvent.at(1));

        // if there are 2 numbers, then assume those are run and event numbers
        sin0 >> run;
        sin1 >> event;

        rle.run = run;
        rle.event = event;
    }

    return rle;
}

unsigned int ConfigurationParser::ParseRunNumber(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.run;
}

unsigned int ConfigurationParser::ParseLumiNumber(std::string strRunLumiEvent)
{
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.lumi;
}

unsigned long long ConfigurationParser::ParseEventNumber(std::string strRunLumiEvent){
    CONFIGPARSER::RunLumiEvent rle = ParseRunLumiEvent(strRunLumiEvent);
    return rle.event;
}

/*
 * list[0].at(i);   nBins for the ith TH1D histogram
 * list[1].at(i);   xLow  for the ith TH1D histogram
 * list[2].at(i);   xUp   for the ith TH1D histogram
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTH1D_Bins(std::string strList)
{

    std::vector<std::vector<float>> list(3);

    std::vector<float> listFlat = ParseListFloat(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<float>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
    }

    return list;
}

std::vector<CONFIGPARSER::TH1Axis> ConfigurationParser::ParseListTH1D_Axis(std::string strList)
{
    std::vector<CONFIGPARSER::TH1Axis> list;

    // split TH1D axis information by ";;"
    std::vector<std::string> strListTH1D_Axis = ParseList(strList, CONFIGPARSER::separator2.c_str());

    for (std::vector<std::string>::iterator it = strListTH1D_Axis.begin() ; it != strListTH1D_Axis.end(); ++it) {

        std::string strTH1D_Axis = (*it);

        CONFIGPARSER::TH1Axis th1Axis;

        if (isTH1D_BinsArray(strTH1D_Axis)) {
            strTH1D_Axis = replaceAll(strTH1D_Axis, "[", "{");
            strTH1D_Axis = replaceAll(strTH1D_Axis, "]", "}");
            // listBinEdges is an array of nBins+1 numbers where
            // 1. the first "nBins" of elements are bin low edges
            // 2. the last element is the last bin upper edges.
            std::vector<float> listFloat = ParseListFloat(strTH1D_Axis);
            std::vector<double> listBinEdges(listFloat.begin(), listFloat.end());

            th1Axis.nBins = listBinEdges.size()-1;
            th1Axis.xLow = listBinEdges.at(0);
            th1Axis.xUp = listBinEdges.at(listBinEdges.size()-1);
            th1Axis.bins = listBinEdges;
            th1Axis.binsAreFromUser = true;
        }
        else {
            std::vector<float> listTmp = ParseListWithoutBracketFloat(strTH1D_Axis);
            if (listTmp.size() == 3) {  // listTmp = { nBins, xLow, xUp }

                th1Axis.nBins = listTmp[0];
                th1Axis.xLow = listTmp[1];
                th1Axis.xUp = listTmp[2];
                th1Axis.binsAreFromUser = false;
                th1Axis.setBins();
            }
        }

        list.push_back(th1Axis);
    }

    return list;
}

/*
 * list[0].at(i);   nBinsx for the ith TH2D histogram
 * list[1].at(i);   xLow   for the ith TH2D histogram
 * list[2].at(i);   xUp    for the ith TH2D histogram
 * list[3].at(i);   nBinsy for the ith TH2D histogram
 * list[4].at(i);   yLow   for the ith TH2D histogram
 * list[5].at(i);   yUp    for the ith TH2D histogram
 */
std::vector<std::vector<float>> ConfigurationParser::ParseListTH2D_Bins(std::string strList)
{

    std::vector<std::vector<float>> list(6);

    std::vector<float> listFlat = ParseListFloat(strList);
    if (listFlat.size() % 6 != 0)   return list;

    for (std::vector<float>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=6) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
        list[3].push_back(*(it+3));
        list[4].push_back(*(it+4));
        list[5].push_back(*(it+5));
    }

    return list;
}

/*
 * assumes that different TH2DAxis info is split by ";;;"
 * assumes that xAxis and yAxis info is split by ";;"
 */
std::vector<CONFIGPARSER::TH2DAxis> ConfigurationParser::ParseListTH2D_Axis(std::string strList)
{
    std::vector<CONFIGPARSER::TH2DAxis> list;

    // split TH2D axis information by ";;;"
    std::vector<std::string> strListTH2D_Axis = ParseList(strList, CONFIGPARSER::separator3.c_str());

    for (std::vector<std::string>::iterator it = strListTH2D_Axis.begin() ; it != strListTH2D_Axis.end(); ++it) {

        std::string strTH2D_Axis = (*it);

        // change the string such that it is a list of 2 TH1Axis objects
        std::vector<CONFIGPARSER::TH1Axis> listTH1Axis = ParseListTH1D_Axis(Form("{ %s }", strTH2D_Axis.c_str()));

        CONFIGPARSER::TH2DAxis th2DAxis;
        if (listTH1Axis.size() == 2) {
            th2DAxis.axisX = listTH1Axis.at(0);
            th2DAxis.axisY = listTH1Axis.at(1);
        }

        list.push_back(th2DAxis);
    }

    return list;
}

std::string ConfigurationParser::verboseTH1D_Axis(CONFIGPARSER::TH1Axis th1Axis)
{
    std::string res;

    if (th1Axis.nBins > 0) {
        res.append(Form("{ %d, ", th1Axis.nBins));
        res.append(Form("%f, ", th1Axis.xLow));
        res.append(Form("%f }", th1Axis.xUp));

        if (th1Axis.binsAreFromUser) {

            res.append(", [ ");

            for (int iBin=0; iBin < th1Axis.nBins; ++iBin) {
                res.append(Form("%.2f, ", th1Axis.bins.at(iBin)));
            }
            res.append(Form("%.2f ]", th1Axis.bins.at(th1Axis.nBins)));
        }
    }

    return res;
}

std::string ConfigurationParser::verboseTH2D_Axis(CONFIGPARSER::TH2DAxis th2DAxis)
{
    std::string resX = verboseTH1D_Axis(th2DAxis.axisX);
    std::string resY = verboseTH1D_Axis(th2DAxis.axisY);

    std::string res = Form("xAxis : %s, yAxis : %s", resX.c_str(), resY.c_str());

    return res;
}

/*
 * parse the Latex syntax in the configuration file to the Latex syntax in ROOT
 */
std::string ConfigurationParser::ParseLatex(std::string str)
{
    std::string strNew = replaceAll(str, "\\", "#");
    return strNew;
}

/*
 * parse a list of Latex strings
 */
std::vector<std::string> ConfigurationParser::ParseListLatex(std::string strList, std::string separator)
{
    std::vector<std::string> list;

    if(strList.empty())
        return list;

    size_t posStart = 0;
    size_t posEnd   = strList.size();

    if (separator.size() == 0) {
        // by default, elements of the list are separated by ","
        separator = CONFIGPARSER::separator1.c_str();
        // allow "," to be a list element
        // if one wants to use comma inside a list element, then one should use ";;" as element separator
        if (strList.find(CONFIGPARSER::separator2.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator2.c_str();
        // if one wants to use ";;" inside a list element for lower-level separation, then one should use ";;;" as element separator
        if (strList.find(CONFIGPARSER::separator3.c_str()) != std::string::npos)  separator = CONFIGPARSER::separator3.c_str();
    }

    size_t pos;
    bool listFinished = false;
    while (!listFinished) {
        pos = strList.find(separator, posStart);
        if (pos == std::string::npos) {    // this must be the last element.
            pos = posEnd;
            listFinished = true;
        }

        std::string tmp = strList.substr(posStart, pos-posStart);  //  strList = ...,ABC123 ,... --> posStart = 0, pos = 8, tmp = "ABC123 "

        // make trimming optional, trimming can be skipped if "noTrim" key appears in the element
        bool doTrim = (tmp.find(CONFIGPARSER::noTrim) == std::string::npos);
        std::string element;
        if (doTrim) {
            element = trim(tmp);
        } else {  // do not trim, allow  leading and trailing white space characters to appear in the element.
            // remove the "do not trim" option from the element
            element = replaceAll(tmp, CONFIGPARSER::noTrim, "");
        }

        if (element.size() > 0)
            list.push_back(ConfigurationParser::ParseLatex(element.c_str()));
        posStart = pos + separator.size();
    }

    return list;
}

/*
 * list[0].at(i);   formula for the ith TF1 function
 * list[1].at(i);   xMin    for the ith TF1 function
 * list[2].at(i);   xMax    for the ith TF1 function
 */
std::vector<std::vector<std::string>> ConfigurationParser::ParseListTF1(std::string strList)
{
    std::vector<std::vector<std::string>> list(3);
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list[0].push_back(*it);
        list[1].push_back(*(it+1));
        list[2].push_back(*(it+2));
    }

    return list;
}

/*
 * list.at(i);   formula for the ith TF1 function
 */
std::vector<std::string> ConfigurationParser::ParseListTF1Formula(std::string strList)
{
    std::vector<std::string> list;
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        list.push_back(*it);
    }

    return list;
}

/*
 * list[0].at(i);   xMin for the ith TF1 function
 * list[0].at(i);   xMax for the ith TF1 function
 */
std::vector<std::vector<double>> ConfigurationParser::ParseListTF1Range(std::string strList)
{
    std::vector<std::vector<double>> list(2);
    std::vector<std::string> listFlat = ParseList(strList);
    if (listFlat.size() % 3 != 0)   return list;

    for (std::vector<std::string>::iterator it = listFlat.begin() ; it != listFlat.end(); it+=3) {
        std::string str1 = *(it+1);
        std::string str2 = *(it+2);

        list[0].push_back(std::stod(str1));
        list[1].push_back(std::stod(str2));
    }

    return list;
}

#endif
