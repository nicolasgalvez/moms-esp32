#!/bin/bash

OUTPUT_DIR="include"

# Ensure the output directory exists
mkdir -p $OUTPUT_DIR

# Get the certificate chain from the website
echo -n | openssl s_client -showcerts -servername $1 -connect $1:443 > fullchain.pem

# Extract the highest-level certificate
awk '/BEGIN CERTIFICATE/{i++} i==2' fullchain.pem > root_ca.pem

# Check if the certificate works
echo -n | openssl s_client -connect $1:443 -CAfile root_ca.pem > /dev/null
if [ $? -ne 0 ]; then
    echo "The certificate does not work."
    exit 1
fi

# Convert the certificate file to a header file
echo "const char root_ca_pem[] = R\"(" > $OUTPUT_DIR/root_ca.h
cat root_ca.pem >> $OUTPUT_DIR/root_ca.h
echo ")\";" >> $OUTPUT_DIR/root_ca.h

# Remove the pem files
rm fullchain.pem root_ca.pem