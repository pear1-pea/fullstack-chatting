#!/bin/bash
set -e

PROTOC=~/vcpkg/installed/arm64-osx/tools/protobuf/protoc
GRPC_PLUGIN=~/vcpkg/installed/arm64-osx/tools/grpc/grpc_cpp_plugin

PROTO_DIR=./proto

OUT_DIR=./generated

mkdir -p "$OUT_DIR"

$PROTOC \
    --proto_path="$PROTO_DIR" \
    --cpp_out="$OUT_DIR" \
    --grpc_out="$OUT_DIR" \
    --plugin=protoc-gen-grpc="$GRPC_PLUGIN" \
    "$PROTO_DIR/message.proto"

echo "Proto files generated in: $OUT_DIR"
