/*
This program is for test purpose only

*/
#include "spdk/stdinc.h"

#include "spdk/bdev.h"
#include "spdk/env.h"
#include "spdk/event.h"
#include "spdk/log.h"

struct discover_ctx_t {
  struct spdk_bdev *bdev;
  struct spdk_bdev_desc *bdev_desc;
  struct spdk_io_channel *bdev_io_channel;
  const char *bdev_name;
};

static void callback(enum spdk_bdev_event_type type, struct spdk_bdev *bdev,
                     void *event_ctx) {
  SPDK_NOTICELOG("Unsupported bdev event: type %d\n", type);
}

static void discover(void *arg1) {

  SPDK_NOTICELOG("Entering discover function\n");
  struct discover_ctx_t *discover_ctx = arg1;
  struct spdk_bdev *bdev;
  discover_ctx->bdev = NULL;
  discover_ctx->bdev_desc = NULL;
  int rc;

  bdev = spdk_bdev_first();

  if (bdev == NULL) {
    SPDK_ERRLOG("Ca bdev pas\n");
    spdk_app_stop(-1);
    return;
  }
  discover_ctx->bdev_name = spdk_bdev_get_name(bdev);

  rc = spdk_bdev_open_ext(discover_ctx->bdev_name, false, callback, NULL,
                          &discover_ctx->bdev_desc);
  if (rc) {
    SPDK_ERRLOG("Descriptor error : %d", rc);
    spdk_app_stop(-1);
    return;
  }

  discover_ctx->bdev = spdk_bdev_desc_get_bdev(discover_ctx->bdev_desc);
  discover_ctx->bdev_io_channel =
      spdk_bdev_get_io_channel(discover_ctx->bdev_desc);
  if (discover_ctx->bdev_io_channel == NULL) {
    SPDK_ERRLOG("No io channel for : %s", discover_ctx->bdev_name);
    spdk_bdev_close(discover_ctx->bdev_desc);
    spdk_app_stop(-1);
    return;
  }

  const char *name = spdk_bdev_get_name(discover_ctx->bdev);
  const char *product = spdk_bdev_get_product_name(discover_ctx->bdev);
  uint32_t bsize = spdk_bdev_get_block_size(discover_ctx->bdev);
  uint64_t iotime = spdk_bdev_get_io_time(discover_ctx->bdev);

  SPDK_NOTICELOG(
      "\n\nName : %s\nProduct : %s\nBlock size : %u bytes\nI/O time : %lu\n\n",
      name, product, bsize, iotime);

  SPDK_NOTICELOG("End of discover\n");

  spdk_put_io_channel(discover_ctx->bdev_io_channel);
  spdk_bdev_close(discover_ctx->bdev_desc);
  spdk_app_stop(0);
}

int main(int argc, char **argv) {
  struct spdk_app_opts opts = {};
  struct discover_ctx_t discover_ctx = {};
  int rc;

  spdk_log_set_print_level(SPDK_LOG_DEBUG);
  spdk_log_set_level(SPDK_LOG_DEBUG);

  spdk_app_opts_init(&opts, sizeof(opts));
  opts.name = "bdev_name";
  opts.json_config_file = "./simple-config.json";
  opts.env_context = "-a 0000:02:00.0";

  rc = spdk_app_start(&opts, discover, &discover_ctx);

  if (rc) {
    SPDK_ERRLOG("Error starting spdk (rc=%d)\n", rc);
  }

  SPDK_NOTICELOG("Successfuly finished !\n");
  spdk_app_fini();
  return rc;
}
