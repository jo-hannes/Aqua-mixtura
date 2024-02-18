// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "meta.h"

Meta::Meta(QString name) {
  this->name = name;
  create = QDateTime::currentDateTime();
  edit = QDateTime::currentDateTime();
}

QString Meta::getName() const {
  return name;
}

void Meta::setName(const QString& newName) {
  name = newName;
}

void Meta::edited() {
  edit = QDateTime::currentDateTime();
}

void Meta::fromJson(const QJsonObject& json) {
  name = json["Name"].toString("");
  create = QDateTime::fromString(json["Created"].toString(""), Qt::ISODate);
  edit = QDateTime::fromString(json["Edited"].toString(""), Qt::ISODate);
  if (!create.isValid()) {
    create = QDateTime::currentDateTime();
  }
  if (!edit.isValid()) {
    edit = QDateTime::currentDateTime();
  }
}

void Meta::toJson(QJsonObject& json) const {
  json["Name"] = name;
  json["Created"] = create.toString(Qt::ISODate);
  json["Edited"] = edit.toString(Qt::ISODate);
}
