#include <sstream>

#include "utilities.hpp"

std::string Utilities::readFirstString(std::string& thisString)
{
    size_t spacePos = thisString.find(' ');
    std::string tmpStr = thisString.substr(0, spacePos);

    if (spacePos == std::string::npos)
    {
        thisString.clear();
    }
    else
    {
        thisString.erase(0, spacePos + 1);
    }

    return tmpStr;
}

std::vector<int> Utilities::readNumber(std::string& thisString, int numberOfNumber)
{
    std::vector<int> listOfNumber;

    for (int i = 0; i < numberOfNumber; ++i)
    {
        listOfNumber.push_back(stringToInt(readFirstString(thisString)));
    }

    return listOfNumber;
}

std::vector<std::string> Utilities::readString(std::string& thisString, int numberOfString)
{
    std::vector<std::string> listOfString;

    for (int i = 0; i < numberOfString; ++i)
    {
        listOfString.push_back(readFirstString(thisString));
    }

    return listOfString;
}

std::string Utilities::intToString(int thisInt)
{
    std::stringstream ss;

    ss << thisInt;

    return ss.str();
}

int Utilities::stringToInt(std::string thisString)
{
    std::istringstream iss(thisString);
    int tmpInt;

    iss >> tmpInt;

    return tmpInt;
}

double Utilities::stringToDouble(std::string thisString)
{
    std::istringstream iss(thisString);
    double tmpDouble;

    iss >> tmpDouble;

    return tmpDouble;
}

std::string& Utilities::addNewLine(std::string& thisString)
{
    size_t pos = 0;
    while ((pos = thisString.find("\\n", pos)) != std::string::npos)
    {
        thisString.replace(pos, 2, "\n");
        pos += 1;
    }

    return thisString;
}

bool Utilities::doubleIsNear(double baseVal, double compareVal)
{
    return ((baseVal - minDoublePrec) < compareVal && (baseVal + minDoublePrec) > compareVal);
}
