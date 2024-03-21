#include <iostream>
#include <Windows.h>
#include <conio.h> // For _kbhit and _getch
#include "render.h"
#include "grdv/global.h"
#include "grdv/binary/dropper.h"
#include "Includes1.h"
#include "Windows.h"
#include <thread>
#include "stdio.h"
#include <random>
#include <psapi.h>


#include <fstream>
#include <vector>
#include <sstream>


#pragma comment (lib, "ni0h-AntiDebug.lib")    //include .ni0h anti-debugging library
#include "LoadDrv.h"
#include "Security/AntiDebug.hpp"
#include "Security/WindowsAPI.hpp"



using namespace std;
using namespace KeyAuth;
using json = nlohmann::json;

std::string name = skCrypt("Vox FN Private").decrypt();
std::string ownerid = skCrypt("q2PUNgNQ3y").decrypt();
std::string secret = skCrypt("ef5131d10a0a4b97077b2133e8efad1383d1776c79fbec351f7dcfea3c30910c").decrypt();
std::string version = skCrypt("1.0").decrypt();
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt();


api KeyAuthApp(name, ownerid, secret, version, url);

typedef HANDLE(WINAPI* pCreateFile)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL(WINAPI* pWriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef BOOL(WINAPI* pCloseHandle)(HANDLE);

void initialize();
void findFortniteWindow();
void injectAndAttach();
void startEntityCachingThread();
void changeExecutableName(); // Function to change the name of the executable file


HWND fortniteWindow = NULL;


void PrintBool(const char* message, bool value) {
    std::cout << message << ": " << (value ? "Detected" : "Not Detected") << std::endl;
}

// Function to generate a random driver name
std::string generateRandomFileName() {
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(nullptr)));

    // Generate a random number
    int randomNumber = rand();

    // Convert the random number to a string
    std::string randomFileName = "";
    for (int i = 0; i < 10; ++i) {
        randomFileName += ('A' + rand() % 26);
    }

    // Add the ".sys" extension to the file name
    randomFileName += ".sys";

    return randomFileName;
}

bool Windows_HasDetectedProcessDebugFlags()
{
    if (HMODULE nttdll = GetModuleHandleA("ntdll.dll"))
    {
        using fpNtQueryInformationProcess = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
        auto NtQueryInformationProcess = (fpNtQueryInformationProcess)GetProcAddress(nttdll, "NtQueryInformationProcess");

        ULONG processDebugFlags = NULL;
        if (NtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)0x1F, &processDebugFlags, sizeof(ULONG), NULL) == STATUS_SUCCESS)
        {
            return processDebugFlags == NULL;
        }
    }

    return false;
}

bool Windows_HasDetectedProcessDebugObject()
{
    if (HMODULE nttdll = GetModuleHandleA("ntdll.dll"))
    {
        using fpNtQueryInformationProcess = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
        auto NtQueryInformationProcess = (fpNtQueryInformationProcess)GetProcAddress(nttdll, "NtQueryInformationProcess");

        HANDLE processDebugObjectHandle = NULL;
        if (NtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)0x1E, &processDebugObjectHandle, sizeof(HANDLE), NULL) == STATUS_SUCCESS)
        {
            return processDebugObjectHandle != NULL;
        }
    }

    return false;
}

bool Windows_HasDetectedHardwareBreakpoints()
{
    bool result = false;

    if (PCONTEXT context = (PCONTEXT)VirtualAlloc(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE))
    {
        context->ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if (GetThreadContext(GetCurrentThread(), context))
        {
            //not sure if should also check Dr6 and Dr7
            result = (context->Dr0 != 0ull || context->Dr1 != 0ull || context->Dr2 != 0ull || context->Dr3 != 0ull);
        }

        VirtualFree(context, 0, MEM_RELEASE);
    }

    return result;
}

//This function using normal windows API to get parent process name. Debugger-hide tools can spoof this to looks like its opened without any debugger.
wchar_t* Windows_GetParentProcessFileName()
{
    DWORD pid = GetCurrentProcessId();
    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32W);
    wchar_t* result = nullptr;

    if (h != INVALID_HANDLE_VALUE)
    {
        if (Process32FirstW(h, &pe))
        {
            do
            {
                if (pe.th32ProcessID == pid)
                {
                    if (HANDLE parentProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ParentProcessID))
                    {
                        result = new wchar_t[MAX_PATH] {};
                        GetModuleFileNameExW(parentProcess, NULL, result, MAX_PATH);
                    }

                    break;
                }
            } while (Process32NextW(h, &pe));
        }

        CloseHandle(h);
    }

    return result;
}

