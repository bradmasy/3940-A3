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

string ServerThread::readSocket()
{
    char character[1];
    string fileData;
    int r;

    cout << "START OF READSOCKET LOOP" << endl;

    while (r = recv(msgsocket->getSocket(), &character, 1, 0) == 1)
    {
        cout << *character << endl;
        fileData += *character;
        // cout << "R VAL: " << r << endl;
    }

    //     char* requestPtr = (char*)malloc(fileData.length() + 1);
    //     char arr[fileData.length()];
    //     strcpy(arr, fileData.c_str());

    //   //  cout << "FILE DATA LOOP" << endl;
    //     for(int i = 0; i < fileData.length(); i++){
    //         requestPtr[i] = arr[i];
    //   //      cout << "file character: " << requestPtr[i] <<endl;;

    //     }

    // requestPtr[fileData.length()] = '\0';
    // cout<<"---------------------------------------"<<endl;
    // cout << requestPtr << endl;

    cout << "END OF READSOCKET LOOP" << endl;

    return fileData;
}

void ServerThread::runMethod(string &method, Response *res, Request *req, Servlet &up)
{
    if (method == "GET")
    {
        cout << "-------------------------------2. METHOD: " << method << endl;
        int responseInt = up.get(*res, *req);

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

string ServerThread::run()
{
    // char buffer[1024 * 1024];
    cout << "here" << endl;
    // string data = readSocket();
    //  renderHTML();

    char buf1[526336];
    char buf[1];
    int i = 0;
    int rval;
    string patternGet = "GET";
    string patternPost = "POST";
    string patternBoundary = "WebKitFormBoundary"; // "boundary=----";
    string pattern = "\r\n\r\n";

    char buf2[526336];
    bool checkBoundary = false;
    int j = 0;

    bool isGet = false;
    bool isPost = false;

    bool startByteCode = false;
    bool endByteCode = false;
    string boundaryCheckString = "";

    while (((rval = read(msgsocket->getSocket(), buf, 1)) == 1))
    {
        buf1[i] = *buf;

        i++;
        cout << *buf;
        string b{buf1};

        if (checkBoundary == true)
        {
            buf2[j] = *buf;
            j++;
        }

        if(b.find(patternGet) != std::string::npos){
            isGet = true;

        }

        if (b.find(patternPost) != std::string::npos)
        {
            isPost = true;


            // cout << "--------------------------------------------POST IS HAPPENING --------------------------------------------" << endl;
        }
    
        if (isGet)
        {
            if (b.find(pattern) != std::string::npos)
            {
                cout << "BREAKING THE LOOP" << endl;
                break;
            }
        }


        if (b.find("Content-Type: image/") != std::string::npos){
            startByteCode = true;
        }

        // if (startByteCode == true) {
        //     imageByteCode[byteCodeIndex] = *buf;
        //     byteCodeIndex++;
        // } else {
            
        // }

        //cout << "BUFFER:---------------------------"<< b << endl ;
        if((b.find("boundary=---------------------------") != std::string::npos) && (isPost == true)){
            
            cout << "READING BYTE CODE" << endl;
            // looping here until end of line
            
            // char imageByteCode[526336];
            // int byteCodeIndex = 0;

            // char byteBuf[1];
            // cout << "-----BYTE CODE-----" << endl;
            // while (((rval = read(msgsocket->getSocket(), byteBuf, 1)) == 1)) {
            //     imageByteCode[byteCodeIndex] = *byteBuf;
            //     cout << *byteBuf;
                
            //     if(*byteBuf == '\n'){
            //         break;
            //     }

            //     byteCodeIndex++;
            // }

            //             cout << "-----END OF BYTE CODE-----" << endl;



            
            // break;
        }
    }
    buf[i] = '\0';

    cout << "BUF 2: " << buf2 << endl;

    string data = buf1;
    istringstream requestInfo(data.c_str());
    istringstream *reqPtr = &requestInfo;

    cout << "------------------REQ:----------------" << endl;
    cout << data << endl;
    cout << "------------------REQ:----------------" << endl;

    ostringstream *outputWriter = new ostringstream();
    response = new Response(msgsocket->getSocket(), outputWriter);
    request = new Request(reqPtr);

    UploadServlet *up = new UploadServlet{msgsocket->getSocket()};

    string requestMethod = request->getReqMethod();
    // cout << "METHOD: [" << requestMethod << "]" << endl;

    runMethod(requestMethod, response, request, *up);

    string locationOfRequest = request -> getUserAgent();

    // if (locationOfRequest == "browser")
    // {
    //     up = (UploadServlet) new UploadServlet();
    // }
    // else if (locationOfRequest == "cli")
    // {
    //     // up = new ClientServlet();
    // }



    runMethod(requestMethod, response, request, *up);



    // send(msgsocket,get_http, strlen(get_http.c_str()),0 );
   return requestMethod;
}
