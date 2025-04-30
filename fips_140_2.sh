# Get bytes, pipe into hex to binary converter, pipe to rng test to ensure FIPS 140-2 compliance.
curl --insecure https://192.168.0.28:3006/?count=40000 | xxd -r -p | rngtest
