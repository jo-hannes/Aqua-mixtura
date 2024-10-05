// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QString>
#include <QUrl>

#include <QtNetwork/QNetworkAccessManager>

/**
 * @brief Class to download files
 */
class Download {
 public:
  Download() = delete;

  /**
   * @brief Download a file from url and store it at dest
   * @param url Url to download from
   * @param dest Path name where to store download
   * @return Returns true on success
   */
  static bool loadFile(QUrl url, QString dest);

  /**
   * @brief Download defaults if file does not already exist
   * @param fileName Name of file only (without path!)
   * @return Returns true on success
   */
  static bool loadDefaults(QString fileName);
};

#endif  // DOWNLOAD_H