// Function to display a message box
void ShowMessageBox(const wchar_t* message)
{
    MessageBoxW(NULL, message, L"Debugger Detected", MB_ICONEXCLAMATION | MB_OK);
}




auto main() -> int {


    
    changeExecutableName();
 


        //////////////////////////////////////////////////////////////  HADES  ///////////////////////////////////////////////////////////////


        BOOL hadesRemoteDebug = FALSE;
        if (Hades::WindowsAPI::CheckRemoteDebuggerPresent(Hades::WindowsAPI::GetCurrentProcess(), &hadesRemoteDebug)) //call CheckRemoteDebuggerPresent with Hades. Not recommended, attackers can still spoof it
        {

            if (hadesRemoteDebug)
            {
                ShowMessageBox(L"A debugger is currently running");
                exit(1); // exit the application with error code 1
            }
        }


        if (wchar_t* debugName = Hades::AntiDebug::GetParentProcessFileName())
        {
            std::wcout << "Hades - GetParentProcessFileName: " << debugName << std::endl;
            delete[] debugName;
            //ShowMessageBox(L"A debugger is currently running.");
           //exit(1); // exit the application with error code 1
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << std::endl << std::endl;

        ////////////////////////////////////////////////////////////  WINDOWS  ////////////////////////////////////////////////////////////////

        BOOL normalRemoteDebug = FALSE;
        if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &normalRemoteDebug)) //call CheckRemoteDebuggerPresent normally
        {

            if (normalRemoteDebug)
            {
                ShowMessageBox(L"A debugger is currently running.");
                exit(1); // exit the application with error code 1
            }
        }


        if (wchar_t* debugName = Windows_GetParentProcessFileName())
        {
            std::wcout << "Windows - parent process name: " << debugName << std::endl;
            delete[] debugName;
            //ShowMessageBox(L"A debugger is currently running.");
            //exit(1); // exit the application with error code 1
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << std::endl << std::endl;

        bool windowsDetectedDebug = Windows_HasDetectedProcessDebugFlags() || Windows_HasDetectedProcessDebugObject();
        bool hadesDetectedDebug = Hades::AntiDebug::HasDetectedProcessDebugFlags() || Hades::AntiDebug::HasDetectedProcessDebugObject();

        if (hadesDetectedDebug || windowsDetectedDebug)
        {
            std::cout << "Debugger-hide tool/s detected" << std::endl;
            ShowMessageBox(L"A debugger is currently running.");
            exit(1); // exit the application with error code 1
        }

    

    initialize();
    findFortniteWindow();
    injectAndAttach();
    startEntityCachingThread(); 


    Overlay->hijack();
    Overlay->imgui();
    Overlay->render();

    return 0;
}


