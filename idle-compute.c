#include <stdio.h>
#include <uv.h>
#include "co.h"

uv_loop_t *loop;
uv_fs_t stdin_watcher;
/* uv_idle_t idler; */
#define BUF_SIZE 4
char buffer[BUF_SIZE];

/* void crunch_away(uv_idle_t *handle) { */
/*     fprintf(stderr, "Computing PI...\n"); */
/*     uv_idle_stop(handle); */
/* } */

/* void on_type(uv_fs_t *req) { */
/*     if (stdin_watcher.result > 0) { */
/*         buffer[stdin_watcher.result] = '\0'; */
/*         printf("Typed %s\n", buffer); */

/*         uv_buf_t buf = uv_buf_init(buffer, 1024); */
/*         uv_fs_read(loop, &stdin_watcher, 0, &buf, 1, -1, on_type); */
/*         uv_idle_start(&idler, crunch_away); */
/*     } else if(stdin_watcher.result < 0) { */
/*         fprintf(stderr, "Error opening file: %s\n", uv_err_name(req->result)); */
/*     } */
/* } */

typedef struct {} read_state_t;
co_define(read, int, int, read_state_t, NULL);
void read_co(co_base_t *co) {
  co_prologue(read, co);
  while (true) {
    uv_buf_t buf = uv_buf_init(buffer, BUF_SIZE);
    uv_await(ret, fs_read, &stdin_watcher, 0, &buf, 1, -1);
    if (co_errno) {
      fprintf(stderr, "whoops: %s\n", uv_err_name(co_errno));
      co_return(co_errno);
    }
    if (stdin_watcher.result > 0) {
      buffer[stdin_watcher.result] = '\0';
      printf("Typed %s[%lu]\n", buffer, stdin_watcher.result);
    } else {
      fprintf(stderr, "oops: %s\n", uv_err_name(ret->req->result));
    }
  };
  co_epilogue(0);
}

int main() {
  loop = uv_default_loop();

  //uv_idle_init(loop, &idler);
  co_launch(loop, NULL, read, 0);
  //uv_idle_start(&idler, crunch_away);
  return uv_run(loop, UV_RUN_DEFAULT);
}
