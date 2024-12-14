// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "meta.h"

#include <utility>

Meta::Meta(QString name) : name{std::move(name)} {
  create = QDateTime::currentDateTime();
  edit = QDateTime::currentDateTime();
  uuid = QUuid::createUuid();
}

Meta::Meta(const QJsonObject& json) {
  fromJson(json);
}

QString Meta::getName() const {
  return name;
}

QDateTime Meta::getCreationTime() const {
  return create;
}

QDateTime Meta::getModificationTime() const {
  return edit;
}

QString Meta::getUuid() const {
  return uuid.toString(QUuid::WithoutBraces);
}

QString Meta::newUuid() {
  uuid = QUuid::createUuid();
  return uuid.toString(QUuid::WithoutBraces);
}

void Meta::setName(const QString& newName) {
  name = newName;
  updateEditTime();
}

void Meta::updateEditTime() {
  edit = QDateTime::currentDateTime();
}

void Meta::updateCreationTime() {
  create = QDateTime::currentDateTime();
}

bool Meta::fromJson(const QJsonObject& json) {
  bool ret = !json.contains("Meta");
  QJsonObject jMeta = json["Meta"].toObject();
  name = jMeta["Name"].toString("");
  create = QDateTime::fromString(jMeta["Created"].toString(""), Qt::ISODate);
  edit = QDateTime::fromString(jMeta["Edited"].toString(""), Qt::ISODate);
  uuid = QUuid::fromString(jMeta["UUID"].toString(""));
  if (!create.isValid()) {
    create = QDateTime::currentDateTime();
    ret = false;
  }
  if (!edit.isValid()) {
    edit = QDateTime::currentDateTime();
    ret = false;
  }
  if (uuid.isNull()) {
    uuid = QUuid::createUuid();
  }
  return ret;
}

void Meta::toJson(QJsonObject& json) const {
  QJsonObject jMeta;
  jMeta["Name"] = name;
  jMeta["Created"] = create.toString(Qt::ISODate);
  jMeta["Edited"] = edit.toString(Qt::ISODate);
  jMeta["UUID"] = uuid.toString(QUuid::WithoutBraces);
  json["Meta"] = jMeta;
}
