#include "Request.hpp"

Request::Request(){};

Request::Request(istringstream *inStream)
{
    inputStream = inStream;
    parsePayload(inputStream);
    FormDataMap = map<string,string>();
}

void Request::loopLine(string line, map<string, string> FormDataMap)
{
    auto it = line.begin();
    string key = "";
    string value = "";
    bool flag = false;

    while (it != line.end())
    {
        if ((*it == ':' || *it == '/') && !flag)
        {
            it++;
            flag = true;
        }
        else
        {
            if (flag)
            {
                if (*it == '\r' || *it == '\n')
                {
                    break;
                }
                else
                {
                    value += *it;
                }
            }
            else
            {
                key += *it;
                if (key == "GET")
                {
                    reqType = "GET";
                    key = "Request";
                    value = "GET";
                    break;
                }
            }
        }
        it++;
    }

    //auto pair = make_pair(key, value);
    FormDataMap.insert({key,value});
    //FormDataMap.insert(pair);    

}

string Request::getReqMethod()
{

    cout << "in map" << endl;

    auto it = FormDataMap.begin();

    while(it != FormDataMap.end()){
        cout << "first: " << it->first <<endl;
        cout<< "second: " << it->second << endl; 
        it++;

    }

    // auto pair = FormDataMap.find("Request");
    // string request = pair->second;
    // cout << "Request type: " <<request << endl;

    return reqType;
}

void Request::parsePayload(istringstream *inStream)
{
    string savedByteCode = "";
    string temp = "";
    string dataKeys[] = {"Date", "Keyword", "Caption", "File"};
    int currentKey = 0;
    int maxKey = 4;
    bool endOfDataReached = false;
    int totalLengthOfPayload = 0;

    while (*inStream >> temp)
    {
        totalLengthOfPayload += temp.size();
    }

    cout << "Temp size: " << totalLengthOfPayload << endl;

    int currentLength = 0;
    inStream->clear();
    inStream->seekg(0);

    string requestType;

    string title;
    string value;

    char c;
    string line;

    while (getline(*inStream, line))
    {

        loopLine(line, FormDataMap); // loops each line and parses it into a pair.

        int tokenLength = temp.size();

        if (temp.find("User-Agent:"))
        {
            reqUserAgent = "browser";
        }

        if (currentLength == totalLengthOfPayload)
        {
            break;
        }

        currentLength += tokenLength;
    }
}