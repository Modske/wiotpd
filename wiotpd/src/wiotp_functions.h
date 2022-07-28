#include <iotp_device.h>
#include <syslog.h>
#define CONFIG_ERR -1
#define DEVICE_CONNECT_ERR -2
#define DEVICE_CREATE_ERR -3

IOTPRC clear_config(IoTPConfig *config);
IOTPRC device_disconnect(IoTPDevice *device);
int init_ibm(IoTPConfig **config, IoTPDevice **device, char **orgId, char **typeId,
             char **deviceId, char **token);
