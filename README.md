# Oxide Survival Island — PC Cheat (Emulator)

## Установка (для клиента)

1. Скачай `oxide-pc-cheat-full.zip`
2. Распакуй в папку (без русских букв в пути!)
3. Запусти эмулятор
4. Запусти `inject.bat` (или `injector.exe`)
5. Подожди 5-10 секунд
6. Готово

## Удаление

Запусти `cleanup.bat` (или `cleanup.exe`)

## Поддерживаемые эмуляторы

- BlueStacks 4/5
- LDPlayer 9
- MEmu
- Nox
- Любой с ADB

## Файлы

| Файл | Назначение |
|------|-----------|
| `injector.exe` | Инжектор (запусти меня) |
| `cleanup.exe` | Удаление чита |
| `inject.bat` | Инжектор (альтернатива) |
| `cleanup.bat` | Удаление (альтернатива) |
| `find_adb.bat` | Поиск ADB (диагностика) |
| `liboxide_cheat.so` | Библиотека чита |

## Логи

```batch
adb logcat | findstr "OxideCheat OxideHooks OxideESP OxideMenu"
```

## Troubleshooting

**"ADB not found"** → запусти `find_adb.bat`, установи эмулятор или Android SDK

**"Game not installed"** → установи игру в эмулятор

**Меню не появляется** → увеличь `CHEAT_DELAY` до 10 в `inject.bat`

**Игра крашится** → проверь что эмулятор использует OpenGL ES 3
