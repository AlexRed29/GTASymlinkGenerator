// GTA Symlink Generator.cpp
// Created by: AlexRed29X
//
// Ultima Modificacion: 15 Septiembre 2026
//


#include <iostream>
#include <limits>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <unordered_map>

#define NOMINMAX
#include <Windows.h>
#include "resource.h"
#include <sstream>


// ========================================
// TRANSLATION
// ========================================

std::unordered_map<std::string, std::string> language;

bool loadLanguage(const std::string& filename)
{
    HRSRC resource = nullptr;

    if (filename == "languages/English.ini")
        resource = FindResource(nullptr, MAKEINTRESOURCE(ENGLISH_INI), RT_RCDATA);
    else if (filename == "languages/Spanish.ini")
        resource = FindResource(nullptr, MAKEINTRESOURCE(SPANISH_INI), RT_RCDATA);
    else if (filename == "languages/portuguese.ini")
        resource = FindResource(nullptr, MAKEINTRESOURCE(PORTUGUESE_INI), RT_RCDATA);
    else if (filename == "languages/french.ini")
        resource = FindResource(nullptr, MAKEINTRESOURCE(FRENCH_INI), RT_RCDATA);
    else if (filename == "languages/german.ini")
        resource = FindResource(nullptr, MAKEINTRESOURCE(GERMAN_INI), RT_RCDATA);

    if (!resource)
        return false;

    HGLOBAL data = LoadResource(nullptr, resource);

    if (!data)
        return false;

    DWORD size = SizeofResource(nullptr, resource);

    const char* content = static_cast<const char*>(LockResource(data));

    if (!content)
        return false;

    std::string text(content, size);

    std::istringstream file(text);

    std::string line;

    while (std::getline(file, line))
    {
        // Ignore empty lines
        if (line.empty())
            continue;

        // Ignore comments
        if (line[0] == ';')
            continue;

        size_t separator = line.find('=');

        if (separator == std::string::npos)
            continue;

        std::string key = line.substr(0, separator);
        std::string value = line.substr(separator + 1);

        language[key] = value;
    }

    return true;
}


std::string getText(const std::string& key)
{
    auto it = language.find(key);

    if (it != language.end())
    {
        return it->second;
    }

    return "[" + key + "]";
}




// ========================================
// CLEAR SCREEN
// ========================================

void clearScreen()
{
    system("cls");
}


// ========================================
// CREATE SYMLINK
// ========================================

bool createSymlink(
    const std::filesystem::path& source,
    const std::filesystem::path& destination)
{
    bool isDirectory =
        std::filesystem::is_directory(source);

    bool isFile =
        std::filesystem::is_regular_file(source);

    if (!isDirectory && !isFile)
    {
        std::cout << getText("ERROR_SOURCE") << "\n";
        return false;
    }

    DWORD flags =
        isDirectory
        ? (SYMBOLIC_LINK_FLAG_DIRECTORY |
            SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)
        : SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

    if (CreateSymbolicLinkW(
        destination.c_str(),
        source.c_str(),
        flags))
    {
        return true;
    }

    DWORD error = GetLastError();

    std::cout << getText("ERROR_CREATE_SYMLINK") << "\n";
    std::cout << "Windows error code: " << error << "\n\n\n";

    std::cout << getText("FIX_1314") << "\n";

    return false;
}


// ========================================
// COPY ELEMENT
// ========================================

bool copyElement(
    const std::filesystem::path& source,
    const std::filesystem::path& destination)
{
    try
    {
        if (!std::filesystem::exists(source))
        {
            std::cout << getText("ERROR_SOURCE_ELEMENT") << "\n";
            std::cout << source.string() << "\n";

            return false;
        }

        if (std::filesystem::exists(destination))
        {
            std::cout << getText("ERROR_DESTINATION_EXIST") << "\n";
            std::cout << destination.string() << "\n";

            return false;
        }

        if (std::filesystem::is_directory(source))
        {
            std::filesystem::copy(
                source,
                destination,
                std::filesystem::copy_options::recursive
            );
        }
        else if (std::filesystem::is_regular_file(source))
        {
            std::filesystem::copy_file(
                source,
                destination
            );
        }
        else
        {
            std::cout << "\n";
            std::cout << getText("ERROR_SOURCE_NOT") << "\n";
            std::cout << source.string() << "\n";

            return false;
        }

        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cout << getText("ERROR_COPYING") << "\n";
        std::cout << error.what() << "\n";

        return false;
    }
}


