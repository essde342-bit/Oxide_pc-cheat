#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static const char* GAME_PACKAGE = "com.rust.oxidesurvival";
static const char* CHEAT_LIB    = "liboxide_cheat.so";
static const char* CHEAT_DELAY  = "5";

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
        pf + "\\Microvirt\\MEmu\\adb.exe",
        pf + "\\Nox\\bin\\nox_adb.exe",
    };

    for (auto& p : paths) {
        if (GetFileAttributesA(p.c_str()) != INVALID_FILE_ATTRIBUTES)
            return p;
    }

    char buf[MAX_PATH] = {};
    if (SearchPathA(nullptr, "adb.exe", nullptr, MAX_PATH, buf, nullptr))
        return std::string(buf);

    return "";
}

static int Run(const std::string& adb, const std::string& args, bool silent) {
    std::string cmd = "\"" + adb + "\" " + args;
    if (silent) cmd += " >nul 2>nul";
    return system(cmd.c_str());
}

static bool FileExists(const std::string& path) {
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

int main() {
    SetConsoleOutputCP(65001);
    printf("============================================\n");
    printf("  Oxide Survival Island - PC Injector\n");
    printf("============================================\n\n");

    // 1. ADB
    printf("[1/6] Searching for ADB...\n");
    std::string adb = FindAdb();
    if (adb.empty()) {
        printf("[!] ADB not found!\n");
        printf("    Install Android SDK Platform Tools\n");
        printf("    or use emulator's built-in ADB.\n");
        system("pause");
        return 1;
    }
    printf("  Found: %s\n\n", adb.c_str());

    // 2. Подключение
    printf("[2/6] Connecting to emulator...\n");
    Run(adb, "connect 127.0.0.1:5555", true);
    Run(adb, "connect 127.0.0.1:5565", true);
    Run(adb, "connect 127.0.0.1:5575", true);
    Run(adb, "connect 127.0.0.1:62001", true);

    if (Run(adb, "devices", false) != 0) {
        printf("[!] Failed to list devices\n");
        system("pause");
        return 1;
    }
    printf("\n");

    // 3. Root
    printf("[3/6] Requesting root...\n");
    Run(adb, "root", true);
    Sleep(2000);
    Run(adb, "wait-for-device", true);
    printf("  Done\n\n");

    // 4. Проверка игры
    printf("[4/6] Checking game installation...\n");
    std::string checkCmd = "shell pm list packages | findstr " + std::string(GAME_PACKAGE);
    if (Run(adb, checkCmd, true) != 0) {
        printf("[!] Game package '%s' not found!\n", GAME_PACKAGE);
        printf("    Make sure the game is installed in emulator.\n");
        printf("    Or change GAME_PACKAGE in injector.\n");
        system("pause");
        return 1;
    }
    printf("  Game found\n\n");

    // 5. Пушим библиотеку
    printf("[5/6] Pushing cheat library...\n");
    if (!FileExists(CHEAT_LIB)) {
        printf("[!] %s not found!\n", CHEAT_LIB);
        printf("    Put it next to injector.exe\n");
        system("pause");
        return 1;
    }

    std::string pushCmd = "push \"" + std::string(CHEAT_LIB) + "\" /data/local/tmp/" + CHEAT_LIB;
    if (Run(adb, pushCmd, false) != 0) {
        printf("[!] Push failed!\n");
        system("pause");
        return 1;
    }
    Run(adb, "shell chmod 755 /data/local/tmp/" + std::string(CHEAT_LIB), true);
    printf("  Done\n\n");

    // 6. Инжект
    printf("[6/6] Injecting...\n");

    std::string setprop = "shell setprop wrap." + std::string(GAME_PACKAGE) +
        " 'LD_PRELOAD=/data/local/tmp/" + std::string(CHEAT_LIB) +
        " CHEAT_DELAY=" + std::string(CHEAT_DELAY) + "'";
    Run(adb, setprop, false);

    Run(adb, "shell am force-stop " + std::string(GAME_PACKAGE), true);
    Sleep(1000);

    printf("  Launching game...\n");
    Run(adb, "shell monkey -p " + std::string(GAME_PACKAGE) +
        " -c android.intent.category.LAUNCHER 1", true);

    printf("\n============================================\n");
    printf("  Done! Wait %s seconds for menu.\n", CHEAT_DELAY);
    printf("  Run cleanup.exe to remove cheat.\n");
    printf("============================================\n");
    system("pause");
    return 0;
}
