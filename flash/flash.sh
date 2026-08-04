#!/usr/bin/env bash
#
# Flash Bao and its two guests to an RH850/U2A board over a Renesas E2 emulator.
#
#   ./flash.sh              flash the guests, then Bao, then release reset
#   ./flash.sh bao          flash only Bao (guests already in flash)
#   ./flash.sh guests       flash only the two guests
#   ./flash.sh reset        just release the reset signal
#
# Flash layout, matching configs/autosar-vm0+vm1.c:
#   0x000000  bao.bin                       hypervisor
#   0x010000  vm0-autosar-0x10000.hex       AUTOSAR guest, drives CAN (RSCFD0)
#   0x7F0000  vm1-mcal-0x7F0000.hex         MCAL guest, drives the RLIN35 console
#
# Build Bao first:
#   make PLATFORM=rh850-u2a-vm1+vm2 CONFIG=autosar-vm0+vm1
#
# Console: 500000 8N1 on the CH340 adapter wired to RLIN35, NOT the E2's own port.
#   picocom -b 500000 /dev/ttyACM0
#
# rfp-cli lives in the bao-benchmarks-portable bundle, which is not tracked here because of
# its size. Point RFP_DIR at it if yours is elsewhere. If rfp-cli reports
# "E3000201: Cannot find the specified tool", the E2 udev rule is missing:
#   cd <bao-benchmarks-portable> && ./install-udev-rule.sh   # then replug the E2
set -euo pipefail

DEVICE="RH850/U2x"
TOOL="e2"
OSC_MHZ="40.0"
RFP_PASS="ffff"

PLATFORM="${PLATFORM:-rh850-u2a-vm1+vm2}"
CONFIG="${CONFIG:-autosar-vm0+vm1}"
RFP_DIR="${RFP_DIR:-/home/leo/osyx/rh850/bao-benchmarks-portable/Tools/linux-x64}"

HERE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "${HERE}/.." && pwd)"
BAO_BIN="${ROOT}/bin/${PLATFORM}/${CONFIG}/bao.bin"
export PATH="${RFP_DIR}:${PATH}"

command -v rfp-cli >/dev/null || { echo "rfp-cli not on PATH; set RFP_DIR" >&2; exit 1; }

rfp() { echo "${RFP_PASS}" | rfp-cli -device "${DEVICE}" -tool "${TOOL}" -osc "${OSC_MHZ}" "$@"; }

flash_guests() {
    echo "== VM0 (AUTOSAR/CAN) -> 0x10000 =="
    rfp -program -file "${HERE}/guests/vm0-autosar-0x10000.hex"
    echo "== VM1 (MCAL/console) -> 0x7F0000 =="
    rfp -program -file "${HERE}/guests/vm1-mcal-0x7F0000.hex"
}

flash_bao() {
    [[ -s "${BAO_BIN}" ]] || { echo "not built: ${BAO_BIN}" >&2; exit 1; }
    echo "== Bao -> 0x0, then release reset =="
    rfp -program -bin 0x0 "${BAO_BIN}" -run
}

case "${1:-all}" in
    all)    flash_guests; flash_bao ;;
    bao)    flash_bao ;;
    guests) flash_guests ;;
    reset)  rfp -reset -run ;;
    *)      echo "usage: $0 [all|bao|guests|reset]" >&2; exit 2 ;;
esac
