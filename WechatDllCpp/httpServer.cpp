/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include "WechatDllCpp.h"

static const char *s_http_port = "2019";
static struct mg_serve_http_opts s_http_server_opts;
void httpServer();


wchar_t * UTF8ToUnicode(const char* str)
{
	int    textlen = 0;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

static void send(struct mg_connection *nc, struct http_message *hm) {
	wchar_t wxid[0x100], msg[0x200];

	char buf_wxid[0x100], buf_msg[0x200];

	/* Get form variables */
	mg_get_http_var(&hm->body, "wxid", buf_wxid, sizeof(buf_wxid));
	mg_get_http_var(&hm->body, "msg", buf_msg, sizeof(buf_msg));

	wcscpy_s(wxid, UTF8ToUnicode(buf_wxid));
	wcscpy_s(msg, UTF8ToUnicode(buf_msg));

	SendTextMessage(wxid, msg);

	/* Send headers */
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	/* Compute the result and send it back as a JSON object */
	//result = strtod(n1, NULL) + strtod(n2, NULL);
	mg_printf_http_chunk(nc, "{ \"wxid\": %s, \"msg\": %s}", buf_wxid, buf_msg);
	mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) {
	case MG_EV_HTTP_REQUEST:
		if (mg_vcmp(&hm->uri, "/send") == 0) {
			send(nc, hm); /* Handle RESTful call */
		}
		else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
			char buf[100] = { 0 };
			memcpy(buf, hm->body.p,
				sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
			printf("%s\n", buf);
		}
		else {
			mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
		}
		break;
	default:
		break;
	}
}


void httpServer()
{
	struct mg_mgr mgr;
	struct mg_connection *nc;
	struct mg_bind_opts bind_opts;
	int i;
	char *cp;
	const char *err_str;
#if MG_ENABLE_SSL
	const char *ssl_cert = NULL;
#endif

	mg_mgr_init(&mgr, NULL);


	/* Set HTTP server options */
	memset(&bind_opts, 0, sizeof(bind_opts));
	bind_opts.error_string = &err_str;
#if MG_ENABLE_SSL
	if (ssl_cert != NULL) {
		bind_opts.ssl_cert = ssl_cert;
	}
#endif
	nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
	if (nc == NULL) {
		fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
			*bind_opts.error_string);
		exit(1);
	}

	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.enable_directory_listing = "yes";

	printf("Starting RESTful server on port %s, serving %s\n", s_http_port,
		s_http_server_opts.document_root);
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

}