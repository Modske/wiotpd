include $(TOPDIR)/rules.mk
PREFIX:=/usr

PKG_NAME:=libmqtt_client
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c.git
PKG_SOURCE_VERSION:=f921dce616af815da2236954c608a2f377c928c5


include $(INCLUDE_DIR)/package.mk

define Package/libmqtt_client
	CATEGORY:=Libraries
	TITLE:=libmqtt_client
	DEPENDS:=+libpthread +libopenssl
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)$(PREFIX)/lib $(1)$(PREFIX)/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3*.so* $(1)$(PREFIX)/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/*.h $(1)$(PREFIX)/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib*.so* $(1)$(PREFIX)/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/*.h $(1)$(PREFIX)/include
endef

define Package/libmqtt_client/install
	$(INSTALL_DIR) $(1)$(PREFIX)/lib 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3*.so* $(1)$(PREFIX)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib*.so* $(1)$(PREFIX)/lib/
endef

$(eval $(call BuildPackage,libmqtt_client))
