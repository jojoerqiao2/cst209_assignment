#!/usr/bin/env bash

set -e

# Cleanup temporary directory and associated files when exiting the script.
cleanup() {
    EXIT_CODE=$?
    set +e
    if [[ -n "${TMP_DIR}" ]]; then
        echo "Executing cleanup of tmp files"
        rm -Rf "${TMP_DIR}"
    fi
    exit $EXIT_CODE
}
trap cleanup EXIT

echo "Installing Clang..."
TMP_DIR=$(mktemp -d -t cmake-XXXXXXXXXX)

echo "${TMP_DIR}"
cd "${TMP_DIR}"

apt-get update -y
apt install -y lsb-release wget software-properties-common gnupg 
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
apt purge -y --auto-remove clang llvm lldb
apt-add-repository -y "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs) main"
apt-get -y -f install clang-format clang-tidy clang-tools clang clangd libc++-dev libc++1 libc++abi-dev libc++abi1 libclang-dev libclang1 libllvm-ocaml-dev libomp-dev libomp5 lld llvm-dev llvm-runtime llvm lldb python3-clang
