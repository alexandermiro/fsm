#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <filesystem>
#include <memory>
#include <fstream>
#include <optional>
#include <tuple>
#include <string_view>
#include <utility>      // std::exchange

#include "common.h"


namespace fsm::util::io {


namespace fs = std::filesystem;

/**
 * @brief Manipulate a filesystem path
 */
class Path final {
public:
    /**
     * @brief Ctor
     * @param spath a path that could be a absolute/relative to a file
     */
    explicit Path(std::string const& spath) noexcept(false) {
        if (spath.size() == 0) throw except::create_anomaly("invalid empty path parameter");
        path_.reset(new fs::path{spath.data()});
        status_ = fs::status(*path_.get());
        set_type();
    }
    Path() = delete;

    /**
     * @brief Ctor - joins the path and a filename
     * @param spath the path
     * @param filename the filename
     */
    explicit Path(std::string_view spath, std::string_view filename) noexcept(false) {
        if (spath.size() == 0) throw except::create_anomaly("invalid empty path parameter");
        if (filename.size() == 0) throw except::create_anomaly("invalid empty filename parameter");
        path_.reset(new fs::path{spath.data()});
        path_->append(filename.data());
        status_ = fs::status(*path_.get());
        set_type();
    }

    /**
     * @brief Path
     * @param other
     */
    Path(Path const& other)
        : type_(other.type_)
        , status_(other.status_)
        , path_(new fs::path(*other.path_.get())) {

    }

    /**
     * @brief Ctor - copy
     * @param p the source to be copied from
     */
    Path(Path&& p)
        : type_(p.type_)
        , status_(std::move(p.status_))
        , path_(std::move(p.path_)) {
    }

    /**
     * @brief Ctor - assignment
     * @param rhs the source to be copied from
     */
    Path& operator=(Path const& rhs) {
        if (this != &rhs) {
            type_   = rhs.type_;
            status_ = rhs.status_;
            path_.reset(new fs::path{rhs.absolute()});
        }
        return *this;
    }

    Path& operator=(Path&& rhs) {
        if (this != &rhs) {
            type_   = std::exchange(rhs.type_, Path::Type::DOES_NOT_EXISTS_YET);
            status_ = std::move(rhs.status_);
            path_   = std::exchange(rhs.path_, nullptr);
        }
        return *this;
    }

    /**
     * @brief Dctor
     */
    ~Path() {}

    [[nodiscard]] bool is_regular_file() const {
        return type_ == Type::REGULAR_FILE;
    }

    [[nodiscard]] bool is_directory() const {
        return type_ == Type::DIRECTORY;
    }

    [[nodiscard]] bool is_symlink() const {
        return type_ == Type::SYMLINK;
    }

    [[nodiscard]] bool is_socket() const {
        return type_ == Type::SOCKET;
    }

    [[nodiscard]] bool exists() const {
        return fs::exists(*path_.get());
    }

    /**
     * @brief Returns the absolute path
     */
    [[nodiscard]] std::string absolute() const {
        return std::string{path_->c_str()};
    }

    /**
     * @brief Verifies if the given path is a valid one
     * @return path
     */
    [[nodiscard]] static bool exists(std::string_view path) noexcept {
        fs::path p{path.data()};
        return fs::exists(p);
    }

    /**
     * @brief append
     * @param elem
     */
    void append(std::string_view elem) noexcept(false) {
        if (elem.size() == 0) throw except::create_anomaly("cannot append empty value to the path");
        path_->append(elem.data());
        status_ = fs::status(*path_.get());
        set_type();
    }

    /**
     * @brief operator /=
     * @param p
     * @param elem
     * @return
     */
    friend Path& operator/=(Path& p, std::string_view elem) noexcept(false) {
        if (elem.size() == 0) throw except::create_anomaly("invalid empty value to append in a path");
        p.append(elem.data());
        return p;
    }

    /**
      * @brief operator /
      * @param p
      * @param elem
      * @return
      */
    friend Path operator/(Path const& left, Path const& right) {
        Path p{left.absolute(), right.absolute()};
        return p;
    }

