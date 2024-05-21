// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef PATHS_H
#define PATHS_H

#include <QDir>
#include <QFile>
#include <QString>

/**
 * @brief Manage paths
 */
class Paths {
 public:
  Paths() = delete;

  /**
   * @brief Get directory for storing persistant application data
   * @return String with path to data dir
   */
  static QString dataDir();
};

#endif  // PATHS_H
