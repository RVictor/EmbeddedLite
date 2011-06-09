BUILD_DIR=../../../build/ReleaseUnicode
INSTALL_IMG=./debian
INSTALL_BIN=$(INSTALL_IMG)/usr/local/EmbeddedLite

install:
	rm -rf $(INSTALL_BIN)
	mkdir -p $(INSTALL_BIN)
	cp -r ../../distr/* $(INSTALL_BIN)
	cp $(BUILD_DIR)/EmbeddedLite $(INSTALL_BIN)
	cp $(BUILD_DIR)/emblite_indexer $(INSTALL_BIN)
	cp -r $(BUILD_DIR)/plugins $(INSTALL_BIN)
	cp -r $(BUILD_DIR)/debuggers $(INSTALL_BIN)
