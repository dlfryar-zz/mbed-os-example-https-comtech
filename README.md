# mbed-os-example-https-comtech

This application demonstrates how to make an HTTPS request to Comtech's location API and parse the response from mbed OS 5.

### Based on source

    https://github.com/ARMmbed/mbed-os-example-http

## Tested on

* K64F with Ethernet.

Do a location search for food places around coordinates (30.2447,-97.8443783) using https://apis.location.studio/geo/v2/search/json

Return the data, but don't iterate through all the results making additional calls.

## Certificates

There is a script called cert.py to help generate the main_ssl_ca_pem.h file with server certificates.  Create a file called cert.txt by using the openssl command to fetch the server certificates.

echo | openssl s_client -connect apis.location.studio:443 -showcerts 2> /dev/null > cert.txt

## API_KEY

Find main.cpp and enter your Comtech Location Studio API_KEY retrieved from https://developer.location.studio/#/applications
