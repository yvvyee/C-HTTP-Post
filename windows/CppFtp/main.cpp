#include <windows.h>
#include <wininet.h>
#include <atlstr.h>
#include <iostream>
#pragma comment(lib, "wininet")

int main() {
    CString ip = L"";
    CString id = L"";
    CString pw = L"";

    CString src_path = L"C:\\Temp\\input2.txt"; // 전송하고자 하는 파일의 경로
    CString dest_fname = L"MyFtpTest.txt";      // 서버에 저장될 파일 이름

    HINTERNET hInternet;
    HINTERNET hFtpSession;
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, id, pw, INTERNET_SERVICE_FTP, 0, 0);
    if (hFtpSession) {
        if (FtpPutFile(hFtpSession, src_path, dest_fname, FTP_TRANSFER_TYPE_BINARY, 0)) {
            std::cout << "성공적으로 업로드 완료됨" << std::endl;
        }
        else {
            std::cout << "업로드 실패" << std::endl;
        }
    }
    InternetCloseHandle(hFtpSession);
    InternetCloseHandle(hInternet);

    return 0;
}