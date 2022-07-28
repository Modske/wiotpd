#include "system_info.h"

int rc = 0;

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	memory_data *data = (struct memory_data *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		syslog(LOG_ERR, "No memory data received\n");
		rc=-1;
		return;
	}

    blobmsg_parse(memory_policy, __MEMORY_MAX, memory,
	blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));

    data -> total_memory = blobmsg_get_u64(memory[TOTAL_MEMORY]);
    data -> free_memory = blobmsg_get_u64(memory[FREE_MEMORY]);

}

int get_sys_info(struct ubus_context *ctx, memory_data *data)
{	
    uint32_t id;

	ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus\n");
		return -1;
	}

	if (ubus_lookup_id(ctx, "system", &id) ||
	    ubus_invoke(ctx, id, "info", NULL, board_cb, data, 3000)) {
		syslog(LOG_ERR, "Cannot request memory info from procd\n");
		rc = -1;
	}

	ubus_free(ctx);

	return rc;
}