#!/bin/bash

DATA_DIR="data"
OUTPUT_DIR="output"

if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir "$OUTPUT_DIR"
fi

for i in {1..10}; do
    echo "hmac$i"
    INPUT_FILE="$DATA_DIR/hmac_$i.txt"
    OUTPUT_FILE="$OUTPUT_DIR/hmac_$i.txt"
    ./bin/hmac "$INPUT_FILE" "$OUTPUT_FILE"
done

for i in {1..10}; do
    echo "aes-gcm$i"
    INPUT_FILE="$DATA_DIR/aes_gcm_$i.txt"
    OUTPUT_FILE="$OUTPUT_DIR/aes_gcm_$i.txt"
    ./bin/aes-gcm "$INPUT_FILE" "$OUTPUT_FILE"
done

for i in {1..10}; do
    echo "hkdf$i"
    INPUT_FILE="$DATA_DIR/hkdf_$i.txt"
    OUTPUT_FILE="$OUTPUT_DIR/hkdf_$i.txt"
    ./bin/hkdf "$INPUT_FILE" "$OUTPUT_FILE"
done