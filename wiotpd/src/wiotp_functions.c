#include "wiotp_functions.h"

static int create_config(IoTPConfig **config, char *orgId, char *typeId,
                          char *deviceId, char *token)
{
    IOTPRC rc = 0;

    rc = IoTPConfig_create(config, NULL);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to initialize configuration structure: rc=%d\n", rc);
        return 1;
    }

    rc = IoTPConfig_setProperty(*config, "identity.orgId", orgId);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to set configuration property");
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }

    rc = IoTPConfig_setProperty(*config, "identity.typeId", typeId);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to set configuration property");
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }

    rc = IoTPConfig_setProperty(*config, "identity.deviceId", deviceId);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to set configuration property");
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }
    
    rc = IoTPConfig_setProperty(*config, "auth.token", token);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to set configuration property");
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }
    syslog(LOG_INFO, "Configuration structure created succesfully");
    return 0;
}

static int device_create(IoTPConfig *config, IoTPDevice **device)
{
    IOTPRC rc;
    rc = IoTPDevice_create(device, config);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to configure IoTP device: rc=%d\n", rc);
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }
    syslog(LOG_INFO, "Device structure created succesfully");
    return 0;
}

static int device_connect(IoTPDevice *device)
{   
    int rc;
    rc = IoTPDevice_connect(device);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        syslog(LOG_ERR, "Returned error reason: %s\n", IOTPRC_toString(rc));
        return 1;
    }
    syslog(LOG_INFO, "Connected to device succesfully");
    return 0;
}



int init_ibm(IoTPConfig **config, IoTPDevice **device, char **orgId, char **typeId,
             char **deviceId, char **token) {

    int rc = 0;

    rc = create_config(config, *orgId, *typeId, *deviceId, *token);
    if(rc != 0){
        goto program_exit;
    }

    rc = device_create(*config, device);
    if(rc != 0){
        goto program_exit;
    }

    rc = device_connect(*device);
    if(rc != 0){
        goto program_exit;
    }

    program_exit:
    return rc;
}

IOTPRC device_disconnect(IoTPDevice *device)
{
    IOTPRC rc;

    rc = IoTPDevice_disconnect(device);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog(LOG_ERR, "Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
        return IOTPRC_FAILURE;
    }

    IoTPDevice_destroy(device);
    return IOTPRC_SUCCESS;
}

IOTPRC clear_config(IoTPConfig *config)
{
    IOTPRC rc;
    rc = IoTPConfig_clear(config);

    if (rc != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "Could not delete IoTP config: %s", IOTPRC_toString(rc));
        return IOTPRC_FAILURE;
    }
    return IOTPRC_SUCCESS;
}