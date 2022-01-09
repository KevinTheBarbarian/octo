
#ifndef __DIR_ENTRY_HH__
#define __DIR_ENTRY_HH__

#include <string>
#include <vector>
#include <memory>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace sys {
    // Collects file information on construction.
    class DirEntry {
    public:
        explicit DirEntry(const std::string& path);

        bool exists() const;
        bool isDir() const;
        bool isFile() const;

        size_t getFileLength() const;

        std::vector<DirEntry> readDir() const;
        std::string           readFile() const;

        std::string getPath() const
        {
            return path_;
        }

        static std::string combinePaths(const std::string& one,
                                        const std::string& two);

    private:
        const struct stat* getStat_() const;

        void verifyExists_() const;

        DIR* openDir_() const;
        static std::string readEntry_(DIR* dirp);

        std::string                  path_;
        std::unique_ptr<struct stat> fileInfo_;
    };
}

#endif

