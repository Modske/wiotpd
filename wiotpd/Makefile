include $(TOPDIR)/rules.mk

PKG_NAME:=wiotpd
PKG_RELEASE:=1
PKG_VERSION:=1.0.7

include $(INCLUDE_DIR)/package.mk

define Package/wiotpd
	CATEGORY:=Base system
	TITLE:=wiotpd
	DEPENDS:=+libmqtt_client +libubus +libubox +libblobmsg-json 
endef

define Package/wiotpd/description
	Daemon that transfers data to an ibm watson iot cloud
endef

define Package/wiotpd/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wiotpd $(1)/usr/bin
	$(INSTALL_BIN) ./files/wiotpd.init $(1)/etc/init.d/wiotpd
	$(INSTALL_CONF) ./files/wiotpd.config $(1)/etc/config/wiotpd
endef

$(eval $(call BuildPackage,wiotpd))