map = Map("wiotpd")

section = map:section(NamedSection, "wiotpd_sct", "wiotpd", "Wiotpd section")

flag = section:option(Flag, "enable", "Enable", "Enable program")

orgId = section:option( Value, "orgId", "orgId")

typeId = section:option( Value, "typeId", "typeId")

deviceId = section:option( Value, "deviceId", "deviceId")

token = section:option( Value, "auth_token", "Token")

return map
