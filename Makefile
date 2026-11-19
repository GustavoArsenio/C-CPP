BUILD_DIR ?= build
BUILD_TYPE ?= Debug
CLANG_FORMAT ?= clang-format-18
PYTHON ?= python3

.PHONY: all configure test clean format format-check

all: configure
	cmake --build $(BUILD_DIR) --parallel

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

test: all
	ctest --test-dir $(BUILD_DIR) --output-on-failure

format:
	$(PYTHON) tools/validation/format_code.py --clang-format "$(CLANG_FORMAT)" --fix

format-check:
	$(PYTHON) tools/validation/format_code.py --clang-format "$(CLANG_FORMAT)"

clean:
	cmake -E rm -rf $(BUILD_DIR)
