#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

if [ $# -ne 2 ]; then
  echo "Illegal number of parameters"
  exit 1
fi
if [ ! -d "${1}" ]; then
  echo "Source is not a directory"
  exit 1
fi
if [ ! -d "${2}" ]; then
  echo "Destination is not a directory"
  exit 1
fi

# make temporary target dir
TMPDIR="${2}/icon.iconset/"
mkdir -p "${TMPDIR}"

# 1024px
cp "${1}/logo_1024x1024.png" "${TMPDIR}/icon_512x512@2x.png"
# 512px images
cp "${1}/logo_512x512.png" "${TMPDIR}/icon_512x512.png"
cp "${1}/logo_512x512.png" "${TMPDIR}/icon_256x256@2x.png"
# 256px
sips -Z 256 "${1}/logo_512x512.png" --out "${TMPDIR}/icon_256x256.png"
cp "${TMPDIR}/icon_256x256.png" "${TMPDIR}/icon_128x128@2x.png"
# 128px
sips -Z 128 "${1}/logo_512x512.png" --out "${TMPDIR}/icon_128x128.png"
# 64px
sips -Z 64 "${1}/logo_512x512.png" --out "${TMPDIR}/icon_32x32@2x.png"
# 32px
sips -Z 32 "${1}/logo_512x512.png" --out "${TMPDIR}/icon_32x32.png"
cp "${TMPDIR}/icon_256x256.png" "${TMPDIR}/icon_16x16@2x.png"
# 16px
sips -Z 16 "${1}/logo_512x512.png" --out "${TMPDIR}/icon_16x16.png"
# build iconset
iconutil -c icns "${TMPDIR}" -o "${2}/icon.icns"
