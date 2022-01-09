
#include "DirEntry.hh"
#include "Slice.hh"
#include "verify.hh"

#include <fstream>
#include <stdexcept>

#include <assert.h>

using namespace general;
using namespace std;

namespace sys {
    DirEntry::DirEntry(const string& path)
        : path_     (path),
          fileInfo_ (new struct stat)
    {
        int rc = stat(path_.c_str(), &* fileInfo_);

        if (rc < 0) {
            fileInfo_.reset();
        }
    }

    bool DirEntry::exists() const
    {
        return fileInfo_.get();
    }

    bool DirEntry::isFile() const
    {
        return exists() && S_ISREG(fileInfo_->st_mode);
    }

    bool DirEntry::isDir() const
    {
        return exists() && S_ISDIR(fileInfo_->st_mode);
    }

    void DirEntry::verifyExists_() const
    {
        verify(exists(), "file does not exist");
    }

    size_t DirEntry::getFileLength() const
    {
        verifyExists_();
        return fileInfo_->st_size;
    }

    namespace {
        struct DirPtr {
            DirPtr(DIR* dirptr)
                : dirPtr_(dirptr)
            {
            }

            ~DirPtr()
            {
                if (dirPtr_) {
                    closedir(dirPtr_);
                    dirPtr_ = 0;
                }
            }

            DIR* dirPtr_;
        };

        string combineDirPaths(CharSlice one, CharSlice two)
        {
            if (0)
                cout << "combine paths, one='"
                     << one << "', two = '"
                     << two << "': ";

            if (two.find("/") != CharSlice::npos) {
                throw runtime_error("combineDirPaths: filename contains /");
            }

            if (two.empty()) {
                throw runtime_error("combineDirPaths: ERROR, empty filename provided");
            }

            if (one.empty() || one == ".") {
                return two.toObject<string>();
            }

            if (two == ".") {
                return one.toObject<string>();
            }

            if (endMatches(one, "/")) {
                one.removeBack(1);
            }

            if (endMatches(one, "/")) {
                one.removeBack(1);
            }

            string result;
            result.reserve(one.size()+1+two.size());

            result += one;
            result += "/";
            result += two;

            //cout << "result = '" << result << "'" << endl;

            return result;
        }

        template<class T, class U>
        string combineDirPaths(const T& one, const U& two)
        {
            return combineDirPaths(CharSlice(one), CharSlice(two));
        }
    }

    vector<DirEntry> DirEntry::readDir() const
    {
        verifyExists_();

        DirPtr dir(openDir_());

        vector<DirEntry> rv;

        string dirName;
        while(! (dirName = readEntry_(dir.dirPtr_)).empty()) {
            string newPath = combineDirPaths(path_, dirName);
            rv.push_back(DirEntry(newPath));
        }

        return rv;
    }

    DIR* DirEntry::openDir_() const
    {
        if (! isDir()) {
            throw runtime_error("Not a directory");
        }

        DIR* dirp = opendir(path_.c_str());

        if (! dirp) {
            throw runtime_error(string("Could not open directory '") + path_ + "'");
        }

        return dirp;
    }

    string DirEntry::readEntry_(DIR* dirp)
    {
        struct dirent  entry;
        struct dirent* result = 0;

        int rc = readdir_r(dirp, & entry, & result);

        if (rc) {
            throw runtime_error("Error while reading directory");
        }

        if (! result) {
            return string();
        }

        return string(result->d_name);
    }

    string DirEntry::readFile() const
    {
        verifyExists_();

        string contents;
        contents.reserve(getFileLength());

        ifstream f(path_.c_str());
        char buf[4096];

        while(f) {
            f.read(buf, sizeof(buf));

            size_t amt = f.gcount();

            if (amt <= 0) {
                break;
            }

            contents.append(buf, amt);
        }

        return contents;
    }

    string DirEntry::combinePaths(const string& one, const string& two)
    {
        return combineDirPaths(one, two);
    }
}