void initialize() {

    // For KeyAuth Checks
    bool loggedIn = false;

    // KeyAuth start
    KeyAuthApp.init();

    // Console Sytle
    system("MODE CON COLS=75 LINES=12");
    SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 195, LWA_ALPHA);


    // Console Misc
    Misc::MoveCenter();
    Misc::RemoveExtras();
    Misc::DisableTaskMgr();

    // Console Tittle
    std::string consoleTitle = (std::string)skCrypt("");
    SetConsoleTitleA(consoleTitle.c_str());

    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);


    while (true)
    {
 

        ///////////////////////////////////////////////////////////////  HADES  ///////////////////////////////////////////////////////////////


        BOOL hadesRemoteDebug = FALSE;
        if (Hades::WindowsAPI::CheckRemoteDebuggerPresent(Hades::WindowsAPI::GetCurrentProcess(), &hadesRemoteDebug)) //call CheckRemoteDebuggerPresent with Hades. Not recommended, attackers can still spoof it
        {

            if (hadesRemoteDebug)
            {
                ShowMessageBox(L"A debugger is currently running");
                exit(1); // exit the application with error code 1
            }
        }


        if (wchar_t* debugName = Hades::AntiDebug::GetParentProcessFileName())
        {
            std::wcout << "Hades - GetParentProcessFileName: " << debugName << std::endl;
            delete[] debugName;
            //ShowMessageBox(L"A debugger is currently running.");
           //exit(1); // exit the application with error code 1
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << std::endl << std::endl;

        ////////////////////////////////////////////////////////////  WINDOWS  ////////////////////////////////////////////////////////////////

        BOOL normalRemoteDebug = FALSE;
        if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &normalRemoteDebug)) //call CheckRemoteDebuggerPresent normally
        {

            if (normalRemoteDebug)
            {
                ShowMessageBox(L"A debugger is currently running.");
                exit(1); // exit the application with error code 1
            }
        }


        if (wchar_t* debugName = Windows_GetParentProcessFileName())
        {
            std::wcout << "Windows - parent process name: " << debugName << std::endl;
            delete[] debugName;
            //ShowMessageBox(L"A debugger is currently running.");
            //exit(1); // exit the application with error code 1
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << std::endl << std::endl;

        bool windowsDetectedDebug = Windows_HasDetectedProcessDebugFlags() || Windows_HasDetectedProcessDebugObject();
        bool hadesDetectedDebug = Hades::AntiDebug::HasDetectedProcessDebugFlags() || Hades::AntiDebug::HasDetectedProcessDebugObject();

        if (hadesDetectedDebug || windowsDetectedDebug)
        {
            std::cout << "Debugger-hide tool/s detected" << std::endl;
            ShowMessageBox(L"A debugger is currently running.");
            exit(1); // exit the application with error code 1
        }
   
        system("cls");

        ////////////////////////////////////////////////////////////  END HADES  ////////////////////////////////////////////////////////////////


        // Main Code
        colorize::coolPrint(std::string(skCrypt("Connecting to Server...")), 1);

        Sleep(4000);
        system("cls");
        Sleep(2000);

        // Check if Key.json exists and read the key
        if (std::filesystem::exists("Key.json")) {
            std::string key = ReadKeyFromJson("Key.json", "license");
            if (!key.empty()) {
                KeyAuthApp.license(key);
                if (KeyAuthApp.data.success) {
                    colorize::coolPrint(std::string(skCrypt("Successfully Automatically Logged In")), 1);
                    loggedIn = true; // Set the loggedIn flag to true
                }
                else {
                    std::remove("Key.json");
                    std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
                    Sleep(1500);
                }
            }
        }

        Sleep(4000);
        system("cls");
        Sleep(2000);

        // If not logged in, ask the user for the key | first check
        if (!loggedIn) {

            std::string key;
            colorize::coolPrint(std::string(skCrypt("Key:")), 1);
            std::cin >> key;
            KeyAuthApp.license(key);
            if (KeyAuthApp.data.success) {
                // Save the key for auto-login next time
                WriteKeyToJson("Key.json", "license", key);
                colorize::coolPrint(std::string(skCrypt("Successfully Saved Key for Auto Login")), 1);
            }
            else {
                std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
                Sleep(1500);
                exit(1);
            }
        }

        // Check if Key.json exists second check
        if (std::filesystem::exists("Key.json")) {
            std::ifstream file("Key.json");
            json j;
            try {
                file >> j; // Parse the JSON file
            }
            catch (json::parse_error& e) {
                std::cout << skCrypt("Parsing error: ") << e.what() << std::endl;
                exit(1);
            }

            // Check if the key exists and validate it
            if (j.contains("license")) {
                std::string key = j["license"];
                if (validateKey(key)) {
                    system("cls");
                    // Key is valid, proceed with your logic
                    std::cout << skCrypt("[ AL ] Valid key found: ") << key << std::endl;
                    Sleep(4000);
                    system("cls");
                    Sleep(2000);
                    std::cout << skCrypt("[ AL ] Please wait ") << std::endl;
                    Sleep(2000);
                    system("cls");

                    
                }
                else {
                    // Key is not valid, exit the application
                    colorize::coolPrint(std::string(skCrypt("Invalid key. Exiting application.")), 1);
                    exit(1);
                }
            }
            else {
                // Key does not exist, exit the application
                colorize::coolPrint(std::string(skCrypt("Key does not exist in JSON. Exiting application.")), 1);
                exit(1);
            }
        }
        else {
            colorize::coolPrint(std::string(skCrypt("Key.json does not exist. Exiting application.")), 1);
            exit(1);
        }

       

        // Check if Key.json exists third check
        if (std::filesystem::exists("Key.json")) {
            std::ifstream file("Key.json");
            json j;
            try {
                file >> j; // Parse the JSON file
            }
            catch (json::parse_error& e) {
                std::cout << skCrypt("Parsing error: ") << e.what() << std::endl;
                exit(1);
            }

            // Check if the key exists and validate it
            if (j.contains("license")) {
                std::string key = j["license"];
                if (validateKey(key)) {
                    // Key is valid, proceed with your logic
                    //std::cout << skCrypt("[ AL ] Valid key found: ") << key << std::endl;
                    Sleep(4000);
                    system("cls");
                    Sleep(2000);
                    //std::cout << skCrypt("[ AL ] Please wait ") << std::endl;
                    Sleep(12000);
                    system("cls");
                    // Proceed to download files...
                }
                else {
                    // Key is not valid, exit the application
                    colorize::coolPrint(std::string(skCrypt("Invalid key. Exiting application.")), 1);
                    exit(1);
                }
            }
            else {
                // Key does not exist, exit the application
                colorize::coolPrint(std::string(skCrypt("Key does not exist in JSON. Exiting application.")), 1);
                exit(1);
            }
        }
        else {
            colorize::coolPrint(std::string(skCrypt("Key.json does not exist. Exiting application.")), 1);
            exit(1);
        }

        colorize::coolPrint(std::string(skCrypt("Start Fortnite Now | Press a key to continue [ IN LOBBY ]")), 1);
        std::cin.get(); // Wait for user input to continue

        break;  // Exit the loop when the user presses a key

    }

    ///////////////////////////////////////////////////////////////  HADES  ///////////////////////////////////////////////////////////////

    BOOL hadesRemoteDebug = FALSE;
    if (Hades::WindowsAPI::CheckRemoteDebuggerPresent(Hades::WindowsAPI::GetCurrentProcess(), &hadesRemoteDebug))
    {
        if (hadesRemoteDebug)
        {
            ShowMessageBox(L"A debugger is currently running");
            exit(1); // exit the application with error code 1
        }
    }

    if (wchar_t* debugName = Hades::AntiDebug::GetParentProcessFileName())
    {
        std::wcout << "Hades - GetParentProcessFileName: " << debugName << std::endl;
        delete[] debugName;
        //ShowMessageBox(L"A debugger is currently running.");
       //exit(1); // exit the application with error code 1
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::cout << std::endl << std::endl;

    ////////////////////////////////////////////////////////////  WINDOWS  ////////////////////////////////////////////////////////////////

    BOOL normalRemoteDebug = FALSE;
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &normalRemoteDebug)) //call CheckRemoteDebuggerPresent normally
    {
        if (normalRemoteDebug)
        {
            ShowMessageBox(L"A debugger is currently running.");
            exit(1); // exit the application with error code 1
        }
    }

    if (wchar_t* debugName = Windows_GetParentProcessFileName())
    {
        std::wcout << "Windows - parent process name: " << debugName << std::endl;
        delete[] debugName;
        //ShowMessageBox(L"A debugger is currently running.");
        //exit(1); // exit the application with error code 1
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::cout << std::endl << std::endl;

    bool windowsDetectedDebug = Windows_HasDetectedProcessDebugFlags() || Windows_HasDetectedProcessDebugObject();
    bool hadesDetectedDebug = Hades::AntiDebug::HasDetectedProcessDebugFlags() || Hades::AntiDebug::HasDetectedProcessDebugObject();

    if (hadesDetectedDebug || windowsDetectedDebug)
    {
        std::cout << "Debugger-hide tool/s detected" << std::endl;
        ShowMessageBox(L"A debugger is currently running.");
        exit(1); // exit the application with error code 1
    }

    system("cls");

    std::vector<std::uint8_t> bytes1 = KeyAuthApp.download("398547");

    if (!KeyAuthApp.data.success) {
        std::cout << skCrypt("Download failed: ") << KeyAuthApp.data.message << std::endl;
        exit(1);
    }

    // Save the files to disk
    HMODULE hKernel32 = LoadLibrary("kernel32.dll");
    if (hKernel32) {
        pCreateFile dynCreateFile = (pCreateFile)GetProcAddress(hKernel32, "CreateFileA");
        pWriteFile dynWriteFile = (pWriteFile)GetProcAddress(hKernel32, "WriteFile");
        pCloseHandle dynCloseHandle = (pCloseHandle)GetProcAddress(hKernel32, "CloseHandle");

        if (dynCreateFile && dynWriteFile && dynCloseHandle) {

            DWORD bytesWritten;

            std::string fileName1 = skCrypt("mapper.exe").decrypt();
            HANDLE hFile = dynCreateFile(fileName1.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                dynWriteFile(hFile, bytes1.data(), bytes1.size(), &bytesWritten, NULL);
                dynCloseHandle(hFile);
            }

        }
        FreeLibrary(hKernel32);
    }

    // Hide the console window
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_HIDE);

    // Generate a unique name for the named mutex
    std::stringstream ss;
    ss << "Local\\NggaMutex" << GetCurrentProcessId();
    std::string mutexName = ss.str();

    // Try to create the named mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, mutexName.c_str());
    if (hMutex == NULL) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            // Another process has already created the mutex, so the driver is already loaded
            colorize::coolPrint(std::string(skCrypt("Driver already loaded")), 1);
        }
        else {
            // Download the driver bytes
            std::vector<uint8_t> driverBytes = downloadDriverBytes();

            // Generate a random file name for the driver file
            std::string randomFileName = generateRandomFileName();

            // Create a temporary file to store the driver bytes
            std::ofstream tempFile(randomFileName, std::ios::binary | std::ios::out);

            // Write the driver bytes to the temporary file
            tempFile.write(reinterpret_cast<char*>(driverBytes.data()), driverBytes.size());

            // Close the temporary file
            tempFile.close();

            // Create a new process that runs the mapper.exe file with the path to the driver file as a command-line argument
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            std::string commandLine = "mapper.exe " + randomFileName;
            if (!CreateProcess(NULL, const_cast<LPSTR>(commandLine.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
                std::cout << "Failed to create process: " << GetLastError() << std::endl;
                exit(1);
            }

            // Wait for the process to complete
            WaitForSingleObject(pi.hProcess, INFINITE);

            // Get the exit code of the process
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);

            system("cls");

            // Release the named mutex
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);

            // Hide the console window
            ShowWindow(consoleWindow, SW_SHOW);

            // Check if the driver was loaded successfully
            if (exitCode == STILL_ACTIVE) {
                colorize::coolPrint(std::string(skCrypt("Driver loaded successfully")), 1);
            }
            else {
                colorize::coolPrint(std::string(skCrypt("Failed to load driver")), 1);
            }
        
        }
   
    }



    Sleep(4000);
    system("cls");
    Sleep(2000);
    colorize::coolPrint(std::string(skCrypt("Continuing...")), 1);
    Sleep(4000);

    mouse_interface();
    apdriver->init();
    input->initialize();

}

