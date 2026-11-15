#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif

std::filesystem::path dataDir() {
#ifdef _WIN32
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(NULL, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path();
#else
    return ".";
#endif
}

#ifdef _WIN32
void setupDllDirectory() {
    std::filesystem::path dllPath = dataDir() / "dlls";
    SetDllDirectoryW(dllPath.c_str());
}
#endif 

std::string chapterFixer(std::string chapter) {
    if (chapter.length() == 1) {
        chapter = "0" + chapter;
    }
    return chapter;
}

int rowCounter(std::ifstream& file, std::string line) {
    int rows = 0;
    while (getline(file, line)) {
        rows++;
    }
    file.clear();
    file.seekg(0);
    return rows;
}



void verseReader(const std::string& book, const std::string& chapter, const int& first, const int& last, std::ifstream& file, const bool& divider = false, const bool& verse_nl = false, const bool& wholeChapter = false, const bool& quote = false) {
    int i = 0;
    std::string line;
    while (getline(file, line)) {
        if (i>1) {
            if (i>=first && i<=last) {
                if (i == first && quote == true) {
                    std::cout << "\"";
                }
                if (i != last) {
                    if (divider == true) {
                        if (!line.empty()) line.pop_back();
                        std::cout << line;
                        std::cout << "\n";
                    } else {
                        std::cout << line;
                    }
                } else if (i == last && quote == true) {
                    if (!line.empty()) line.pop_back();
                    std::cout << line;
                    std::cout << "\""; 
                } else {
                    std::cout << line;
                }
            }
        }
        i++;
    }
    if (verse_nl == true) {
        std::cout << "\n";
    } else if (quote == true) {
        std::cout << " ";
    }

    std::cout << book << " " << chapter;
    if (!wholeChapter) {
        std::cout << ":" << (first-1);
        if (first != last) {
            std::cout << "-" << (last-1);
        }
    }
    file.close();
}

using json = nlohmann::ordered_json;

int main(int argc, char* argv[]) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        setupDllDirectory();
    #endif
    
    std::string version = "1.0";
    std::string bibleVersion = "WEBC";
    std::string optionsMsg = "These are the available options for bible-CLI:\n-h, --help                           Shows this help message\n-v, --version                        Shows the version of Bible-CLI\n-p, --passage                        Set the Bible passage you want to read\n-nl, --new-line                      Makes every verse be outputed in a new line\n-pnl, --passage-new-line             Makes the Bible passage be outputed in a new line\n-q, --quote                          Adds quotation makrs at the begining and end of the output (doesn't include the passage)\n-b, --books                          Shows all the books the current version (" + bibleVersion + ") of the bible contains\n-r, --random                         Outputs a random passage";
    std::string book;
    std::string chapter = "";
    std::string verse = "";
    std::ifstream jsonFile(dataDir() / "books.json");
    json data;
    jsonFile >> data;

    std::string shortBook;
    std::string bookNumber;
    std::string fileChapter;
    std::filesystem::path filePath;
    std::string line;
    
    bool optionProblem = false;
    bool showVersion = false;
    bool help = false;
    bool random = false;
    bool greenLight = true;

    bool nl = false;
    bool pnl = false;
    bool quoteOption = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-p" || arg == "--passage") {
            if (i + 2 >= argc) {
                std::cout << "ERROR: Missing book or passage.";
                return 1;
            }
            book = argv[i + 1];
            std::string passage_part = argv[i + 2];

            if (passage_part.find(":") != std::string::npos) {
                chapter = passage_part.substr(0, passage_part.find(":"));
                verse = passage_part.substr(passage_part.find(":") + 1);
            } else {
                chapter = passage_part;
            }
            i += 2;

        } else if (arg == "-nl" || arg == "--new-line") {
            nl = true;
        } else if (arg == "-pnl" || arg == "--passage-new-line") {
            pnl = true;
        } else if (arg == "-q" || arg == "--quote") {
            quoteOption = true;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << optionsMsg;
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "Bible-CLI " << version;
            return 0;
        } else if (arg == "-b" || arg == "--books") {
            for (auto jsonbooks = data.begin(); jsonbooks != data.end(); ++jsonbooks) {
                std::cout << jsonbooks.key() << "\n";
            }
            return 0;
        } else if (arg == "-r" || arg == "--random") {
            srand(time(0));
            int bookIndex = rand() % data.size();
            auto bookIt = std::next(data.begin(), bookIndex);
            book = bookIt.key();
            shortBook  = bookIt.value()["code"].get<std::string>();
            bookNumber = bookIt.value()["number"].get<std::string>();
            int chapterCount = bookIt.value()["chapters"].get<int>();
            chapter = std::to_string(rand() % chapterCount + 1);
            fileChapter = chapterFixer(chapter);
            filePath = dataDir() / "bible" / ("eng-web-c_" + bookNumber + "_" + shortBook + "_" + fileChapter + "_read.txt");

            std::ifstream randFile(filePath);
            int verseCount = rowCounter(randFile, line) - 2;
            if (verseCount < 1) {
                std::cout << "ERROR: Could not read " << book << " " << chapter << ".";
                return 1;
            }
            int first = rand() % verseCount + 1;
            verse = std::to_string(first);
            if (first < verseCount && rand() % 2 == 1) {
                int last = first + 1 + rand() % (verseCount - first);
                verse += "-" + std::to_string(last);
            }
            random = true;
        } else {
            std::cout << "ERROR: You have an unknown option. " << optionsMsg;
            return 1;
        }
    }    

    if (book.empty() || chapter.empty()) {
        std::cout << "ERROR: Missing passage or passage is incorrect.";
        return 1;
    }

    if (!jsonFile.is_open()) {
        std::cout << "ERROR: Could not open books.json.";
        return 1;
    }

    if (data.contains(book)) {
        if (!random) {
            shortBook   = data[book]["code"].get<std::string>();
            bookNumber  = data[book]["number"].get<std::string>();
            fileChapter = chapterFixer(chapter);
            filePath = dataDir() / "bible" / ("eng-web-c_" + bookNumber + "_" + shortBook + "_" + fileChapter + "_read.txt");
        }
        std::ifstream curentFile(filePath);
        std::string line;
        int totalRows = rowCounter(curentFile, line);

        if (std::filesystem::exists(filePath)) {
            int firstRow = 0;
            int lastRow = 0;
            bool wholeChapter = false;
            if (!verse.empty()) {
                if (verse.find("-") != std::string::npos) {
                    if (verse.find("-") == 0 || verse.find("-") + 1 >= verse.length()) {
                        std::cout << "ERROR: Incorrect passage format.";
                        return 1;
                    }
                    try {
                        firstRow = stoi(verse.substr(0, verse.find('-')));
                        lastRow = stoi(verse.substr(verse.find('-')+1));
                    } catch (...) {
                        std::cout << "ERROR: Incorrect passage format.";
                        return 1;
                    }
                } else {
                    try {
                        firstRow = stoi(verse);
                    } catch (...) {
                        std::cout << "ERROR: Incorrect passage format.";
                        return 1;
                    }
                    lastRow = firstRow;
                }
            } else {
                wholeChapter = true;
                firstRow = 1;
                lastRow = totalRows - 2;
            }
            firstRow++;
            lastRow++;

            if (lastRow <= totalRows-1) {
                if (firstRow <= lastRow && firstRow >= 2) {
                    verseReader(book, chapter, firstRow, lastRow, curentFile, nl, pnl, wholeChapter, quoteOption);
                } else if (firstRow > lastRow) {
                    std::cout << "ERROR: The verse " << verse << " is typed incorectly. The first number has to be smaller than the second one or not equal.";
                } else {
                    std::cout << "ERROR: The verse " << verse << " is typed incorectly. One of the numbers is less or equal to 0.";
                }
            } else {
                std::cout << "ERROR: There is no verse " << verse << " in " << book << " chapter " << chapter << " in the " << bibleVersion << " version of the bible";
            }
        } else {
            std::cout << "ERROR: There is no chapter " << chapter << " in the book " << book << " in the " << bibleVersion << " version of this bible.";
        }
    } else {
            std::cout << "ERROR: There is no book named " << book << " in the " << bibleVersion << " version of this bible.";
    }
    return 0;
}