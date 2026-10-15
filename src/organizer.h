#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

enum FileType {
    IMAGE,
    DOCUMENT,
    EXECUTABLE,
    MEDIA,
    COMPRESSED,
    NUN
};

string imageExtensions[] = {".gif", ".png", ".jpg", ".jpeg", ".webp", ".avif", ".tiff", ".tif", ".bmp", ".heic", ".heif"};
string docsExtensions[] = {".pdf", ".docx", ".doc", ".xlsx", ".xls", ".pptx", ".ppt", ".odt", ".ods", ".odp", ".txt", ".rtf", ".csv"};
string mediaExtensions[] = {".mp4", ".mkv", ".avi", ".mov", ".webm", ".mp3", ".wav", ".flac", ".aac", ".m4a"};
string cmprsExtensions[] = {".zip", ".rar", ".7z", ".tar", ".gz", ".tgz"};
string execsExtensions[] = {".exe", ".msi", ".dmg", ".pkg", ".apk", ".aab", ".deb", ".rpm"};
string* allExtensions[] = {imageExtensions, docsExtensions, mediaExtensions, cmprsExtensions, execsExtensions};

struct LFile {
    string name;
    string extension;
    FileType type = NUN;
    void getType();
};

void LFile::getType() {
    string* extensionType;

    for (string* ext : allExtensions) {
        for (int i = 0; i < ext->size(); i++) {
            if (extension == ext[i]) {
                extensionType = ext;
            }
        }
    }

    if (extensionType == imageExtensions) {
        type = IMAGE;
    } else if (extensionType == docsExtensions) {
        type = DOCUMENT;
    } else if (extensionType == mediaExtensions) {
        type = MEDIA;
    } else if (extensionType == cmprsExtensions) {
        type = COMPRESSED;
    } else if (extensionType == execsExtensions) {
        type = EXECUTABLE;
    }

};

string getDirType(FileType ext) {
    switch(ext) {
    case 0:
        return "/Images";
    case 1:
        return "/Documents";
    case 2:
        return "/Executables";
    case 3:
        return "/Media";
    case 4:
        return "/Compressed";
    case 5:
        return "/Other";
    default:
        return "/Other";
    };
}

bool isSubDir(FileType ext, string folderPath) {
    return fs::is_directory(folderPath + getDirType(ext));
}

string getFile(string folderPath, string fileName) {
    return folderPath + fileName;
}

vector<LFile> extractFiles(string folderPath) {
    fs::path folder = folderPath;
    vector<LFile> contents;
    for (const auto& file : fs::directory_iterator(folder)) {
        if (!fs::is_regular_file(file.path()))
            continue;
        
        LFile content;
        content.extension = file.path().extension().string();
        content.name = file.path().filename().string();
        content.getType();
        contents.push_back(content);
    }
    return contents;
}

bool createDir(vector<LFile> files, string folderPath) {
    try {
        for (LFile file : files) {
            if (!isSubDir(file.type, folderPath)) {
                fs::create_directory(folderPath + getDirType(file.type));
            }
        }

        return true;
    } catch (fs::filesystem_error& err) {
        cerr << "Error: " << err.what() << "\n";
        return false;
    }
}

bool moveFiles(vector<LFile> files, string folderPath) {
    try {
        for (LFile file : files) {
            fs::rename(fs::path(folderPath) / file.name, fs::path(folderPath) / getDirType(file.type).substr(1) / file.name);
        }
        return true;

    } catch (fs::filesystem_error& err) {
        cerr << "Error: " << err.what() << "\n";
        return false;
    }

}

bool organize(string folderPath) {
    vector<LFile> dirFiles;
    dirFiles = extractFiles(folderPath);
    
    if (!createDir(dirFiles, folderPath)) {
        cerr << "Error: Could not create directories!\n";
        return false;
    }
    
    if (!moveFiles(dirFiles, folderPath)) {
        cerr << "Error: Could not move files!\n";
        return false;
    }
    
    cout << "Successfuly Organized!";
    return true;
}
