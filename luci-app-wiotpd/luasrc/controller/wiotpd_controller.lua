module("luci.controller.wiotpd_controller", package.seeall)

function index()
	entry({"admin", "services", "wiotpd"}, cbi("wiotpd_model"), _("wiotpd"),105)
end
