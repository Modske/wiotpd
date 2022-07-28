#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/file.h>
#include <argp.h>

#include "iotp_device.h"
#include "wiotp_functions.h"
#include "system_info.h"
#include "lock.h"

typedef struct arguments
{
    char *args[4];
    char *orgId, *typeId, *deviceId, *token;
}arguments;

struct argp_option argp_options[] =
{
    {"orgId",    'o', "ORGID",    0, "Provide org id"},
    {"typeId",   't', "TYPEID",   0, "Provide type id"},
    {"deviceId", 'd', "DEVICEID", 0, "Provide device id"},
    {"token",    'a', "TOKEN",    0, "Provide authentication token"},
	{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    arguments *arguments = state->input;
    switch(key)
    {
        case 'o': 
            arguments->orgId = arg;
            break;
        case 't':
            arguments->typeId = arg;
            break;
        case 'd':
            arguments->deviceId = arg;
            break;
        case 'a':
            arguments->token = arg;
            break;

		case ARGP_KEY_ARG:
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if ((!arguments->orgId) || (!arguments->typeId) || (!arguments->deviceId)
                || (!arguments->token))
		{
			argp_usage(state);
		}
		break;
		default:
			return ARGP_ERR_UNKNOWN;
    }
	return 0;
}

struct argp argp = {argp_options, parse_opt};

volatile int daemon_handler = 0;

void sigHandler(int sig) 
{
    if(sig == SIGINT){
        syslog(LOG_INFO,"Received signal: %d", sig);
        daemon_handler = 1;
        return 0;
    }
}

int main(int argc, char **argv)
{
    arguments arguments;
    arguments.deviceId = NULL;
    arguments.orgId = NULL;
    arguments.token = NULL;
    arguments.typeId = NULL;

    struct sigaction action;
    action.sa_handler = sigHandler;    
    sigfillset(&action.sa_mask);    
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);   

    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    int rc = 0;
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;
    struct ubus_context *ctx;
    memory_data data;

    openlog("wiotpd", LOG_PID, LOG_DAEMON);

    rc = lock_file();
    if(rc != 0){
        printf("Program is already running");
        rc = EXIT_FAILURE;
        goto log_cleanup;
    }

    rc = init_ibm(&config, &device, &arguments.orgId, &arguments.typeId, &arguments.deviceId, &arguments.token);
    if(rc != 0){
        rc = EXIT_FAILURE;
        goto device_disc;
    }

    char mem_buf[512];

    while(!daemon_handler)
    {  
        rc = get_sys_info(ctx, &data);
        if(rc != 0){
            goto device_disc;
        }
        
        sprintf(mem_buf, "Total memory: %lld\n Free memory: %lld\n", data.total_memory, data.free_memory);

        rc = IoTPDevice_sendEvent(device,"status", mem_buf, "json", QoS0, NULL);
        if(rc != IOTPRC_SUCCESS){
            syslog(LOG_ERR, "Failed to send data");
            goto device_disc;
        }
        syslog(LOG_INFO, "Data sent succesfully\n");
        memset(mem_buf, 0, sizeof(mem_buf));
        sleep(10);
    }

    device_disc:
        rc = device_disconnect(device);

    config_cleanup:
        rc = clear_config(config);
    
    log_cleanup:
        closelog();

    program_exit:
        return rc;

return 0;
}
