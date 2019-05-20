#include <iostream>
#include <fstream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::ref;


bool validate(server & s, connection_hdl hdl) {
    server::connection_ptr con = s.get_con_from_hdl(hdl);

    std::cout << "Cache-Control: " << con->get_request_header("Cache-Control") << std::endl;

    const std::vector<std::string> & subp_requests = con->get_requested_subprotocols();
    std::vector<std::string>::const_iterator it;

    for (it = subp_requests.begin(); it != subp_requests.end(); ++it) {
        std::cout << "Requested: " << *it << std::endl;
    }

    // here need to check for dds-protocol and return a false if it does not match
    if (subp_requests.size() > 0) {
        con->select_subprotocol(subp_requests[0]);
    }

    return true;
}

// Blocking HTTP Handler
void on_http(server * s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    
    std::stringstream output;

    std::ifstream f;
    std::string fName;

    fName = con->get_resource();
    if (fName.substr(0,1) == "/" && fName.length()>1)
       fName = fName.substr(1,fName.length());
    //std::cout << "opening " << fName << std::endl;
    f.open(fName);
    if (f)
    {
       output << f.rdbuf();    
       f.close();
       // Set status to 200 rather than the default error code
       con->set_status(websocketpp::http::status_code::ok);
    }
#ifdef XXX
    output << "<!doctype html><html><body>You requested "
           << con->get_resource()
           << "</body></html>";
   
#endif 
    // Set status to 200 rather than the default error code
    //con->set_status(websocketpp::http::status_code::ok);
    // Set body text to the HTML created above
    con->set_body(output.str());
}

int main() {
    try {
        server s;

        s.set_validate_handler(bind(&validate,ref(s),::_1));
        s.set_http_handler(bind(&on_http,&s,::_1));

        s.init_asio();
        s.listen(9005);
        s.start_accept();

        s.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
