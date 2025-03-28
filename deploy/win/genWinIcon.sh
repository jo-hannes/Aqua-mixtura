#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

magick ../../src/icons/logo_512x512.png -define icon:auto-resize=256,128,48,32,16 icon.ico
