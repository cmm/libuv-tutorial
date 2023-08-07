_co_define_uv_cancellable(shutdown, uv_shutdown_t *, req, int, status);
_co_define_uv(close, uv_handle_t *, handle);
_co_define_uv(connection, uv_stream_t *, server, int, status);
_co_define_uv_stoppable(read, uv_stream_t *, stream, ssize_t, nread,
                        const uv_buf_t *, buf);
_co_define_uv_cancellable(write, uv_write_t *, req, int, status);
_co_define_uv_cancellable(connect, uv_connect_t *, req, int, status);
_co_define_uv_cancellable(udp_send, uv_udp_send_t *, req, int, status);
_co_define_uv_stoppable(udp_recv, uv_udp_t *, handle, ssize_t, nread,
                        const uv_buf_t *, buf, const struct sockaddr *, addr,
                        unsigned, flags);
_co_define_uv_stoppable(prepare, uv_prepare_t *, prepare);
_co_define_uv_stoppable(check, uv_check_t *, check);
_co_define_uv_stoppable(idle, uv_idle_t *, idle);
_co_define_uv(async, uv_async_t *, handle);
_co_define_uv(timer, uv_timer_t *, handle);
_co_define_uv_cancellable(getaddrinfo, uv_getaddrinfo_t *, req, int, status,
                          struct addrinfo *, res);
_co_define_uv_cancellable(getnameinfo, uv_getnameinfo_t *, req, int, status,
                          const char *, hostname, const char *, service);
_co_define_uv(after_work, uv_work_t *, req, int, status);
_co_define_uv_cancellable(fs, uv_fs_t *, req);
_co_define_uv_cancellable(random, uv_random_t *, req, int, status, void *, buf,
                          size_t, buflen);
