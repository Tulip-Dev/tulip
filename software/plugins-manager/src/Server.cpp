#include "Server.h"
#include "Request.h"

using namespace std;

namespace tlp {

  Server::Server(const string& serverAddress,QObject* parent) : QObject(parent){
    this->serverAddress = serverAddress;
    http = new HttpRequest(serverAddress);
    connect(http,SIGNAL(done()),this,SLOT(requestDone()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
  }
  
  Server::~Server(){
    delete http;
  }
  
  void Server::send(Request* request){
    requests.push_back(request);
    if(requests.size() == 1) {
      if(request->post){
	string result;
	((SOAPRequest*)request)->getXml(result);
	http->request(result);
      }else{
	http->get(((GetRequest*)request)->getFileName,((GetRequest*)request)->outFileName);
      }
      timer.start(30000);
    }
  }
  
  void Server::getAddress(string &address){
    address=serverAddress;
  }
  
  void Server::requestDone() {
    timer.stop();
    Request* lastRequest = requests.front();

    if(lastRequest->post) {
      string response;
      http->getResponse(response);

      SoapResponseReader srr(response);
      string function;
      string server;

      if(!srr.getFunctionName(function)) {
	return;
      }

      http->getServerName(server);
    
      string returnedData;
      if(!srr.getReturnedData(returnedData)) {
	send(lastRequest);
      }else{
	((SOAPRequest*)lastRequest)->applyResponseTreatment(returnedData);
      }
    }else{
      ((GetRequest*)lastRequest)->applyRequestFinish();
    }
      
    requests.pop_front();
    
    if(requests.size() > 0) {
      if((*requests.begin())->post){
	string result;
	((SOAPRequest*)(*requests.begin()))->getXml(result);
	http->request(result);
      }else{
	http->get(((GetRequest*)(*requests.begin()))->getFileName,((GetRequest*)(*requests.begin()))->outFileName);
      }
    }
    
    delete lastRequest;
  }

  void Server::timeout() {
    Request* lastRequest = requests.front();
    requests.pop_front();
    send(lastRequest);
  }

}
