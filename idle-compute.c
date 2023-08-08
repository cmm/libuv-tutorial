#include <stdio.h>
#include <uv.h>
#include "co.h"

uv_loop_t *loop;
uv_fs_t stdin_watcher;
uv_idle_t idler;
char buffer[1024];
uv_async_t as;

void crunch_away() {
  fprintf(stderr, "Computing PI...\n");
}

typedef struct {
  uv_buf_t buf;
  uv_out_t uv;
} stuff_state_t;
co_define(do_stuff, co_none_t, co_none_t, stuff_state_t);
void do_stuff_co(co_t *co) {
  co_begin(do_stuff, co, _, s);
  while (true) {
    s->buf = uv_buf_init(buffer, 1024);
    uv_await(&s->uv.fs, fs_read, &stdin_watcher, 0, &s->buf, 1, -1);
    if (stdin_watcher.result <= 0) {
      fprintf(stderr, "Error opening file: %s\n", uv_err_name(s->uv.fs.req->result));
      break;
    }
    buffer[stdin_watcher.result] = '\0';
    printf("Typed %s\n", buffer);
    uv_async_send(&as);
  }
  co_end({});
}

co_define(idle, co_none_t, co_none_t, co_none_t);
void idle_co(co_t *co) {
  co_begin(idle, co, _, __);
  while (true) {
    uv_await(NULL, idle, &idler);
    crunch_away();
    uv_await(NULL, async_init, &as);
  }
  co_end({});
}

int main() {
  loop = uv_default_loop();

  uv_idle_init(loop, &idler);
  co_launch(loop, NULL, NULL, idle, {});
  co_launch(loop, NULL, NULL, do_stuff, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
