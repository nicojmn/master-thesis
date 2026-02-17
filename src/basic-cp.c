/*
This program is for test purpose only

*/
#include "spdk/stdinc.h"

#include "spdk/bdev.h"
#include "spdk/bdev_zone.h"
#include "spdk/env.h"
#include "spdk/event.h"
#include "spdk/log.h"
#include "spdk/thread.h"
#include <asm-generic/errno-base.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct context_t {
  struct spdk_bdev *bdev;
  struct spdk_bdev_desc *bdev_desc;
  struct spdk_io_channel *bdev_io_channel;
  struct spdk_bdev_io_wait_entry bdev_io_wait;
  const char *bdev_name;
  char *buf;
  uint32_t buf_size;
};

struct g_opts_t {
  char *input_path;
  char *output_path;
  struct stat fstat;
  int fd;
};

static struct g_opts_t g_opts = {.input_path = NULL, .output_path = NULL};

static void callback(enum spdk_bdev_event_type type, struct spdk_bdev *bdev,
                     void *event_ctx) {
  SPDK_NOTICELOG("Unsupported bdev event: type %d\n", type);
}

static void read_complete(struct spdk_bdev_io *bdev_io, bool success,
                          void *cb_arg) {
  struct context_t *ctx = cb_arg;

  if (success) {
    if (g_opts.output_path != NULL) { // only print content if not -o set
      SPDK_NOTICELOG("I/O read from bdev : %s\n", ctx->buf);
    }
  } else {
    SPDK_ERRLOG("I/O read bdev error\n");
  }

  if (g_opts.output_path != NULL) {
    int fd = open(g_opts.output_path, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
      SPDK_ERRLOG("Failed to open %s : %s\n", g_opts.output_path,
                  strerror(errno));
    } else {
      uint32_t current_written = 0;
      while (current_written < ctx->buf_size) {
        int rc = write(fd, ctx->buf + current_written,
                       ctx->buf_size - current_written);
        if (rc < 0) {
          SPDK_ERRLOG("Error writing to %s : %s\n", g_opts.output_path,
                      strerror(errno));
          close(fd);
          break;
        }
        current_written += rc;
      }
      close(fd);
    }
  }

  uint64_t iotime = spdk_bdev_get_io_time(ctx->bdev);
  SPDK_NOTICELOG("Final I/O time : %lu", iotime);

  spdk_bdev_free_io(bdev_io);
  spdk_put_io_channel(ctx->bdev_io_channel);
  spdk_bdev_close(ctx->bdev_desc);
  SPDK_NOTICELOG("Stopping app\n");
  spdk_app_stop(success ? 0 : -1);
}

static void read_file(void *arg) {
  struct context_t *ctx = arg;
  int rc = 0;

  SPDK_NOTICELOG("Reading I/O\n");
  rc = spdk_bdev_read(ctx->bdev_desc, ctx->bdev_io_channel, ctx->buf, 0,
                      ctx->buf_size, read_complete, ctx);

  if (rc == -ENOMEM) {
    SPDK_NOTICELOG("Queuing I/O");
    ctx->bdev_io_wait.bdev = ctx->bdev;
    ctx->bdev_io_wait.cb_fn = read_file;
    ctx->bdev_io_wait.cb_arg = ctx;
    spdk_bdev_queue_io_wait(ctx->bdev, ctx->bdev_io_channel,
                            &ctx->bdev_io_wait);
  } else if (rc) {
    SPDK_ERRLOG("%s error while reading from bdev : %d\n", spdk_strerror(-rc),
                rc);
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
  }
}

static void write_complete(struct spdk_bdev_io *bdev_io, bool success,
                           void *cb_arg) {
  struct context_t *context = cb_arg;

  spdk_bdev_free_io(bdev_io);

  if (success) {
    SPDK_NOTICELOG("I/O write completed successfully\n");
  } else {
    SPDK_ERRLOG("I/O write error : %d\n", EIO);
    spdk_put_io_channel(context->bdev_io_channel);
    spdk_bdev_close(context->bdev_desc);
    spdk_app_stop(-1);
    return;
  }

  memset(context->buf, 0, context->buf_size);
  SPDK_NOTICELOG("Content written (%dB) : %s\n", context->buf_size,
                 context->buf);
  SPDK_NOTICELOG("Resetting content of buffer\n");

  read_file(context);
}
static void write_file(void *arg) {
  struct context_t *ctx = arg;
  int rc = 0;

  size_t fsize = g_opts.fstat.st_size;
  uint32_t block_size = spdk_bdev_get_block_size(ctx->bdev);
  uint64_t num_blocks = (fsize + (block_size - 1)) / block_size;

  SPDK_NOTICELOG("DMA alloc for write\n");
  ctx->buf_size =
      (num_blocks * block_size) * spdk_bdev_get_write_unit_size(ctx->bdev);
  SPDK_NOTICELOG("fsize : %lu\nbuf_size : %d\n", fsize, ctx->buf_size);
  uint32_t buf_align = spdk_bdev_get_buf_align(ctx->bdev);
  SPDK_NOTICELOG("Buf align : %d\n", buf_align);
  ctx->buf = spdk_dma_zmalloc(ctx->buf_size, buf_align, NULL);

  if (!ctx->buf) {
    SPDK_ERRLOG("DMA alloc failed :(\n");
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
  }

  int r = read(g_opts.fd, ctx->buf, fsize);
  if (r == -1) {
    SPDK_ERRLOG("Failed to copy file in DMA buffer");
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
  }
  SPDK_NOTICELOG("I/O : writing to bdev : %s\n", ctx->bdev_name);
  rc = spdk_bdev_write(ctx->bdev_desc, ctx->bdev_io_channel, ctx->buf, 0,
                       ctx->buf_size, write_complete, ctx);
  if (rc == -ENOMEM) {
    SPDK_NOTICELOG("Queuing I/O\n");
    ctx->bdev_io_wait.bdev = ctx->bdev;
    ctx->bdev_io_wait.cb_fn = write_file;
    ctx->bdev_io_wait.cb_arg = ctx;
    spdk_bdev_queue_io_wait(ctx->bdev, ctx->bdev_io_channel,
                            &ctx->bdev_io_wait);
  } else if (rc) {
    SPDK_ERRLOG("%d error while writing to bdev: %s\n", spdk_strerror(-rc), rc);
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
  }
}

