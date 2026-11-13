#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

static const char* GAME_PACKAGE = "com.rust.oxidesurvival";

static std::string GetProgramFiles() {
    const char* pf = getenv("ProgramFiles");
    return pf ? std::string(pf) : "C:\\Program Files";
}

static std::string FindAdb() {
    std::string pf = GetProgramFiles();
    std::vector<std::string> paths = {
        pf + "\\BlueStacks_nxt\\HD-Adb.exe",
        pf + "\\BlueStacks\\HD-Adb.exe",
        pf + "\\LDPlayer\\LDPlayer9\\adb.exe",
    };
    for (auto& p : paths) {
        if (GetFileAttributesA(p.c_str()) != INVALID_FILE_ATTRIBUTES)
            return p;
    }
    char buf[MAX_PATH] = {};
    if (SearchPathA(nullptr, "adb.exe", nullptr, MAX_PATH, buf, nullptr))
        return buf;
    return "";
}

int main() {
    SetConsoleOutputCP(65001);
    printf("=== Oxide Cheat Cleanup ===\n\n");

    std::string adb = FindAdb();
    if (adb.empty()) {
        printf("[!] ADB not found\n");
        system("pause");
        return 1;
    }

    system(("\"" + adb + "\" connect 127.0.0.1:5555 >nul 2>nul").c_str());
    system(("\"" + adb + "\" connect 127.0.0.1:5565 >nul 2>nul").c_str());
    Sleep(1000);

    printf("[*] Removing LD_PRELOAD...\n");
    system(("\"" + adb + "\" shell setprop wrap." + std::string(GAME_PACKAGE) + " ''").c_str());

    printf("[*] Removing library...\n");
    system(("\"" + adb + "\" shell rm -f /data/local/tmp/liboxide_cheat.so").c_str());

    printf("[*] Restarting game...\n");
    system(("\"" + adb + "\" shell am force-stop " + std::string(GAME_PACKAGE)).c_str());

    printf("\n[*] Done. Game will start clean.\n");
    system("pause");
    return 0;
}
