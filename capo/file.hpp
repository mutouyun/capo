/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/detect_plat.hpp"
#include "capo/noncopyable.hpp"
#include "capo/force_inline.hpp"
#include "capo/vector.hpp"

#include <cstdio>       // std::fopen, ...
#include <ios>          // std::ios_base, std::streamoff
#include <utility>      // std::swap, std::move
#include <cstring>      // std::memcpy
#include <algorithm>    // std::min
#include <iterator>     // std::advance, std::next, std::distance

#if defined(CAPO_OS_WIN_)
#   include <io.h>      // ::_get_osfhandle
#   include <windows.h> // ::SetEndOfFile
#else
#   include <unistd.h>  // ::ftruncate
#endif

namespace capo {

////////////////////////////////////////////////////////////////
/// The base class of file
////////////////////////////////////////////////////////////////

struct file : capo::noncopyable
{
    using off_type  = std::streamoff;
    using size_type = std::streamsize;
    using buf_type  = capo::vector<unsigned char>;

    enum : off_type { OutOfRange = -1 };

    class seeker
    {
        file&    file_;
        off_type tell_;

    public:
        seeker(file& f)
            : file_(f)
            , tell_(file::OutOfRange)
        {
            tell_ = file_.tell();
        }

        ~seeker(void)
        {
            if (tell_ == file::OutOfRange) return;
            file_.seek(tell_, std::ios_base::beg);
        }
    };

    virtual ~file(void) {}

	virtual bool close(void) = 0;
	virtual bool clear(void) = 0;
	virtual file::size_type read(buf_type* buff) = 0;
	virtual file::size_type write(const buf_type& buff) = 0;

	virtual bool seek(file::off_type off, std::ios_base::seekdir way = std::ios_base::cur) = 0;
	virtual file::off_type  tell(void) = 0;
	virtual file::size_type size(void) = 0;

    virtual bool set_size(file::size_type size) = 0;
	virtual bool flush(void) = 0;
	virtual bool eof(void) = 0;
	virtual bool error(void) = 0;

    void clone(file& rhs)
    {
        if (error()) return;
        if (!rhs.clear()) return;
        file::seeker handle(*this);
        if (!seek(0, std::ios_base::beg)) return;
        buf_type buff(2 << 10); // 1024 bytes
        while (read(&buff) != 0)
            if (rhs.write(buff) != buff.size())
                break;
    }
};

////////////////////////////////////////////////////////////////
/// IO file
////////////////////////////////////////////////////////////////

class io_file : public file
{
    std::FILE * file_ = nullptr;
    std::string open_path_, open_mode_;

public:
    io_file(void) {}

    explicit io_file(const std::string& path, std::ios_base::openmode mode = 
                                              std::ios_base::in | std::ios_base::out | std::ios_base::trunc)
    {
        open(path, mode);
    }

    ~io_file(void) { close(); }

public:
    bool open(const std::string& path, std::ios_base::openmode mode = 
                                       std::ios_base::in | std::ios_base::out | std::ios_base::trunc)
    {
        open_path_ = path;
        if (mode & std::ios_base::in)       // input
        {
            open_mode_ = "r";
        }
        if (mode & std::ios_base::out)      // output
        {
            if (mode & std::ios_base::in)
                open_mode_ = "r+";
            else
                open_mode_ = "w";
        }
        if (mode & std::ios_base::ate || 
            mode & std::ios_base::app)      // at end || append
        {
            if (mode & std::ios_base::in)
                open_mode_ = "a+";
            else
                open_mode_ = "a";
        }
        else if (mode & std::ios_base::trunc) // truncate
        {
            if (mode & std::ios_base::in)
                open_mode_ = "w+";
            else
                open_mode_ = "w";
        }
        if (mode & std::ios_base::binary)   // binary
        {
            open_mode_ += "b";
        }
        if (file_ == nullptr)
            file_ = std::fopen(open_path_.c_str(), open_mode_.c_str());
        else
            file_ = std::freopen(open_path_.c_str(), open_mode_.c_str(), file_);
        return (file_ != nullptr);
    }
    bool close(void)
    {
        if (file_ == nullptr) return true;
        return std::fclose(file_) == 0;
    }
    bool clear(void)
    {
        if (file_ == nullptr) return false;
        file_ = std::freopen(open_path_.c_str(), "w", file_);
        if (file_ == nullptr) return false;
        file_ = std::freopen(open_path_.c_str(), open_mode_.c_str(), file_);
        return (file_ != nullptr);
    }

    file::size_type read(buf_type* buff)
    {
        if (file_ == nullptr) return 0;
        if (buff == nullptr || buff->empty()) return 0;
        return std::fread(buff->data(), sizeof(buf_type::value_type), buff->size(), file_);
    }
    file::size_type write(const buf_type& buff)
    {
        if (file_ == nullptr) return 0;
        if (buff.empty()) return 0;
        return std::fwrite(buff.data(), sizeof(buf_type::value_type), buff.size(), file_);
    }

