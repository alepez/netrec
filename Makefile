.PHONY: default
default: app

BUILD_DIR ?= build

.PHONY: app
app: $(BUILD_DIR)/netrec

$(BUILD_DIR)/netrec: $(BUILD_DIR)
	$(CXX) src/main.cpp -o $@ -std=c++14 -pedantic -Wall -Wextra -fmessage-length=0

$(BUILD_DIR):
	@mkdir -p $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
