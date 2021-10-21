#include <cstring>
#include "DCIBase.hh"
#include "Parser.hh"
#include "TcpServer.hh"

template <typename V>
void print_container_uint16(V v)
{
    for (auto x: v)
        printf(" 0x%.02hX", x);
}

/*!
 * C-code declares callback as:
 *    void (*func_ptr_logs)(unsigned char *ptr, int len)
 */
DCIBase *dci_object = NULL;

void proxy_log_handler(unsigned char *ptr, int len)
{
    if(dci_object)
        dci_object->_handle_log(ptr, len);
}

void proxy_event_handler(unsigned char *ptr, int len)
{
    if(dci_object)
        dci_object->_handle_event(ptr, len);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::cout << "\nDCIBase + Parser unit test (Pure software)\n";

    DCIBase dci;
    Parser parser;
    TcpServer server;

    dci.setup_interface();
/*
    // Utilizing C-style handler proxy function from DCI.cpp
    extern DCIBase *dci_object;
*/
    dci_object = &dci;

    using std::placeholders::_1;
    using std::placeholders::_2;

    log_callback_t callback = std::bind(&Parser::process_log, std::ref(parser), _1, _2);

    // Add a log handler
    dci.add_log_handler(callback);

    const char *packets[] = 
    {
        "ab\x2F\x51some datsdflivbu dsf dsfa aa 1",
        "cdd daYYbata  asdf asdf asdf asd9",
        "gh\x6C\x5AZZso me da asdf asdf asdfasdf t",
    };

    auto send_packets = [packets] {
        std::cout << "\nSending packets...";
        for(auto pkt: packets)
            proxy_log_handler( (uint8_t*)pkt, strlen(pkt));
    };

    auto print_line = []{ std::cout <<"\n_____________________________"; };

    print_line();
    std::unordered_set<uint16_t> log_codes { 0x5A71, 0x5A6C };
    std::cout << "\n1. Setting logs to catch:";
    print_container_uint16(log_codes);
    dci.add_log_codes(log_codes);

    send_packets();

    print_line();
    log_codes = {0x512F, 0x5B2F, 0x5A64, 0x5A70};
    std::cout << "\n2. Setting logs to catch:";
    print_container_uint16(log_codes);
    dci.set_log_codes(log_codes);

    send_packets();

    print_line();
    std::cout << "\n3. Resetting logs...";
    dci.reset_log_codes();

    send_packets();

    print_line();
    log_codes = { 0x5A6C };
    std::cout << "\n4. Setting logs to catch:";
    print_container_uint16(log_codes);
    dci.set_log_codes(log_codes);
    std::cout << "\n4.1 Enabling dump... ";
    std::cout << parser.bDump;
    parser.bDump = true;
    std::cout << " -> " << parser.bDump;

    send_packets();

    print_line();
    log_codes = { 0x512F };
    std::cout << "\n5. Adding logs to catch:";
    print_container_uint16(log_codes);
    dci.add_log_codes(log_codes);

    send_packets();

    std::cout << "\nDCIBase + Parser unit test is done\n";
    return 0;
}