    bool seek(file::off_type off, std::ios_base::seekdir way = std::ios_base::cur)
    {
        if (file_ == nullptr) return false;
        int origin = SEEK_CUR;
        switch(way)
        {
        case std::ios_base::beg: origin = SEEK_SET; break;
        case std::ios_base::end: origin = SEEK_END; break;
        }
        return std::fseek(file_, (long)off, origin) == 0;
    }
    file::off_type tell(void)
    {
        if (file_ == nullptr) return 0;
        auto t = std::ftell(file_);
        return (t == -1L) ? file::OutOfRange : t;
    }
    file::size_type size(void)
    {
        file::seeker handle(*this);
        if (!seek(0, std::ios_base::end)) return 0;
        return tell();
    }

    bool set_size(file::size_type size)
    {
        if (file_ == nullptr) return false;
#   if defined(CAPO_OS_WIN_)
        file::seeker handle(*this);
        if (!seek(size, std::ios_base::beg)) return false;
        HANDLE hfile = (HANDLE)::_get_osfhandle(::_fileno(file_));
        return !!::SetEndOfFile(hfile);
#   else
        return ::ftruncate(::fileno(file_), size) == 0;
#   endif
    }
    bool flush(void) { return (file_ == nullptr) ? false : std::fflush(file_) == 0; }
    bool eof  (void) { return (file_ == nullptr) ? true  : std::feof  (file_) != 0; }
    bool error(void) { return (file_ == nullptr) ? true  : std::ferror(file_) != 0; }
};

////////////////////////////////////////////////////////////////
/// Memory file
////////////////////////////////////////////////////////////////

class mem_file : public file
{
    buf_type buff_;
    buf_type::iterator position_;

public:
    mem_file(void) { open(); }

    explicit mem_file(const buf_type& buff)
    {
        open(buff);
    }
    explicit mem_file(buf_type&& buff)
    {
        open(std::move(buff));
    }
    explicit mem_file(const buf_type::value_type* buff, std::size_t size)
    {
        open(buff, size);
    }

    ~mem_file(void) { close(); }

public:
    bool open(void) { return clear(); }
    bool open(const buf_type& buff)
    {
        buff_ = buff;
        position_ = buff_.begin();
        return true;
    }
    bool open(buf_type&& buff)
    {
        buff_ = std::move(buff);
        position_ = buff_.begin();
        return true;
    }
    bool open(const buf_type::value_type* buff, std::size_t size)
    {
        buff_.resize(size);
        std::memcpy(buff_.data(), buff, size);
        position_ = buff_.begin();
        return true;
    }
    bool close(void)
    {
        buff_.clear();
        position_ = buff_.begin();
        return true;
    }
    bool clear(void) { return close(); }

    file::size_type read(buf_type* buff)
    {
		if (error() || eof()) return 0;
        if (buff == nullptr || buff->empty()) return 0;
        std::size_t s = (std::min)(buff->size(), (std::size_t)(size() - tell()));
        std::memcpy(buff->data(), &(*position_), s);
        std::advance(position_, s);
        return s;
    }
    file::size_type write(const buf_type& buff)
    {
        if (error()) return 0;
        if (buff.empty()) return 0;
        file::off_type t = tell();
        if (t == file::OutOfRange) return 0;
        std::size_t s = (std::size_t)size(), sr = (std::size_t)(t + buff.size());
        if (s < sr) buff_.resize(sr);
        position_ = std::next(buff_.begin(), (long)t); // relocate
        std::memcpy(&(*position_), buff.data(), buff.size());
        std::advance(position_, buff.size());
        return buff.size();
    }

    bool seek(file::off_type off, std::ios_base::seekdir way = std::ios_base::cur)
    {
        if (error()) return false;
        switch(way)
        {
        case std::ios_base::cur:
            std::advance(position_, (long)off);
            break;
        case std::ios_base::beg:
            position_ = std::next(buff_.begin(), (long)off);
            break;
        case std::ios_base::end:
            position_ = std::next(buff_.end(), (long)off);
            break;
        }
        return true;
    }
    file::off_type tell(void)
    {
        if (buff_.empty()) return 0;
        if (error()) return file::OutOfRange;
        file::off_type t = std::distance(buff_.begin(), position_);
        return (t < 0) ? file::OutOfRange : t;
    }
    file::size_type size(void)
    {
        if (error()) return 0;
        return buff_.size();
    }

    bool set_size(file::size_type size)
    {
        if (error()) return false;
        file::off_type t = tell();
        if (t == file::OutOfRange) return false;
        buff_.resize((std::size_t)size);
        position_ = std::next(buff_.begin(), (long)t); // relocate
        return true;
    }
    bool flush(void) { return !error(); }
    bool eof(void)
    {
        file::off_type t = tell();
        return error() ? true : (t >= size() || t < 0);
    }
    bool error(void) { return false; }
};

////////////////////////////////////////////////////////////////

} // namespace capo
