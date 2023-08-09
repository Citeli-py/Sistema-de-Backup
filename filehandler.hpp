#include <iostream>
#include <string>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

void copy(const std::string& origem, const std::string& destino) {
    // Check if the source file exists
    if (!fs::exists(origem)) {
        std::cout << "Source file does not exist: " << origem << std::endl;
        return;
    }

    // Check if the destination directory exists
    if (!fs::exists(destino)) {
        std::cout << "Destination directory does not exist: " << destino << std::endl;
        return;
    }

    // Get the destination file path by appending the source file name to the destination directory
    std::string destinationFilePath = destino + "\\" + fs::path(origem).filename().string();

    // Copy the file
    try {
        fs::copy_file(origem, destinationFilePath, fs::copy_options::overwrite_existing);
        std::cout << "File copied successfully: " << destinationFilePath << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Failed to copy the file: " << ex.what() << std::endl;
        return;
    }
}

void createDir(const std::string& caminho) {
    // Check if the directory already exists
    if (fs::exists(caminho))
        std::cout << "Directory already exists: " << caminho << std::endl;

    else {
        // Create the directory
        if (fs::create_directory(caminho)) 
            std::cout << "Directory created successfully: " << caminho << std::endl;
        else 
            std::cout << "Failed to create the directory: " << caminho << std::endl;
    }
}

std::string GetLastModificationTime(const std::string filePath) {
    std::string cmd = "python teste_stat.py " + filePath;
    system(cmd.c_str());
    std::cout << "teste1\n";

    FILE *f = fopen("temp.txt", "r");
    if (f == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return "0";
    }

    char Linha[100];
    int i = 1;
    while (!feof(f))
    {
    // Lê uma linha (inclusive com o '\n')
        auto result = fgets(Linha, 100, f);  // o 'fgets' lê até 99 caracteres ou até o '\n'
        if (result)  // Se foi possível ler
            printf("Linha %d : %s",i,Linha);
        i++;
    }
    fclose(f);
    return "0";
}