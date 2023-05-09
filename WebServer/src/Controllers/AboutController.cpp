#include "AboutController.h"

AboutController::AboutController(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void AboutController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    QByteArray language = request.getHeader("Accept-Language");

    response.setHeader("Content-Type", "text/html; charset = UTF-8");
    //get tamplate
    Template tempMain = templateCache->getTemplate("about",language);
    tempMain.setCondition("logged-in", session.contains("username"));
    response.write(tempMain.toUtf8(), true);

}
