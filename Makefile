default: app

BUILD_DIR ?= build

app: $(BUILD_DIR)
	$(CXX) src/main.cpp -o $(BUILD_DIR)/netrec -std=c++14

$(BUILD_DIR):
	@mkdir -p $@


