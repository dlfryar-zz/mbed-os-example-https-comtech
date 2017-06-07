#include "mbed.h"
#include "easy-connect.h"
#include "https_request.h"
#include "main_ssl_ca_pem.h"

Serial pc(USBTX, USBRX);
const char *API_ENDPOINT = "apis.location.studio";
const char *PROTOCOL = "https://";
const char *API_KEY = "YOUR_LOCATION_STUDIO_API_KEY_HERE"; // https://developer.location.studio/#/applications

// move this in to the function or some structure
const char *API_RESOURCE = "/geo/v2/search/json";

void dump_response(HttpResponse* res) {
    mbedtls_printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

    mbedtls_printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        mbedtls_printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    mbedtls_printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

int main() {
    pc.baud(115200);

    NetworkInterface* network = easy_connect(true);
    if (!network) {
        return 1;
    }

    // Create a TLS socket (which holds a TCPSocket)
    printf("\n----- Setting up TLS connection -----\n");

    TLSSocket* socket = new TLSSocket(network, API_ENDPOINT, 443, SSL_CA_PEM);
    socket->set_debug(true);
    if (socket->connect() != 0) {
        printf("TLS Connect failed %d\n", socket->error());
        return 1;
    }

    // GET request to apis.location.studio
    {
        const char * LAT_LONG = "30.2447,-97.8443783";
        int STR_BUF_SZ = 256;
        char strbuf[STR_BUF_SZ];
        int cx, cz; // character copy length

        cx = snprintf(strbuf, STR_BUF_SZ, "%s%s%s/?query=food&api_key=%s&search_center=%s", PROTOCOL, API_ENDPOINT, API_RESOURCE, API_KEY, LAT_LONG);

        // Do an initial copy to see how long the URI really is
        if (cx >= 0 && cx < STR_BUF_SZ) {
            char URI[cx];
            cz = snprintf(URI, cx, strbuf);  // build a URI
            printf("(%d) %s\n", cz, URI);

            HttpsRequest *get_req = new HttpsRequest(socket, HTTP_GET, URI); // Build a request with our URI
            get_req->set_debug(true);

            HttpResponse *get_res = get_req->send(); // Call the Comtech API to get the JSON data
            if (!get_res) {
                printf("HttpRequest failed (error code %d)\n", get_req->get_error());
                return 1;
            }

            printf("\n----- HTTPS GET response -----\n");
            dump_response(get_res);

            delete get_req;
        }
    }

    delete socket;
    Thread::wait(osWaitForever);
}
