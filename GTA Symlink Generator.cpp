// GTA Symlink Generator.cpp 
// Created by: AlexRed29X
//
// Last modification: September 1st 2026
//

// GTA Symlink Generator.cpp

#include <iostream>
#include <limits>
#include <string>
#include <filesystem>
#include <vector>

#define NOMINMAX
#include <Windows.h>


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
        std::cout << "\nERROR: The source is not a valid file or folder.\n";
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

    std::cout << "\nERROR: Failed to create the Symlink.\n";
    std::cout << "Windows error code: "
        << error << "\n";

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
            std::cout << "\nERROR: Source element does not exist:\n";
            std::cout << source.string() << "\n";

            return false;
        }

        if (std::filesystem::exists(destination))
        {
            std::cout << "\nERROR: Destination already exists:\n";
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
            std::cout << "\nERROR: Source is not a file or folder.\n";
            std::cout << source.string() << "\n";

            return false;
        }

        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cout << "\nERROR while copying the element.\n";
        std::cout << error.what() << "\n";

        return false;
    }
}


// ========================================
// CREATE GAME SYMLINK
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

    std::cout << "ENG: Will this installation be for a 'TOTAL CONVERSION MOD'?\n";
    std::cout << "ESP: Esta instalacion sera para un 'MOD DE CONVERSION TOTAL'?\n\n\n";
    std::cout << "EXAMPLES:\n";
    std::cout << "III: GTA Frosted Winter, Forelli Redemption\n";
    std::cout << "Vice City: GTA Long Night, Vice City Extended Features\n";
    std::cout << "San Andreas: GTA Carcer City, GTA Underground, Zombie Andreas, Project Eagle\n\n";

    std::cout << "[N] No\n";
    std::cout << "[Y] Yes\n\n";

    std::cout << "Select an option: ";
    std::cin >> totalConversion;


    if (totalConversion == 'Y' ||
        totalConversion == 'y')
    {
        clearScreen();
        std::cout << "\n========================================\n";
        std::cout << "              WARNING! / ADVERTENCIA! \n";
        std::cout << "========================================\n\n";

        std::cout << "ENG: Make a full copy of the game! \n";
        std::cout << "ESP: Haz una copia completa del juego! \n\n";

        std::cout << "DON'T USE SYMLINKS with the\n";
        std::cout << "Total Conversion mods!\n\n";

        std::cout << "NO HAGAS SYMLINKS Con los\n";
        std::cout << "Mods de Conversion Total!\n\n\n";

        std::cout << "FILES THAT MODIFY THE 'TOTAL CONVERSION MODS':\n";
        std::cout << "ARCHIVOS QUE MODIFICAN LOS 'MODS DE CONVERSION TOTAL':\n\n";
        std::cout << "- Anim\n";
        std::cout << "- Audio\n";
        std::cout << "- Data\n";
        std::cout << "- Models\n";
        std::cout << "- Text\n\n\n";

        std::cout << "Press ENTER to return...";

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
        std::cout << "\nInvalid option.\n";
        std::cout << "Please select N or Y.\n\n";

        std::cout << "Press ENTER to continue...";

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
    // PATHS
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "             SYMLINK PATHS\n";
    std::cout << "========================================\n\n";

    std::cout << "SOURCE path (GTA WITHOUT MODS):\n";
    std::cout << "Ruta ORIGEN (GTA SIN MODS):\n\n> ";
    std::getline(std::cin, sourceText);

    std::cout << "\n\nDESTINATION PATH: (Where we are going to install our 'GTA Symlink')\n";
    std::cout << "RUTA DESTINO: (En donde vamos a instalar nuestro 'GTA Symlink')\n\n> ";
    std::getline(std::cin, destinationText);


    std::filesystem::path source(sourceText);
    std::filesystem::path destination(destinationText);


    // ========================================
    // CHECK PATHS
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "             CHECKING PATHS\n";
    std::cout << "========================================\n\n";


    if (!std::filesystem::exists(source))
    {
        std::cout << "Source: ERROR - Path does not exist.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(source))
    {
        std::cout << "Source: ERROR - Path is not a folder.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::cout << "Source: OK - Folder exists.\n";


    if (!std::filesystem::exists(destination))
    {
        std::cout << "Destination: ERROR - Path does not exist.\n\n";

        std::cout << "You must create the 'Destination Folder' first.\n";
        std::cout << "Debes crear 'la Carpeta de Destino' primero.\n\n\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(destination))
    {
        std::cout << "Destination: ERROR - Path is not a folder.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::cout << "Destination: OK - Folder exists.\n";


    // ========================================
    // CHECK REQUIRED ELEMENTS
    // ========================================

    bool allElementsExist = true;


    std::cout << "\n========================================\n";
    std::cout << "          REQUIRED ELEMENTS\n";
    std::cout << "========================================\n\n";


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
        std::cout << "\n========================================\n";
        std::cout << "        WARNING / ADVERTENCIA \n";
        std::cout << "\n========================================\n\n";

        std::cout << "Required files or folders are MISSING.\n";
        std::cout << "FALTAN archivos o carpetas requeridas.\n\n";

        std::cout << "The "
            << gameName
            << " Symlink cannot be created.\n\n";

        std::cout << "Operation cancelled.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::cout << "\nAll required elements were found!\n";
    std::cout << "Se encontraron todos los elementos requeridos!\n\n";



    // ========================================
    // CHECK DESTINATIONS
    // ========================================

    bool allDestinationsAvailable = true;


    std::cout << "\n========================================\n";
    std::cout << "          CHECKING DESTINATION\n";
    std::cout << "========================================\n\n";


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
        std::cout << "              WARNING\n";
        std::cout << "========================================\n\n";

        std::cout << "One or more destinations already exist.\n\n";

        std::cout << "Operation cancelled for safety.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::cout << "\nAll destinations are available.\n";


    // ========================================
    // CONFIRMATION
    // ========================================

    char confirmation;


    std::cout << "\n========================================\n";
    std::cout << "          FINAL CONFIRMATION\n";
    std::cout << "========================================\n\n";

    std::cout << "Symlinks to create: "
        << symlinks.size()
        << "\n";

    std::cout << "Elements to copy: "
        << copies.size()
        << "\n\n";

    std::cout << "Source:\n";
    std::cout << source.string() << "\n\n";

    std::cout << "Destination:\n";
    std::cout << destination.string() << "\n\n";

    std::cout << "[Y] Yes\n";
    std::cout << "[N] No\n\n";

    std::cout << "Continue? ";
    std::cin >> confirmation;


    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout << "\nOperation cancelled.\n";

        std::cout << "\nPress ENTER to continue...";

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
    std::cout << "Creando Symlinks...\n\n";


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
        std::cout << "\nCopy operation will not continue.\n";

        std::cout << "\nPress ENTER to continue...";

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
    std::cout << "              RESULT\n";
    std::cout << "========================================\n\n";

    std::cout << "Symlinks created: "
        << symlinksCreated
        << " / "
        << symlinks.size()
        << "\n";

    std::cout << "Elements copied: "
        << elementsCopied
        << " / "
        << copies.size()
        << "\n";

    std::cout << "\n"
        << gameName
        << " processed successfully.\n\n\n";

    std::cout << "\n========================================\n";
    std::cout << "              MISSION PASSED!\n";
    std::cout << "                   RESPECT+\n";
    std::cout << "========================================\n\n\n";


    std::cout << "\nPress ENTER to continue...";

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

    std::cout << "ENG: The original mod folder MUST remain in its location.\n";
    std::cout << "ESP: La carpeta original del mod DEBE permanecer en su ubicacion\n\n";

    std::cout << "ENG: A Symlink will be created inside your Modloader folder.\n";
    std::cout << "ESP: Se creara un 'Symlink' dentro de tu carpeta de Modloader.\n\n\n";


    // ========================================
    // SOURCE
    // ========================================

    std::cout << "MOD SOURCE path (Mod folder):\n";
    std::cout << "Ruta ORIGEN del mod (Carpeta del mod):\n\n>";
    std::getline(std::cin, sourceText);


    // ========================================
    // MODLOADER
    // ========================================

    std::cout << "\n\nDESTINATION path (MODLOADER Folder):\n";
    std::cout << "Ruta DESTINO (Carpeta MODLOADER):\n\n>";
    std::getline(std::cin, modloaderText);


    std::filesystem::path source(sourceText);
    std::filesystem::path modloader(modloaderText);


    // ========================================
    // CHECK PATHS
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "             CHECKING PATHS\n";
    std::cout << "========================================\n\n";


    if (!std::filesystem::exists(source))
    {
        std::cout << "Source: ERROR - Path does not exist.\n";
        std::cout << "Origen: ERROR - La ruta no existe.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(source))
    {
        std::cout << "Source: ERROR - Path is not a folder.\n";
        std::cout << "Origen: ERROR - La ruta no es una carpeta.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::cout << "Source: OK - Folder exists!\n";
    std::cout << "Origen: OK - La carpeta existe!\n";


    if (!std::filesystem::exists(modloader))
    {
        std::cout << "Modloader: ERROR - Path does not exist.\n\n";

        std::cout << "You must create the Modloader folder first.\n";

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    if (!std::filesystem::is_directory(modloader))
    {
        std::cout << "Modloader: ERROR - Path is not a folder.\n";

        std::cout << "\nPress ENTER to continue...";
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

        std::cout << "\nPress ENTER to continue...";
        std::cin.get();

        return;
    }


    std::filesystem::path finalDestination =
        modloader / modName;


    // ========================================
    // MOD INFORMATION
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "             MOD INFORMATION\n";
    std::cout << "========================================\n\n";

    std::cout << "Mod name:\n";
    std::cout << modName << "\n\n";

    std::cout << "Source:\n";
    std::cout << source.string() << "\n\n";

    std::cout << "Modloader:\n";
    std::cout << modloader.string() << "\n\n";

    std::cout << "Final destination:\n";
    std::cout << finalDestination.string() << "\n";


    // ========================================
    // CHECK FINAL DESTINATION
    // ========================================

    std::cout << "\n========================================\n";
    std::cout << "        CHECKING FINAL DESTINATION\n";
    std::cout << "========================================\n\n";


    if (std::filesystem::exists(finalDestination))
    {
        std::cout << "[NOT AVAILABLE]\n\n";

        std::cout << "Something already exists at:\n";
        std::cout << finalDestination.string() << "\n\n";

        std::cout << "Operation cancelled for safety.\n";

        std::cout << "\nPress ENTER to continue...";

        std::cin.get();

        return;
    }


    std::cout << "[AVAILABLE]\n";

    std::cout << "\nThe destination is available.\n";


    // ========================================
    // CONFIRMATION
    // ========================================

    char confirmation;


    std::cout << "\n========================================\n";
    std::cout << "          FINAL CONFIRMATION\n";
    std::cout << "========================================\n\n";

    std::cout << "A 'Symlink' will be created:\n";
    std::cout << "Se va a crear un 'Symlink':\n\n";

    std::cout << "MOD:\n";
    std::cout << modName << "\n\n";

    std::cout << "FROM:\n";
    std::cout << source.string() << "\n\n";

    std::cout << "TO:\n";
    std::cout << finalDestination.string() << "\n\n";

    std::cout << "[Y] Yes\n";
    std::cout << "[N] No\n\n";

    std::cout << "Continue? ";
    std::cin >> confirmation;


    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout << "\nOperation cancelled.\n";

        std::cout << "\nPress ENTER to continue...";

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
    std::cout << "Creating Symlink...\n";
    std::cout << "Creando Symlink...\n\n";


    if (createSymlink(
        source,
        finalDestination))
    {
        std::cout << "\n========================================\n";
        std::cout << "              RESULT\n";
        std::cout << "========================================\n\n";

        std::cout << "[OK] Symlink created successfully! \n\n";

        std::cout << "Mod:\n";
        std::cout << modName << "\n\n";

        std::cout << "Source:\n";
        std::cout << source.string() << "\n\n";

        std::cout << "Destination:\n";
        std::cout << finalDestination.string() << "\n\n\n";

        std::cout << "\n========================================\n";
        std::cout << "              MISSION PASSED!\n";
        std::cout << "                   RESPECT+\n";
        std::cout << "========================================\n\n\n";
    }
    else
    {
        std::cout << "\n========================================\n";
        std::cout << "              ERROR\n";
        std::cout << "========================================\n\n";

        std::cout << "Failed to create the 'Mod Symlink'.\n";
        std::cout << "No se pudo crear el 'Mod Symlink'.\n\n\n";

        std::cout << "\n========================================\n";
        std::cout << "              MISSION FAILED!\n";
        std::cout << "========================================\n\n\n";
    }


    std::cout << "\nPress ENTER to continue...";

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

        std::cout << "========================================\n\n";
        std::cout << "          GTA SYMLINK GENERATOR\n";
        std::cout << "          Here we go again...\n\n";
        std::cout << "========================================\n\n";

        std::cout << "ENG: Are you going to create a ''GTA Symlink'' or a ''Symlink mod'' (For heavy mods)? \n";
        std::cout << "ESP: Vas a crear un ''GTA Symlink'' o un ''Symlink mod'' (Para mods pesados)? \n\n\n";

        std::cout << "[1] GTA III\n";
        std::cout << "[2] GTA Vice City\n";
        std::cout << "[3] GTA San Andreas\n";
        std::cout << "[4] Create Mod Symlink\n";
        std::cout << "[5] Exit\n\n";

        std::cout << "Select an option: ";
        std::cin >> option;


        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "\nInvalid option.\n";
            std::cout << "Press ENTER to continue...";

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

                std::cout << "[1] Create Symlink\n";
                std::cout << "[2] Back to Main Menu\n\n";

                std::cout << "Select an option: ";
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "\nInvalid option.\n";
                    std::cout << "Press ENTER to continue...";

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
                    std::cout << "\nInvalid option.\n";

                    std::cout << "Press ENTER to continue...";

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

                std::cout << "[1] Create Symlink\n";
                std::cout << "[2] Back to Main Menu\n\n";

                std::cout << "Select an option: ";
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "\nInvalid option.\n";
                    std::cout << "Press ENTER to continue...";

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
                    std::cout << "\nInvalid option.\n";

                    std::cout << "Press ENTER to continue...";

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

                std::cout << "[1] Create Symlink\n";
                std::cout << "[2] Back to Main Menu\n\n";

                std::cout << "Select an option: ";
                std::cin >> gameOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "\nInvalid option.\n";
                    std::cout << "Press ENTER to continue...";

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
                    std::cout << "\nInvalid option.\n";

                    std::cout << "Press ENTER to continue...";

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

                std::cout << "[1] Create Symlink\n";
                std::cout << "[2] Back to Main Menu\n\n";

                std::cout << "Select an option: ";
                std::cin >> modOption;


                if (std::cin.fail())
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "\nInvalid option.\n";
                    std::cout << "Press ENTER to continue...";

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
                    std::cout << "\nInvalid option.\n";

                    std::cout << "Press ENTER to continue...";

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

            std::cout << "\nInvalid option.\n";

            std::cout << "Press ENTER to continue...";

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
// are property of ©Rockstar Games and ©Rockstar North.
