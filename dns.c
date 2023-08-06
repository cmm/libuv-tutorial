#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <uv.h>
#include "co.h"

uv_loop_t *loop;

typedef struct {
  struct addrinfo hints;
  uv_getaddrinfo_t resolver;
  struct addrinfo *ai;
  char addr[17];
  uv_connect_t connect_req;
  uv_tcp_t socket;
  char buffer[32];
  uv_buf_t w_buf;
  uv_write_t write_req;
  uv_stream_t *stream;
  union {
    uv_getaddrinfo_promise_t getaddrinfo;
    uv_connect_promise_t connect;
    uv_write_promise_t write;
    uv_read_promise_t read;
    uv_close_promise_t close;
  } ret;
} state_t;
co_define(foo, co_none_t, co_none_t, state_t);
void foo_co(co_t *co) {
  co_begin(foo, co, _, state);
  state->hints = (typeof(state->hints)){
    .ai_family = PF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP,
    .ai_flags = 0
  };
  fprintf(stderr, "irc.freenode.net is ");
  uv_await(&state->ret.getaddrinfo, getaddrinfo, &state->resolver, "irc.freenode.net", "6667", &state->hints);
  if (co_status) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(co_status));
    co_return({});
  }
  if (state->ret.getaddrinfo.out.status < 0) {
    fprintf(stderr, "getaddrinfo error %s\n", uv_err_name(state->ret.getaddrinfo.out.status));
    co_return({});
  }
  state->ai = state->ret.getaddrinfo.out.res;
  uv_ip4_name((struct sockaddr_in*)state->ai->ai_addr, state->addr, 16);
  fprintf(stderr, "%s\n", state->addr);
  uv_tcp_init(co->loop, &state->socket);
  uv_await(&state->ret.connect, tcp_connect, &state->connect_req, &state->socket, (const struct sockaddr*)state->ai->ai_addr);
  uv_freeaddrinfo(state->ai);
  if (state->ret.connect.out.status < 0) {
    fprintf(stderr, "connect error\n");
    co_return({});
  }
  static char msg[] = "hello";
  state->w_buf = (uv_buf_t){.len = strlen(msg), .base = msg};
  __auto_type tcp = (uv_stream_t *)state->ret.connect.out.req->handle;
  uv_await(&state->ret.write, write, &state->write_req, tcp, &state->w_buf, 1);
  if (state->ret.write.out.status < 0) {
    fprintf(stderr, "write error");
    co_return({});
  }
  state->stream = state->ret.write.out.req->handle;
  while (true) {
    uv_await(&state->ret.read, read, state->stream, (uv_buf_t){.len = sizeof(state->buffer) - 1, .base = state->buffer});
    if (state->ret.read.out.nread == UV_EOF) {
      printf("\n");
      break;
    }
    if (state->ret.read.out.nread < 0) {
      fprintf(stderr, "read error");
      co_return({});
    }
    state->ret.read.out.buf->base[state->ret.read.out.nread] = '\0';
    printf("%s", state->ret.read.out.buf->base);
  }

  uv_await(&state->ret.close, close, (uv_handle_t *)state->stream);
  co_end({});
}

int main() {
  loop = uv_default_loop();

  co_launch(loop, NULL, foo, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