static void reset_zone_complete(struct spdk_bdev_io *bdev_io, bool success,
                                void *cb_arg) {
  struct context_t *ctx = cb_arg;

  spdk_bdev_free_io(bdev_io);

  if (!success) {
    SPDK_ERRLOG("bdev reset error : %d\n", EIO);
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
    return;
  }

  write_file(ctx);
}

static void reset_zone(void *arg) {
  struct context_t *ctx = arg;
  int rc = 0;

  rc =
      spdk_bdev_zone_management(ctx->bdev_desc, ctx->bdev_io_channel, 0,
                                SPDK_BDEV_ZONE_RESET, reset_zone_complete, ctx);
  if (rc == -ENOMEM) {
    ctx->bdev_io_wait.bdev = ctx->bdev;
    ctx->bdev_io_wait.cb_fn = reset_zone;
    ctx->bdev_io_wait.cb_arg = ctx;
    spdk_bdev_queue_io_wait(ctx->bdev, ctx->bdev_io_channel,
                            &ctx->bdev_io_wait);
  } else if (rc) {
    SPDK_ERRLOG("%s error while resetting zone: %d\n", spdk_strerror(-rc), rc);
    spdk_put_io_channel(ctx->bdev_io_channel);
    spdk_bdev_close(ctx->bdev_desc);
    spdk_app_stop(-1);
  }
}
static void discover(void *arg1) {

  SPDK_NOTICELOG("Entering discover function\n");
  struct context_t *discover_ctx = arg1;
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

  rc = spdk_bdev_open_ext(discover_ctx->bdev_name, true, callback, NULL,
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
  if (spdk_bdev_is_zoned(discover_ctx->bdev)) {
    reset_zone(discover_ctx);
    return;
  }
  write_file(discover_ctx);
}

static void usage(void) {
  printf("Usage : \n");
  printf("\t-f <path> : source file path\n");
  printf("\t-o <path> : output destination path\n");
}

static int parse_arg(int ch, char *argv) {
  switch (ch) {
  case 'f':
    g_opts.input_path = strdup(argv);
    break;
  case 'o':
    g_opts.output_path = strdup(argv);
    break;
  default:
    usage();
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  struct spdk_app_opts opts = {};
  struct context_t context = {};
  int rc;

  spdk_log_set_print_level(SPDK_LOG_DEBUG);
  spdk_log_set_level(SPDK_LOG_DEBUG);

  spdk_app_opts_init(&opts, sizeof(opts));
  opts.name = "bdev_name";
  opts.json_config_file = "./simple-config.json";
  opts.env_context = "-a 0000:02:00.0";

  rc = spdk_app_parse_args(argc, argv, &opts, "f:o:", NULL, parse_arg, usage);
  if (rc == SPDK_APP_PARSE_ARGS_FAIL) {
    SPDK_ERRLOG("Error parsing args (invalid args) : %d\n", rc);
    exit(rc);
  }

  if (g_opts.input_path == NULL) {
    SPDK_ERRLOG("Missing args\n");
    usage();
    rc = EINVAL;
    exit(rc);
  }
  g_opts.fd = open(g_opts.input_path, O_RDONLY);
  if (g_opts.fd == -1) {
    SPDK_ERRLOG("Failed to open %s\n", g_opts.input_path);
    exit(-1);
  }

  if (fstat(g_opts.fd, &g_opts.fstat) == -1) {
    SPDK_ERRLOG("Failed to read file stat of : %s\n", g_opts.input_path);
    exit(-1);
  }

  SPDK_NOTICELOG("Source file : %s\n", g_opts.input_path);

  rc = spdk_app_start(&opts, discover, &context);

  if (rc) {
    SPDK_ERRLOG("Error starting spdk (rc=%d)\n", rc);
  }

  spdk_dma_free(context.buf);
  spdk_app_fini();
  return rc;
}
