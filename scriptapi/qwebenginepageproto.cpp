/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#include "qwebenginepageproto.h"

QScriptValue QWebEnginePagetoScriptValue(QScriptEngine *engine, QWebEnginePage* const &item)
{
  return engine->newQObject(item);
}

void QWebEnginePagefromScriptValue(const QScriptValue &obj, QWebEnginePage* &item)
{
  item = qobject_cast<QWebEnginePage*>(obj.toQObject());
}

void setupQWebEnginePageProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QWebEnginePagetoScriptValue, QWebEnginePagefromScriptValue);

  QScriptValue proto = engine->newQObject(new QWebEnginePageProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QWebEnginePage*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQWebEnginePage, proto);
  engine->globalObject().setProperty("QWebEnginePage",  constructor);
}

QScriptValue constructQWebEnginePage(QScriptContext * context, QScriptEngine  *engine)
{
  QWebEnginePage *obj = 0;
  if (context->argumentCount() >= 1)
    obj = new QWebEnginePage(qobject_cast<QObject*>(context->argument(0).toQObject()));
  else
    obj = new QWebEnginePage();
  return engine->toScriptValue(obj);
}

QAction* QWebEnginePageProto::action(QWebEnginePage::WebAction action) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->action(action);
  return 0;
}

QColor QWebEnginePageProto::backgroundColor() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->backgroundColor();
  return QColor();
}

QSizeF QWebEnginePageProto::contentsSize() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->contentsSize();
  return QSizeF();
}

// const QWebEnginePageProto::() const
// {
//   QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
//   if (item)
//     return item->();
//   return const();
// }

QMenu* QWebEnginePageProto::createStandardContextMenu()
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->createStandardContextMenu();
  return 0;
}

void QWebEnginePageProto::download(const QUrl & url, const QString & filename)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->download(url, filename);
}

void QWebEnginePageProto::findText(const QString & subString, QWebEnginePage::FindFlag options)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->findText(subString, options);
}

void QWebEnginePageProto::findText(const QString & subString, QWebEnginePage::FindFlag options, std::function<void(bool)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->findText(subString, options, resultCallback);
}

bool QWebEnginePageProto::hasSelection() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->hasSelection();
  return false;
}

QWebEngineHistory* QWebEnginePageProto::history() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->history();
  return 0;
}

QIcon QWebEnginePageProto::icon() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->icon();
  return QIcon();
}

QUrl QWebEnginePageProto::iconUrl() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->iconUrl();
  return QUrl();
}

bool QWebEnginePageProto::isAudioMuted() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->isAudioMuted();
  return false;
}

void QWebEnginePageProto::load(const QUrl & url) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->load(url);
}

void QWebEnginePageProto::load(const QWebEngineHttpRequest & request)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->load(request);
}

void QWebEnginePageProto::print(QPrinter *printer, std::function<void(bool)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->print(printer, resultCallback);
}

void QWebEnginePageProto::printToPdf(const QString & filePath, const QPageLayout & pageLayout)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->printToPdf(filePath, pageLayout);
}

void QWebEnginePageProto::printToPdf(std::function<void(QByteArray)> resultCallback, const QPageLayout & pageLayout)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->printToPdf(resultCallback, pageLayout);
}

QWebEngineProfile* QWebEnginePageProto::profile() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->profile();
  return 0;
}

bool QWebEnginePageProto::recentlyAudible() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->recentlyAudible();
  return false;
}

void QWebEnginePageProto::replaceMisspelledWord(const QString & replacement)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->replaceMisspelledWord(replacement);
}

QUrl QWebEnginePageProto::requestedUrl() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->requestedUrl();
  return QUrl();
}

void QWebEnginePageProto::runJavaScript(const QString & scriptSource, quint32 worldId, std::function<void(QVariant)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, worldId, resultCallback);
}

void QWebEnginePageProto::runJavaScript(const QString & scriptSource, quint32 worldId)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, worldId);
}

void QWebEnginePageProto::runJavaScript(const QString & scriptSource, std::function<void(QVariant)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, resultCallback);
}

void QWebEnginePageProto::runJavaScript(const QString & scriptSource)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource);
}

void QWebEnginePageProto::save(const QString & filePath, QWebEngineDownloadItem::SavePageFormat format) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->save(filePath, format);
}

QPointF QWebEnginePageProto::scrollPosition() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->scrollPosition();
  return QPointF();
}

QString QWebEnginePageProto::selectedText() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->selectedText();
  return 0;
}

void QWebEnginePageProto::setAudioMuted(bool muted)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setAudioMuted(muted);
}

void QWebEnginePageProto::setBackgroundColor(const QColor & color)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setBackgroundColor(color);
}

void QWebEnginePageProto::setContent(const QByteArray & data, const QString & mimeType, const QUrl & baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setContent(data, mimeType, baseUrl);
}

void QWebEnginePageProto::setFeaturePermission(const QUrl & securityOrigin, QWebEnginePage::Feature feature, QWebEnginePage::PermissionPolicy policy)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setFeaturePermission(securityOrigin, feature, policy);
}

void QWebEnginePageProto::setHtml(const QString & html, const QUrl & baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setHtml(html, baseUrl);
}

void QWebEnginePageProto::setUrl(const QUrl & url)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setUrl(url);
}

void QWebEnginePageProto::setView(QWidget * view)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setView(view);
}

void QWebEnginePageProto::setWebChannel(QWebChannel * channel, int worldId)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel, worldId);
}

void QWebEnginePageProto::setWebChannel(QWebChannel * channel)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel);
}

void QWebEnginePageProto::setZoomFactor(qreal factor)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setZoomFactor(factor);
}

QWebEngineSettings* QWebEnginePageProto::settings() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->settings();
  return 0;
}

QString QWebEnginePageProto::title() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->title();
  return QString();
}

void QWebEnginePageProto::toHtml(std::function<void(QString)> resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toHtml(resultCallback);
}

void QWebEnginePageProto::toPlainText(std::function<void(QString)> resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toPlainText(resultCallback);
}

void QWebEnginePageProto::triggerAction(QWebEnginePage::WebAction action, bool checked)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->triggerAction(action, checked);
}

QUrl QWebEnginePageProto::url() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->url();
  return QUrl();
}

QWidget* QWebEnginePageProto::view() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->view();
  return 0;
}

QWebChannel* QWebEnginePageProto::webChannel() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->webChannel();
  return 0;
}

qreal QWebEnginePageProto::zoomFactor() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->zoomFactor();
  return 0;
}

bool QWebEnginePageProto::event(QEvent * ev)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->event(ev);
  return false;
}
