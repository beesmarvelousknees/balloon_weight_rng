source config.cfg

openssl req -new -x509 -days 365 -nodes -newkey rsa:2048 -keyout server.key -out server.crt -subj "/C=XX/ST=DefaultState/L=DefaultCity/O=DefaultOrg/OU=DefaultUnit/CN=$IP_ADDRESS" -addext "subjectAltName = IP:$IP_ADDRESS"
