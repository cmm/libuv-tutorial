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
  uv_out_t uv;
} state_t;
co_define(foo, co_none_t, co_none_t, state_t);
void foo_co(co_t *co) {
  co_begin(foo, co, _, s);
  s->hints = (typeof(s->hints)){
    .ai_family = PF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP,
    .ai_flags = 0
  };
  fprintf(stderr, "irc.freenode.net is ");
  uv_await(&s->uv.getaddrinfo, getaddrinfo, &s->resolver, "irc.freenode.net", "6667", &s->hints);
  if (co_status) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(co_status));
    co_return({});
  }
  if (s->uv.getaddrinfo.status < 0) {
    fprintf(stderr, "getaddrinfo error %s\n", uv_err_name(s->uv.getaddrinfo.status));
    co_return({});
  }
  s->ai = s->uv.getaddrinfo.res;
  uv_ip4_name((struct sockaddr_in*)s->ai->ai_addr, s->addr, 16);
  fprintf(stderr, "%s\n", s->addr);
  uv_tcp_init(co->loop, &s->socket);
  uv_await(&s->uv.connect, tcp_connect, &s->connect_req, &s->socket, (const struct sockaddr*)s->ai->ai_addr);
  uv_freeaddrinfo(s->ai);
  if (co_status || s->uv.connect.status < 0) {
    fprintf(stderr, "connect error\n");
    co_return({});
  }
  static char msg[] = "hello";
  s->w_buf = (uv_buf_t){.len = strlen(msg), .base = msg};
  __auto_type tcp = (uv_stream_t *)s->uv.connect.req->handle;
  uv_await(&s->uv.write, write, &s->write_req, tcp, &s->w_buf, 1);
  if (co_status || s->uv.write.status < 0) {
    fprintf(stderr, "write error");
    co_return({});
  }
  s->stream = s->uv.write.req->handle;
  while (true) {
    uv_await(&s->uv.read, read, s->stream, (uv_buf_t){.len = sizeof(s->buffer) - 1, .base = s->buffer});
    if (co_status || s->uv.read.nread == UV_EOF) {
      printf("\n");
      break;
    }
    if (s->uv.read.nread < 0) {
      fprintf(stderr, "read error");
      co_return({});
    }
    s->uv.read.buf->base[s->uv.read.nread] = '\0';
    printf("%s", s->uv.read.buf->base);
  }

  uv_await(NULL, close, (uv_handle_t *)s->stream);
  co_end({});
}

int main() {
  loop = uv_default_loop();

  co_launch(loop, NULL, NULL, foo, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
