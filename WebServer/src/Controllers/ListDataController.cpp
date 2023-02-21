#include "ListDataController.h"



ListDataController::ListDataController(QObject *parent)
    : HttpRequestHandler{parent}
{
    list.append("Robert");
    list.append("Lisa");
    list.append("Hannah");
    list.append("Ludwig");
    list.append("Miranda");
    list.append("Francesco");
    list.append("Kim");
    list.append("Jacko");
}

void ListDataController::service(HttpRequest &request, HttpResponse &response)
{
    response.setHeader("Content-Type", "text/html; charset = UTF-8");
    response.write("<html><body><center>");

    response.write("The time is ");
        QString now = QTime::currentTime().toString("HH:mm:ss");
        response.write(now.toUtf8());

    response.write("<p>List of names:");
        response.write("<table border='1' cellspacing='0'>");
        for(int i = 0; i<list.size(); i++) {
            QString number = QString::number(i);
            QString name = list.at(i);
            response.write("<tr><td>");
            response.write(number.toUtf8());
            response.write("</td><td>");
            response.write(name.toUtf8());
            response.write("</td></tr>");
        }
        response.write("</table>");
        //finish tag
        response.write("</center></body></html>",true);
}