// ========================================
// TOTAL CONVERSION
// ========================================

void createGameSymlink(
    const std::string& gameName,
    const std::vector<std::string>& symlinks,
    const std::vector<std::string>& copies)
{
    std::string sourceText;
    std::string destinationText;

    char totalConversion;

    clearScreen();

    std::cout << "========================================\n";
    std::cout << "          " << gameName << "\n";
    std::cout << "========================================\n\n";

    std::cout << "\n" << getText("TOTAL_CONVERSION_QUESTION") << "\n\n";
    std::cout << getText("EXAMPLES") << "\n";
    std::cout << "III: GTA Frosted Winter, Forelli Redemption\n";
    std::cout << "Vice City: GTA Long Night, Vice City Extended Features\n";
    std::cout << "San Andreas: GTA Carcer City, GTA Underground, Zombie Andreas, Project Eagle\n\n\n";


    std::cout << "[Y] " << getText("YES") << "\n";
    std::cout << "[N] " << getText("NO") << "\n\n";

    std::cout << getText("SELECT_OPTION") << "\n";
    std::cin >> totalConversion;


    if (totalConversion == 'Y' ||
        totalConversion == 'y')
    {
        clearScreen();

        std::cout << "\n========================================\n\n";
        std::cout << getText("WARNING") << "\n";
        std::cout << getText("FULLGAME") << "\n\n";
        std::cout << "========================================\n\n";

        std::cout << getText("NO_SYMLINK") << "\n\n";
        std::cout << getText("IF_SYMLINK") << "\n";

        std::cout << "- Anim\n";
        std::cout << "- Audio\n";
        std::cout << "- Data\n";
        std::cout << "- Models\n";
        std::cout << "- Text\n\n\n";

        std::cout << getText("BAD_SYMLINK") << "\n\n";

        std::cout << getText("RETURN_ENTER") << "\n";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();

        return;
    }


    if (totalConversion != 'N' &&
        totalConversion != 'n')
    {

        std::cout << getText("INVALID_OPTION") << "\n";
        std::cout << "Please select N or Y.\n\n";

        std::cout << getText("PRESS_ENTER") << "\n";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();

        return;
    }


    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );


    // ========================================
    //
    // CREATE SYMLINK GAME
    //
    // ========================================

    clearScreen();

    std::cout << "\n========================================\n";
    std::cout << getText("SYMLINK_PATHS") << "\n";
    std::cout << "          " << gameName << "\n";
    std::cout << "========================================\n\n";

    std::cout << getText("SOURCE_PATH") << "\n";
    std::getline(std::cin, sourceText);

    std::cout << "\n" << getText("DESTINATION_PATH") << "\n";
    std::getline(std::cin, destinationText);


    std::filesystem::path source(sourceText);
    std::filesystem::path destination(destinationText);


    // ========================================
    // CHECK PATHS
    // ========================================

    std::cout << "\n\n========================================\n";
    std::cout << "\n" << getText("CHECKING_PATHS") << "\n";
    std::cout << "\n========================================\n\n";


    if (!std::filesystem::exists(source))
    {
        std::cout << "Source: ERROR - Path does not exist.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(source))
    {
        std::cout << "Source: ERROR - Path is not a folder.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::cout << "Source: OK - Folder exists.\n";


    if (!std::filesystem::exists(destination))
    {
        std::cout << "Destination: ERROR - Path does not exist.\n\n";

        std::cout << "You must create the destination folder first.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(destination))
    {
        std::cout << "Destination: ERROR - Path is not a folder.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::cout << "Destination: OK - Folder exists.\n";


    // ========================================
    // CHECK REQUIRED ELEMENTS
    // ========================================

    bool allElementsExist = true;


    std::cout << "\n\n========================================\n";
    std::cout << "\n" << getText("REQUIRED_ELEMENTS") << "\n";
    std::cout << "\n========================================\n\n";


    std::cout << "SYMLINKS:\n\n";

    for (const auto& element : symlinks)
    {
        std::filesystem::path elementPath =
            source / element;

        if (std::filesystem::exists(elementPath))
        {
            std::cout << "[OK]       "
                << element << "\n";
        }
        else
        {
            std::cout << "[MISSING]  "
                << element << "\n";

            allElementsExist = false;
        }
    }


    std::cout << "\nCOPY:\n\n";

    for (const auto& element : copies)
    {
        std::filesystem::path elementPath =
            source / element;

        if (std::filesystem::exists(elementPath))
        {
            std::cout << "[OK]       "
                << element << "\n";
        }
        else
        {
            std::cout << "[MISSING]  "
                << element << "\n";

            allElementsExist = false;
        }
    }


    if (!allElementsExist)
    {
        std::cout << "\n\n========================================\n";
        std::cout << getText("WARNING") << "\n";
        std::cout << "\n========================================\n\n";

        std::cout << getText("REQUIRED_FILES") << "\n";
                std::cout << getText("CREATE_FOLDERS") << "\n\n";

        std::cout << "The "
            << gameName
            << " Symlink cannot be created.\n\n";

        std::cout << "Operation cancelled.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::cout << "\n";
    std::cout << getText("REQUIRED_FOUND") << "\n";


    // ========================================
    // CHECK DESTINATIONS
    // ========================================

    bool allDestinationsAvailable = true;


    std::cout << "\n\n========================================\n";
    std::cout << "\n" << getText("CHECKING_DESTINATION") << "\n";
    std::cout << "\n========================================\n\n";


    std::cout << "SYMLINKS:\n\n";

    for (const auto& element : symlinks)
    {
        std::filesystem::path destinationPath =
            destination / element;

        if (std::filesystem::exists(destinationPath))
        {
            std::cout << "[EXISTS]      "
                << element << "\n";

            allDestinationsAvailable = false;
        }
        else
        {
            std::cout << "[AVAILABLE]   "
                << element << "\n";
        }
    }


    std::cout << "\nCOPY:\n\n";

    for (const auto& element : copies)
    {
        std::filesystem::path destinationPath =
            destination / element;

        if (std::filesystem::exists(destinationPath))
        {
            std::cout << "[EXISTS]      "
                << element << "\n";

            allDestinationsAvailable = false;
        }
        else
        {
            std::cout << "[AVAILABLE]   "
                << element << "\n";
        }
    }


    if (!allDestinationsAvailable)
    {
        std::cout << "\n========================================\n";
        std::cout << getText("WARNING") << "\n";
        std::cout << "\n========================================\n\n";

        std::cout << getText("DESTINATION_EXIST1") << "\n\n";
        

        std::cout << getText("DESTINATION_EXIST2") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }

    std::cout << "\n";
    std::cout << getText("ALL_AVAILABLE") << "\n";


    // ========================================
    // CONFIRMATION
    // ========================================

    char confirmation;


    std::cout << "\n\n========================================\n\n";
    std::cout << getText("FINAL_CONFIRMATION") << "\n";
    std::cout << "\n========================================\n\n";

    std::cout << getText("SYM_FILE")
        << symlinks.size()
        << "\n";

    std::cout << getText("COPY_FILE")
        << copies.size()
        << "\n\n";

    std::cout << "Source:\n";
    std::cout << source.string() << "\n\n";

    std::cout << "Destination:\n";
    std::cout << destination.string() << "\n\n";

    std::cout << "[Y] " << getText("YES") << "\n";
    std::cout << "[N] " << getText("NO") << "\n\n";

    std::cout << getText("SELECT_OPTION");
    std::cin >> confirmation;


    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout << getText("CANCELLED") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();

        return;
    }


    // ========================================
    // CREATE SYMLINKS
    // ========================================

    std::cout << "\nConfirmed.\n";
    std::cout << "Creating Symlinks...\n\n";


    int symlinksCreated = 0;
    int symlinksFailed = 0;


    for (const auto& element : symlinks)
    {
        std::filesystem::path sourcePath =
            source / element;

        std::filesystem::path destinationPath =
            destination / element;

        std::cout << "[SYMLINK] "
            << element
            << " ... ";


        if (createSymlink(
            sourcePath,
            destinationPath))
        {
            std::cout << "OK\n";
            symlinksCreated++;
        }
        else
        {
            std::cout << "ERROR\n";

            symlinksFailed++;

            break;
        }
    }


    if (symlinksFailed > 0)
    {
        std::cout << getText("\n");
        std::cout << getText("COPY_NOT") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();

        return;
    }


    // ========================================
    // COPY FILES
    // ========================================

    int elementsCopied = 0;


    for (const auto& element : copies)
    {
        std::filesystem::path sourcePath =
            source / element;

        std::filesystem::path destinationPath =
            destination / element;

        std::cout << "[COPY] "
            << element
            << " ... ";


        if (copyElement(
            sourcePath,
            destinationPath))
        {
            std::cout << "OK\n";
            elementsCopied++;
        }
        else
        {
            std::cout << "ERROR\n";

            break;
        }
    }


    // ========================================
    // RESULT
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "\n" << getText("RESULT") << "\n";
    std::cout << "\n========================================\n\n";

    std::cout << "\n" << getText("SYM_OK")
        << symlinksCreated
        << " / "
        << symlinks.size()
        << "\n";

    std::cout << "\n" << getText("COPY_OK")
        << elementsCopied
        << " / "
        << copies.size()
        << "\n";

    std::cout << "\n"
        << gameName
        << " processed successfully.\n\n\n";

    std::cout << "\n========================================\n\n";
    std::cout << getText("MISSION_PASSED") << "\n";
    std::cout << getText("RESPECT_PLUS") << "\n\n";
    std::cout << "========================================\n\n\n";


    std::cout << getText("PRESS_ENTER") << "\n";

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cin.get();
}


// ========================================
// CREATE MOD SYMLINK
// ========================================

void createModSymlink()
{
    std::string sourceText;
    std::string modloaderText;


    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );


    clearScreen();


    std::cout << "========================================\n";
    std::cout << "             MOD SYMLINK\n";
    std::cout << "========================================\n\n";

    std::cout << getText("MOD_SYMLINK_INFO_1") << "\n";
    std::cout << getText("MOD_SYMLINK_INFO_2") << "\n\n";

    std::cout << getText("MOD_SYMLINK_INFO_3") << "\n";
    std::cout << getText("MOD_SYMLINK_INFO_4") << "\n\n";


    // ========================================
    // SOURCE
    // ========================================

    std::cout << getText("MOD_SOURCE_PATH") << "\n> ";
    std::getline(std::cin, sourceText);


    // ========================================
    // MODLOADER
    // ========================================

    std::cout << "\n" << getText("MOD_DESTINATION_PATH") << "\n> ";
    std::getline(std::cin, modloaderText);


    std::filesystem::path source(sourceText);
    std::filesystem::path modloader(modloaderText);


    // ========================================
    // CHECK PATHS
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "\n" << getText("CHECKING_PATHS") << "\n\n";
    std::cout << "========================================\n\n";


    if (!std::filesystem::exists(source))
    {
        std::cout << "\n" << getText("ERROR_NOTEXIST") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(source))
    {
        std::cout << "\n" << getText("ERROR_NOTFOLDER") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::cout << "Source: OK - Folder exists.\n";


    if (!std::filesystem::exists(modloader))
    {
        std::cout << "\n" << getText("ERROR_MODLOADER1") << "\n";
        std::cout << "\n" << getText("ERROR_MODLOADER2") << "\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(modloader))
    {
        std::cout << "\n" << getText("ERROR_MODLOADER3") << "\n\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::cout << "Modloader: OK - Folder exists.\n";


    // ========================================
    // MOD NAME
    // ========================================

    std::string modName =
        source.filename().string();


    if (modName.empty())
    {
        std::cout << "\nERROR - Could not determine the mod name.\n";

        std::cout << getText("PRESS_ENTER") << "\n";
        std::cin.get();

        return;
    }


    std::filesystem::path finalDestination =
        modloader / modName;


    // ========================================
    // MOD INFORMATION
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << getText("MOD_INFORMATION") << "\n\n";
    std::cout << "========================================\n\n";

    std::cout << getText("MOD_NAME") << modName << "\n";
    std::cout << modName << "\n\n";

    std::cout << getText("MOD_SOURCE") << sourceText << "\n";
    std::cout << source.string() << "\n\n";

    std::cout << "Final destination:\n";
    std::cout << finalDestination.string() << "\n";


    // ========================================
    // CHECK FINAL DESTINATION
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << getText("CHECKING_DESTINATION") << "\n";
    std::cout << "========================================\n\n";


    if (std::filesystem::exists(finalDestination))
    {
        std::cout << "[NOT AVAILABLE]\n\n";

        std::cout << "Something already exists at:\n";
        std::cout << finalDestination.string() << "\n\n";

        std::cout << "Operation cancelled for safety.\n";

        std::cout << getText("PRESS_ENTER") << "\n";

        std::cin.get();

        return;
    }


    std::cout << "[AVAILABLE]\n";

    std::cout << "\nThe destination is available.\n";


    // ========================================
    // CONFIRMATION
    // ========================================

    char confirmation;


    std::cout << "\n========================================\n\n";
    std::cout << getText("FINAL_CONFIRMATION_TITLE") << "\n\n";
    std::cout << "========================================\n\n";

    std::cout << getText("FINAL_CONFIRMATION") << "\n\n";

    std::cout << "MOD:\n";
    std::cout << modName << "\n\n";

    std::cout << getText("FROM") << "\n";
    std::cout << source.string() << "\n\n";

    std::cout << getText("TO") << "\n";
    std::cout << finalDestination.string() << "\n\n";

    std::cout << "[Y] " << getText("YES") << "\n";
    std::cout << "[N] " << getText("NO") << "\n\n";

    std::cout << "Continue? ";
    std::cin >> confirmation;


    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout << "\nOperation cancelled.\n";

        std::cout << getText("PRESS_ENTER") << "\n";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();

        return;
    }


    // ========================================
    // CREATE MOD SYMLINK
    // ========================================

    std::cout << "\nConfirmed.\n";
    std::cout << "Creating Symlink...\n\n";


    if (createSymlink(
        source,
        finalDestination))
    {
        std::cout << "\n========================================\n";
        std::cout << "\n" << getText("RESULT") << "\n\n";
        std::cout << "========================================\n\n";

        std::cout << "[OK]" << getText("SYMLINK_OK") << "\n\n";

        std::cout << "Mod:\n";
        std::cout << modName << "\n\n";

        std::cout << "Source:\n";
        std::cout << source.string() << "\n\n";

        std::cout << "Destination:\n";
        std::cout << finalDestination.string() << "\n\n\n";

        std::cout << "\n========================================\n\n";
        std::cout << getText("MISSION_PASSED") << "\n";
        std::cout << getText("RESPECT_PLUS") << "\n\n";
        std::cout << "========================================\n\n\n";
    }
    else
    {
        std::cout << "\n========================================\n";
        std::cout << "              ERROR\n";
        std::cout << "========================================\n\n";

        std::cout << "Failed to create the mod Symlink.\n";
    }


    std::cout << getText("PRESS_ENTER") << "\n";

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cin.get();
}


// ========================================
// MAIN
// ========================================

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int languageOption;

    while (true)
    {
        std::cout << "========================================\n";
        std::cout << "              LANGUAGE\n";
        std::cout << "========================================\n\n";

        std::cout << "[1] English\n";
        std::cout << "[2] Español\n";
        std::cout << "[3] Portuguese\n";
        std::cout << "[4] Français\n";
        std::cout << "[5] Deutsch\n\n";

        std::cout << "Select an option:\n";
        std::cin >> languageOption;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << getText("INVALID_OPTION") << "\n";
            std::cout << getText("PRESS_ENTER") << "\n";

            std::cin.get();
            std::cin.get();

            clearScreen();
            continue;
        }

        if (languageOption == 1)
        {
            if (!loadLanguage("languages/English.ini"))
            {
                std::cout << "\nERROR: Could not load English.ini\n";
                return 1;
            }

            break;
        }
        else if (languageOption == 2)
        {
            if (!loadLanguage("languages/Spanish.ini"))
            {
                std::cout << "\nERROR: Could not load Spanish.ini\n";
                return 1;
            }

            break;
        }
        else if (languageOption == 3)
        {
            if (!loadLanguage("languages/portuguese.ini"))
            {
                std::cout << "\nERROR: Could not load portuguese.ini\n";
                return 1;
            }
            break;
        }
        else if (languageOption == 4)
        {
            if (!loadLanguage("languages/french.ini"))
            {
                std::cout << "\nERROR: Could not load french.ini\n";
                return 1;
            }
            break;
        }
        else if (languageOption == 5)
        {
            if (!loadLanguage("languages/german.ini"))
            {
                std::cout << "\nERROR: Could not load german.ini\n";
                return 1;
            }
            break;
        }
        else
        {
            std::cout << getText("INVALID_OPTION") << "\n";

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cin.get();

            clearScreen();
        }
    }

    int option;


    // ========================================
    // GTA III
    // ========================================

    const std::vector<std::string> gta3Symlinks =
    {
        "Anim",
        "Audio",
        "Models",
        "Movies",
        "MP3",
        "MSS",
        "TEXT",
        "TXD"
    };


    const std::vector<std::string> gta3Copies =
    {
        "Data",
        "Skins",
        "drvmgt.dll",
        "gta3.exe",
        "Mss32.dll"
    };


    // ========================================
    // GTA VICE CITY
    // ========================================

    const std::vector<std::string> viceCitySymlinks =
    {
        "Anim",
        "Audio",
        "Models",
        "Movies",
        "MP3",
        "MSS",
        "TEXT",
        "TXD"
    };


    const std::vector<std::string> viceCityCopies =
    {
        "Data",
        "Skins",
        "gta-vc.exe",
        "Mss32.dll"
    };


    // ========================================
    // GTA SAN ANDREAS
    // ========================================

    const std::vector<std::string> sanAndreasSymlinks =
    {
        "anim",
        "audio",
        "models",
        "movies",
        "text"
    };


    const std::vector<std::string> sanAndreasCopies =
    {
        "data",
        "eax.dll",
        "gta_sa.exe",
        "ogg.dll",
        "stream.ini",
        "vorbis.dll",
        "vorbisFile.dll"
    };


    // ========================================
    // MAIN MENU
    // ========================================

    while (true)
    {
        clearScreen();

        std::cout << "========================================\n";
        std::cout << "          GTA SYMLINK GENERATOR\n";
        std::cout << "========================================\n\n";

        std::cout << "[1] " << getText("GTA_III") << "\n";
        std::cout << "[2] " << getText("GTA_VICE_CITY") << "\n";
        std::cout << "[3] " << getText("GTA_SAN_ANDREAS") << "\n";
        std::cout << "[4] " << getText("CREATE_MOD_SYMLINK") << "\n";
        std::cout << "[5] " << getText("EXIT") << "\n\n";

        std::cout << getText("SELECT_OPTION");
        std::cin >> option;


        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << getText("INVALID_OPTION") << "\n";
            std::cout << getText("PRESS_ENTER") << "\n";

            std::cin.get();
            std::cin.get();

            continue;
        }


        switch (option)
        {

            // ========================================
            // GTA III
            // ========================================

        case 1:
        {
            int gameOption;

            while (true)
            {
                clearScreen();

                std::cout << "========================================\n";
                std::cout << "                GTA III\n";
                std::cout << "========================================\n\n";

                std::cout << "[1] " << getText("CREATE_SYMLINK") << "\n";
                std::cout << "[2] " << getText("RETURN_TO_MAIN_MENU") << "\n\n";

                std::cout << getText("SELECT_OPTION");
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.get();

                    continue;
                }


                if (gameOption == 1)
                {
                    createGameSymlink(
                        "GTA III",
                        gta3Symlinks,
                        gta3Copies
                    );
                }
                else if (gameOption == 2)
                {
                    break;
                }
                else
                {
                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cin.get();
                }
            }

            break;
        }


        // ========================================
        // GTA VICE CITY
        // ========================================

        case 2:
        {
            int gameOption;

            while (true)
            {
                clearScreen();

                std::cout << "========================================\n";
                std::cout << "             GTA VICE CITY\n";
                std::cout << "========================================\n\n";

                std::cout << "[1] " << getText("CREATE_SYMLINK") << "\n";
                std::cout << "[2] " << getText("RETURN_TO_MAIN_MENU") << "\n\n";

                std::cout << getText("SELECT_OPTION") << "\n";
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.get();

                    continue;
                }


                if (gameOption == 1)
                {
                    createGameSymlink(
                        "GTA Vice City",
                        viceCitySymlinks,
                        viceCityCopies
                    );
                }
                else if (gameOption == 2)
                {
                    break;
                }
                else
                {
                    std::cout << getText("INVALID_OPTION") << "\n";

                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cin.get();
                }
            }

            break;
        }


        // ========================================
        // GTA SAN ANDREAS
        // ========================================

        case 3:
        {
            int gameOption;

            while (true)
            {
                clearScreen();

                std::cout << "========================================\n";
                std::cout << "           GTA SAN ANDREAS\n";
                std::cout << "========================================\n\n";

                std::cout << "[1] " << getText("CREATE_SYMLINK") << "\n";
                std::cout << "[2] " << getText("RETURN_TO_MAIN_MENU") << "\n\n";

                std::cout << getText("SELECT_OPTION") << "\n";
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.get();

                    continue;
                }


                if (gameOption == 1)
                {
                    createGameSymlink(
                        "GTA San Andreas",
                        sanAndreasSymlinks,
                        sanAndreasCopies
                    );
                }
                else if (gameOption == 2)
                {
                    break;
                }
                else
                {
                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cin.get();
                }
            }

            break;
        }


        // ========================================
        // MOD SYMLINK
        // ========================================

        case 4:
        {
            int modOption;

            while (true)
            {
                clearScreen();

                std::cout << "========================================\n";
                std::cout << "             MOD SYMLINK\n";
                std::cout << "========================================\n\n";

                std::cout << "[1] " << getText("CREATE_SYMLINK") << "\n";
                std::cout << "[2] " << getText("RETURN_TO_MAIN_MENU") << "\n\n";

                std::cout << getText("SELECT_OPTION") << "\n";
                std::cin >> modOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.get();

                    continue;
                }


                if (modOption == 1)
                {
                    createModSymlink();
                }
                else if (modOption == 2)
                {
                    break;
                }
                else
                {
                    std::cout << getText("INVALID_OPTION") << "\n";
                    std::cout << getText("PRESS_ENTER") << "\n";

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cin.get();
                }
            }

            break;
        }


        // ========================================
        // EXIT
        // ========================================

        case 5:

            std::cout << "\nExiting...\n";

            return 0;


        default:

            std::cout << getText("INVALID_OPTION") << "\n";
            std::cout << getText("PRESS_ENTER") << "\n";

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cin.get();

            break;
        }
    }


    return 0;
}

// Developed in: Visual Studio 2026
// Copyright (c) Microsoft Corporation.
// "Grand Theft Auto III", "Grand Theft Auto: Vice City" and "Grand Theft Auto: San Andreas"
// are the property of ©Rockstar Games and ©Rockstar North
//
