#ifndef REQUESTPRO_H
#define REQUESTPRO_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>

class RequestPro : public QObject {
    Q_OBJECT
public:
    explicit RequestPro(QObject *parent = nullptr) : QObject(parent) {}

    void sendRequest(const QJsonObject &headers, const QJsonObject &params, const QUrl &url, const QString &requestType) {
        QNetworkRequest request(url);
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        if (requestType.toUpper() == "GET") {
            setHeaders(request, headers);
            QUrlQuery query;
            for (auto it = params.begin(); it != params.end(); ++it) {
                query.addQueryItem(it.key(), it.value().toString());
            }
            QUrl urlWithParams = url;
            urlWithParams.setQuery(query);
            request.setUrl(urlWithParams);
            connect(manager, &QNetworkAccessManager::finished, this, &RequestPro::handleResponse);
            manager->get(request);
        } else if (requestType.toUpper() == "POST") {
            QJsonDocument jsonDoc(params);
            QByteArray jsonData = jsonDoc.toJson();
            qDebug() << QString::fromUtf8(jsonData);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            setHeaders(request, headers);
            connect(manager, &QNetworkAccessManager::finished, this, &RequestPro::handleResponse);
            manager->post(request, jsonData);
        }
    }

signals:
    void requestCompleted(const QJsonObject &response);
    void requestError(const QString &error);

private slots:
    void handleResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            emit requestCompleted(jsonResponse.object());
        } else {
            emit requestError(reply->errorString());
        }
        reply->deleteLater();
    }

private:
    void setHeaders(QNetworkRequest &request, const QJsonObject &headers) {
        for (auto it = headers.begin(); it != headers.end(); ++it) {
            request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
        }
    }
};

#endif // REQUESTPRO_H