    /**
     * @brief Returns the filename and it's parts
     * @return a tuple containing:
     *           1st - the full filepath
     *           2nd - the filename without the extension
     *           3th - the extension where the first character is dot '.'
     *           4th - parent path
     * @see FileHelper_gtest.h
     */
    using filename_name_ext_t = std::tuple<std::string, std::string, std::string, std::string>;
    std::optional<filename_name_ext_t> filename_parts() const noexcept {
        if (!path_) return std::nullopt;
        if (auto full_name = path_->filename(); !full_name.empty()) {
            auto just_name_opt = stem();
            std::string ext{path_->extension().c_str()};
            ext = (ext == ".") ? std::string{} : ext;
            std::string parent = path_->parent_path().c_str();

            return std::make_optional<filename_name_ext_t>(
                        std::make_tuple(full_name
                                       , just_name_opt.value_or("")
                                       , ext
                                       , parent));
        }
        return std::nullopt;
    }

    /**
     * @brief stem
     * @return just the stem part of the fullpath
     */
    std::optional<std::string> stem() const noexcept {
        if (!path_) return std::nullopt;
        if (auto name_withoud_ext = path_->stem(); !name_withoud_ext.empty()) {
            return std::make_optional<std::string>(name_withoud_ext.c_str());
        }
        return std::nullopt;
    }

private:
    enum class Type {
          DOES_NOT_EXISTS_YET = 0
        , REGULAR_FILE
        , DIRECTORY
        , SYMLINK
        , SOCKET
    };

    void set_type() {
        if (fs::is_regular_file(status_)) {
            type_ = Type::REGULAR_FILE;
        } else if (fs::is_directory(status_)) {
            type_ = Type::DIRECTORY;
        } else if (fs::is_symlink(status_)) {
            type_ = Type::SYMLINK;
        } else if (fs::is_socket(status_)) {
            type_ = Type::SOCKET;
        }
    }

    Type                                type_{Type::DOES_NOT_EXISTS_YET};
    fs::file_status                     status_;
    std::unique_ptr<fs::path>           path_{nullptr};
};

//Path& operator/=(Path& p, std::string_view elem) {
//    if (elem.size() == 0) throw except::create_anomaly("invalid empty value to append in a path");
//    p.append(elem.data());
//    return p;
//}

//Path operator/(Path const& left, Path const& right) {
//    Path p{left.absolute(), right.absolute()};
//    return p;
//}

/**
 * @brief A basic file helper to manipulate a file
 */
class FileHelper {
public:
    explicit FileHelper(Path const& p) : path_{p} {

    }
    /**
     * @brief Ctor
     * @param p the path to the file
     */
    explicit FileHelper(Path&& p) : path_{std::move(p)} {

    }

    /**
     * @brief Dctor
     */
    ~FileHelper() {
        if (stream_.is_open()) {
            stream_.flush();
            stream_.close();
        }
    }

    /**
     * @brief Creates a new file and its parent directories if not exist.
     * @note TRUNCATES a previous existing file
     * @path the path to the file to be created
     * @return true if the file was created successfully, false otherwise.
     */
    static [[nodiscard]] bool create(Path const& p) {
        auto fparts = p.filename_parts();
        if (fparts.has_value()) {
            std::string full_path, just_name, ext, parent_path;
            std::tie(full_path, just_name, ext, parent_path) = fparts.value();
            if (!Path::exists(parent_path)) {
                fs::create_directories(parent_path);
            }
            if (std::ofstream ofs(p.absolute());  ofs.is_open()) {
                ofs.close();
            }
        }
        return p.exists();
    }

    bool create() {
        auto fparts = path_.filename_parts();
        if (fparts.has_value()) {
            auto [full_path, just_name, ext, parent_path] = fparts.value();
            if (!Path::exists(parent_path)) {
                fs::create_directories(parent_path);
            }
            if (std::ofstream ofs(path_.absolute());  ofs.is_open()) {
                ofs.close();
            }
        }
        return path_.exists();
    }

    /**
     * @brief open_from_scracth
     * @return
     */
    bool open_from_scratch() {
        stream_.open(path_.absolute(), std::ios_base::out | std::ios_base::trunc);
        return stream_.is_open();
    }

    /**
     * @brief filepath
     */
    auto filepath() const {
        return path_.absolute();
    }

    void flush() {
        stream_.flush(); // does nothing in case of null rdbuf
    }

    void write(std::string_view data) {
        stream_ << data.data();
    }

private:
    std::fstream stream_;
    Path         path_;
};


}   // namespace fsm::util::io

#endif // FILEHELPER_H
