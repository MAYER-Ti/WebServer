#include "HomeController.h"

HomeController::HomeController(QObject *parent) : HttpRequestHandler(parent)
{

}

void HomeController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    QByteArray language = request.getHeader("Accept-Language");

    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    //set template
    Template temp = templateCache->getTemplate("home", language);
    temp.setCondition("logged-in", session.contains("username"));
    response.write(temp.toUtf8(), true);
}
