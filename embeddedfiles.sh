#!/bin/bash

readonly ICUDT67L_PATH="ultralight-sdk/resources/icudt67l.dat"
readonly CACERT_PATH="ultralight-sdk/resources/cacert.pem"
readonly OUTPUT_PATH="wrapper/embeddedfiles.cpp"

printf \
'#include "embeddedfiles.h"

const uint8_t icudt67l_data[] = {
%s
};
size_t icudt67l_size = sizeof(icudt67l_data);
const uint8_t cacert_data[] = {
%s
};
size_t cacert_size = sizeof(cacert_data);
' "$(xxd -i < "$ICUDT67L_PATH")" "$(xxd -i < "$CACERT_PATH")" > "$OUTPUT_PATH"
