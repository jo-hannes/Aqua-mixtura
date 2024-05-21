// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "paths.h"

#include <QStandardPaths>

QString Paths::dataDir() {
  QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  if (dir.isEmpty()) {
    qFatal("Unable to get storage location");
  }
  return dir;
}
