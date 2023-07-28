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

co_define(do_stuff, co_none_t, co_none_t, uv_buf_t, NULL);
void do_stuff_co(co_t *co) {
  co_prologue(do_stuff, co, _, buf);
  while (true) {
    *buf = uv_buf_init(buffer, 1024);
    uv_await(ret, fs_read, &stdin_watcher, 0, buf, 1, -1);
    if (stdin_watcher.result <= 0) {
      fprintf(stderr, "Error opening file: %s\n", uv_err_name(ret->req->result));
      break;
    }
    buffer[stdin_watcher.result] = '\0';
    printf("Typed %s\n", buffer);
    uv_async_send(&as);
  }
  co_epilogue({});
}

co_define(idle, co_none_t, co_none_t, co_none_t, NULL);
void idle_co(co_t *co) {
  co_prologue(idle, co, _, __);
  while (true) {
    uv_await0(idle, &idler);
    crunch_away();
    uv_await0(async_init, &as);
  }
  co_epilogue({});
}

int main() {
  loop = uv_default_loop();

  uv_idle_init(loop, &idler);
  co_launch(loop, NULL, idle, {});
  co_launch(loop, NULL, do_stuff, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
