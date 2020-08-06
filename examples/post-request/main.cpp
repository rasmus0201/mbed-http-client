#include "mbedtls_entropy_config.h"
#include "mbed.h"
#include "http_request.h"
#include "network-helper.h"
#include "mbed_mem_trace.h"

void dump_response(HttpResponse* res) {
    printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

    printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

void bodyResponse(const char *at, uint32_t length) {
    char str[length];
    strncpy(str, at, length);
   
    printf("Body:\n%s", str);
}

int main() {
    // Connect to the network with the default networking interface
    // if you use WiFi: see mbed_app.json for the credentials
    NetworkInterface* network = connect_to_default_network_interface();
    if (!network) {
        printf("Cannot connect to the network, see serial output\n");
        return 1;
    }

    HttpRequest* post_req = new HttpRequest(network, HTTP_POST, "http://httpbin.org/post");
    post_req->set_header("Content-Type", "application/json");

    const char body[] = "{\"hello\":\"world\"}";

    HttpResponse* post_res = post_req->send(body, strlen(body));
    if (!post_res) {
        printf("HttpRequest failed (error code %d)\n", post_req->get_error());
        return 1;
    }

    printf("\n----- HTTP POST response -----\n");
    dump_response(post_res);

    delete post_req;

    return 0;
}
