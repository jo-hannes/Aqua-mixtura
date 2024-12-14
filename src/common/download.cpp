// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "download.h"

#include "paths.h"

#include <QEventLoop>
#include <QFile>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

bool Download::loadFile(QUrl url, QString dest) {
  QEventLoop loop;
  QNetworkAccessManager manager;
  QNetworkRequest request;

  request.setUrl(url);
  QNetworkReply* reply = manager.get(request);
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();
  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Failt to download from URL: " << url << " Error: " << reply->errorString();
    return false;
  }
  QFile file(dest);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed opening file: " << dest;
    return false;
  }
  file.write(reply->readAll());
  file.close();
  return true;
}

bool Download::loadDefaults(QString fileName) {
  const QString destFile = Paths::dataDir() + "/" + fileName;
  const QUrl srcUri("https://raw.githubusercontent.com/jo-hannes/Aqua-mixtura/refs/heads/main/defaults/" + fileName);
  if (!QFile::exists(destFile)) {
    return loadFile(srcUri, destFile);
  }
  return true;
}
