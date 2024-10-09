#include "programController.h"

QString getRegDitValue(QString RegDitPath, QString key){
    QSettings settings(RegDitPath, QSettings::NativeFormat); // HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399
    return settings.value(key).toString();
}

void runProgramAsAdmin(const QString &program, const QStringList &arguments) {
    // 将程序和参数转为 LPCWSTR 格式
    QString params = arguments.join(" ");
    LPCWSTR programPath = reinterpret_cast<LPCWSTR>(program.utf16());
    LPCWSTR paramsPath = reinterpret_cast<LPCWSTR>(params.utf16());

    // 调用 ShellExecute 来以管理员身份运行程序
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";  // 以管理员身份运行
    sei.lpFile = programPath;  // 程序路径
    sei.lpParameters = paramsPath;  // 程序参数
    sei.nShow = SW_NORMAL;  // 窗口正常显示

    // 启动程序
    if (!ShellExecuteExW(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            // 用户取消了 UAC 提示
        } else {
            // 处理其他错误
        }
    }
}

std::string ws2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

DWORD findProcessByName(const std::string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return DWORD();

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    // 遍历进程列表以查找进程名
    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            std::string exeFileName = ws2s(processEntry.szExeFile);  // 转换宽字符为 std::string
            if (processName == exeFileName) {
                DWORD processID = processEntry.th32ProcessID;
                CloseHandle(snapshot);
                return processID;
                break;
            }
        } while (Process32NextW(snapshot, &processEntry));
    }

    return DWORD();
}

void TerminateProcessByID(DWORD processID) {
    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (processHandle) {
        if (TerminateProcess(processHandle, 0)) {
            std::cout << "Process terminated successfully." << std::endl;
        } else {
            std::cerr << "Failed to terminate process." << std::endl;
        }
        CloseHandle(processHandle);
    } else {
        std::cerr << "Unable to open process." << std::endl;
    }
}

void TerminateProcessByName(const std::string& processName){
    DWORD processID = findProcessByName(processName);
    if(processID != DWORD()){
        TerminateProcessByID(processID);
    }
    else{
        std::cout << "can't find process." << std::endl;
    }
}
