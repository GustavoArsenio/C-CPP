#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

namespace {

struct file_closer {
    void operator()(std::FILE *file) const noexcept {
        if (file != nullptr) {
            std::fclose(file);
        }
    }
};

class temporary_file {
   public:
    temporary_file() : file_(std::tmpfile()) {}

    temporary_file(const temporary_file &) = delete;
    temporary_file &operator=(const temporary_file &) = delete;
    temporary_file(temporary_file &&) noexcept = default;
    temporary_file &operator=(temporary_file &&) noexcept = default;

    bool valid() const noexcept {
        return file_ != nullptr;
    }

    bool write(const std::string &text) {
        return std::fwrite(text.data(), 1, text.size(), file_.get()) ==
               text.size();
    }

    bool rewind() {
        return std::fseek(file_.get(), 0, SEEK_SET) == 0;
    }

    std::FILE *get() const noexcept {
        return file_.get();
    }

   private:
    std::unique_ptr<std::FILE, file_closer> file_;
};

}  // namespace

int main() {
    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
        input += '\n';
    }
    if (std::cin.bad()) {
        std::cout << "error: input failure\n";
        return 1;
    }

    temporary_file spool;
    if (!spool.valid()) {
        std::cout << "error: temporary file unavailable\n";
        return 1;
    }
    if (!spool.write(input) || !spool.rewind()) {
        std::cout << "error: temporary file I/O failure\n";
        return 1;
    }

    std::size_t lines = 0;
    std::size_t bytes = 0;
    int character = 0;
    while ((character = std::fgetc(spool.get())) != EOF) {
        ++bytes;
        if (character == '\n') {
            ++lines;
        }
    }
    if (std::ferror(spool.get())) {
        std::cout << "error: temporary file I/O failure\n";
        return 1;
    }

    std::cout << "lines=" << lines << " bytes=" << bytes << '\n';
}
