#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <syslog.h>

typedef struct memory_data{
    int64_t total_memory;
    int64_t free_memory;
} memory_data;

int get_sys_info(struct ubus_context *ctx, memory_data *data);