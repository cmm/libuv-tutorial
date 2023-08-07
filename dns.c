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
    uv_getaddrinfo_out_t getaddrinfo;
    uv_connect_out_t connect;
    uv_write_out_t write;
    uv_read_out_t read;
  } ret;
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
  uv_await(&s->ret.getaddrinfo, getaddrinfo, &s->resolver, "irc.freenode.net", "6667", &s->hints);
  if (co_status) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(co_status));
    co_return(foo, {});
  }
  if (s->ret.getaddrinfo.status < 0) {
    fprintf(stderr, "getaddrinfo error %s\n", uv_err_name(s->ret.getaddrinfo.status));
    co_return(foo, {});
  }
  s->ai = s->ret.getaddrinfo.res;
  uv_ip4_name((struct sockaddr_in*)s->ai->ai_addr, s->addr, 16);
  fprintf(stderr, "%s\n", s->addr);
  uv_tcp_init(co->loop, &s->socket);
  uv_await(&s->ret.connect, tcp_connect, &s->connect_req, &s->socket, (const struct sockaddr*)s->ai->ai_addr);
  uv_freeaddrinfo(s->ai);
  if (co_status || s->ret.connect.status < 0) {
    fprintf(stderr, "connect error\n");
    co_return(foo, {});
  }
  static char msg[] = "hello";
  s->w_buf = (uv_buf_t){.len = strlen(msg), .base = msg};
  __auto_type tcp = (uv_stream_t *)s->ret.connect.req->handle;
  uv_await(&s->ret.write, write, &s->write_req, tcp, &s->w_buf, 1);
  if (co_status || s->ret.write.status < 0) {
    fprintf(stderr, "write error");
    co_return(foo, {});
  }
  s->stream = s->ret.write.req->handle;
  while (true) {
    uv_await(&s->ret.read, read, s->stream, (uv_buf_t){.len = sizeof(s->buffer) - 1, .base = s->buffer});
    if (co_status || s->ret.read.nread == UV_EOF) {
      printf("\n");
      break;
    }
    if (s->ret.read.nread < 0) {
      fprintf(stderr, "read error");
      co_return(foo, {});
    }
    s->ret.read.buf->base[s->ret.read.nread] = '\0';
    printf("%s", s->ret.read.buf->base);
  }

  uv_await(NULL, close, (uv_handle_t *)s->stream);
  co_end(foo, {});
}

int main() {
  loop = uv_default_loop();

  co_launch(loop, NULL, NULL, foo, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
