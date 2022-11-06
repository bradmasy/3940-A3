#include "ServerThread.hpp"

ServerThread::ServerThread(Socket *msgSock) : Thread(this)
{
    this->msgsocket = msgSock;
}

int ServerThread::renderHTML()
{
    string html = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 600\n\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title> File Upload Form</title>\r\n</head>\r\n<body>\r\n<h1>Upload file</h1>\r\n<form id =\"form\" method=\"POST\" action=\"/\" enctype=\"multipart/form-data\">\r\n<input type=\"file\" name=\"fileName\"/><br/><br/>\r\nCaption: <input type =\"text\" name=\"caption\"<br/><br/>\r\n <br/>\nDate : <input type=\"date\" name=\"date\"<br/><br/>\r\n <br/>\n <input id='formBtn' type=\"submit\" name=\"submit\" value=\"Submit\"/>\r\n </form>\r\n</body>\r\n</html>\r\n";

    char arr[200] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";

    int send_res = send(msgsocket->getSocket(), html.c_str(), html.length(), 0); //
    // do a response...
    return send_res;
}

void ServerThread::runMethod(string &method, Response *res, Request *req, Servlet &up)
{
    if (method == "GET")
    {
        cout << "-------------------------------2. METHOD: " << method << endl;
        int responseInt = up.get(*res, *req);

        // if (responseInt != -1) {

        // }

        // char buffer[1024 * 1024];
        // int receiveInt = recv(up.getSocket(), &buffer, 1024 * 1024, 0);
        cout << "Response int: " << responseInt << endl;
    }
    else
    {
        cout << "-------------------------------3. METHOD: " << method << endl;
        up.get(*res, *req);
        up.post(*res, *req);
    }

    cout << "in method call" << endl;
}

void ServerThread::readSocket()
{
    char character[1];
    string fileData;

    while (read(msgsocket->getSocket(), character, 1))
    {
        cout << *character;
        fileData += *character;
    }


    char* requestPtr = (char*)malloc(fileData.length() + 1);
    char arr[fileData.length()];
    strcpy(arr, fileData.c_str());

    // requestPtr = fileData.c_str();
    cout << "FILE DATA LOOP" << endl;
    for(int i = 0; i < fileData.length(); i++){
        requestPtr[i] = arr[i];
        cout << "file character: " << requestPtr[i] <<endl;;
        
    }

    requestPtr[fileData.length()] = '\0';
    cout<<"---------------------------------------"<<endl;
    cout << requestPtr << endl;
}

string ServerThread::run()
{
    // char buffer[1024 * 1024];
    char character[1];
    string payload;
    string userAgent;

    // writes to the socket
    // recv(msgsocket->getSocket(), buffer, 1024 * 1024, 0);

    bool endOfData = false;
    string reqType;
    string caption;
    string date;
    string boundary;
    string endofFile;
    int endIndex;

    int index = 0;

    cout << "Before RECV:" << endl;
    while ((read(msgsocket->getSocket(), character, 1)) && (!endOfData))
    {
        cout << "character: " << character << endl;
        // cout << *character;
        payload += character;
        if (payload.find("POST") != std::string::npos || payload.find("GET") != std::string::npos)
        {
            reqType = payload;
            cout << "--------------------REQ TYPE ASSIGNED-------------------------:" << reqType << endl;
        }
        // No Boundary in GET Requests..
        if (payload.find("boundary=") != std::string::npos && reqType == "POST")
        {
            index = payload.find("boundary=");
            endIndex = index;
            while (payload[endIndex] != '\n')
            {
                endIndex++;
            }
            endofFile += "--";
            endofFile += boundary;
            endofFile += "--";
            boundary = payload.substr(index, endIndex);
            cout << boundary << endl;
        }
        if (payload.find("User-Agent:") != std::string::npos)
        {
            index = payload.find("User-Agent: ");
            endIndex = index;
            while (payload[endIndex] != '\n')
            {
                endIndex++;
            }
            userAgent = payload;
            cout << userAgent << endl;
        }
        if (payload.find("name=\"caption\"\n\n") != std::string::npos && reqType == "POST")
        {
            index = payload.find("name=\"caption\"\n\n");
            index += 16;
            endIndex = index;
            while (payload[endIndex] != '\n')
            {
                endIndex++;
            }
            caption = payload.substr(index, endIndex);
            cout << caption << endl;
        }
        if (payload.find("name=\"date\"\n\n") != std::string::npos && reqType == "POST")
        {
            index = payload.find("name=\"date\"\n\n");
            index += 13;
            endIndex = index;

            while (payload[endIndex] != '\n')
            {
                endIndex++;
            }

            date = payload.substr(index, endIndex);
            endOfData = true;
            cout << date << endl;
        }
        if (payload.find(endofFile) != std::string::npos)
        { // Last line
            endOfData = true;
        }
    }
    cout << "Payload: " << payload << endl;

    // istringstream requestInfo = buffer.c_str();
    // istringstream requestInfo(buffer);
    // istringstream requestInfo = payload.c_str();
    istringstream requestInfo;
    istringstream *reqPtr = &requestInfo;

    // Servlet *up = nullptr;

    cout << "-----------SERVER THREAD 1----------" << endl;
    UploadServlet *up = new UploadServlet{msgsocket->getSocket()};
    cout << "-----------SERVER THREAD 2----------" << endl;

    ostringstream *outputWriter = new ostringstream();
    cout << "-----------SERVER THREAD 3----------" << endl;

    response = new Response(msgsocket->getSocket(), outputWriter);
    cout << "-----------SERVER THREAD 4----------" << endl;

    request = new Request(&requestInfo);
    cout << "-----------SERVER THREAD 5----------" << endl;

    string requestMethod = request->getReqMethod();
    cout << "METHOD: [" << requestMethod << "]" << endl;

    return requestMethod;
}
