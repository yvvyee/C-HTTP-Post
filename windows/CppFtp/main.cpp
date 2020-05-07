#include <windows.h>
#include <wininet.h>
#include <atlstr.h>
#include <iostream>
#pragma comment(lib, "wininet")

CString ip = L"eileenyoo.cafe24.com";
CString id = L"eileenyoo";
CString pw = L"sapere4ude";

int send_file(CString src_path, CString fname) {

    HINTERNET hInternet;
    HINTERNET hFtpSession;
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, id, pw, INTERNET_SERVICE_FTP, 0, 0);
    if (hFtpSession) {
        if (FtpPutFile(hFtpSession, src_path, fname, FTP_TRANSFER_TYPE_BINARY, 0)) {
            std::cout << "Successfully uploaded to FTP Server" << std::endl;
        }
        else {
            std::cout << "Failed" << std::endl;
        }
    }
    InternetCloseHandle(hFtpSession);
    InternetCloseHandle(hInternet);

    return 0;
}

int main() {
    CString src_path = L"C:\\Temp\\test.txt";
    CString fname = L"ABCD.txt";
    
    send_file(src_path, fname);

    return 0;
}