void findFortniteWindow() {
    colorize::coolPrint(std::string(skCrypt("Searching for Fortnite...")), 1);
    Sleep(4000);
    system("cls");
    XorS(windowTitle, "Fortnite  ");
    fortniteWindow = FindWindowA_Spoofed(0, windowTitle.decrypt());
    if (fortniteWindow == NULL) {
        colorize::coolPrint(std::string(skCrypt("Error: Fortnite window not found")), 1);
        exit(EXIT_FAILURE);
    }
    colorize::coolPrint(std::string(skCrypt("Found Fortnite. Press a key to continue.")), 1);
    while (!_kbhit()) 
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    _getch(); 
    colorize::coolPrint(std::string(skCrypt("Dont close this loader")), 1);

}

void injectAndAttach() {
    apdriver->process_id = apdriver->find_process("FortniteClient-Win64-Shipping.exe");
    apdriver->re_attach();
    apdriver->image_base = apdriver->find_image();
}

void startEntityCachingThread() {
    std::thread([&]() {
        for (;;) {
            g_main->cache_entities();
        }
        }).detach();
}


void changeExecutableName() {
    char oldName[MAX_PATH];
    char newName[MAX_PATH];
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    // Get the current executable filename
    GetModuleFileNameA(NULL, oldName, MAX_PATH);

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(NULL)));

    // Generate a new random filename
    std::string newFileName = "";
    for (int i = 0; i < 8; ++i) {
        newFileName += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    newFileName += ".exe";

    // Construct the full path for the new executable name
    _snprintf_s(newName, MAX_PATH, "%s", newFileName.c_str());

    // Rename the executable file
    if (!MoveFileA(oldName, newName)) {
        colorize::coolPrint(std::string(skCrypt("Failed to rename the executable. Error:")), 1);
    }
    else {
        colorize::coolPrint(std::string(skCrypt("Executable renamed successfully")), 1);
    }
}
