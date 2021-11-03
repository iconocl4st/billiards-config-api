//
// Created by thallock on 11/2/21.
//

#ifndef IDEA_GET_DEFAULT_HOST_H
#define IDEA_GET_DEFAULT_HOST_H

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace billiards::config {

    /*
    [[nodiscard]] inline
    std::string get_default_host() {
        return "10.0.0.140";
    }
    */

    [[nodiscard]] inline
    std::string get_default_host() {
        struct ifaddrs * ifAddrStruct = nullptr;
        struct ifaddrs * ifa = nullptr;
        void * tmpAddrPtr = nullptr;

        getifaddrs(&ifAddrStruct);

        std::string ret_val{"127.0.0.1"};
        std::string desired_prefix{"10.0.0."};

        for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr) {
                continue;
            }
            if (ifa->ifa_addr->sa_family == AF_INET) {
                // is a valid IP4 Address
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN + 1];
                addressBuffer[INET_ADDRSTRLEN] = 0;
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                std::string tmp_ip{addressBuffer};
                if (!tmp_ip.compare(0, desired_prefix.size(), desired_prefix)) {
                    ret_val = addressBuffer;
                    std::cout << "Using ip = " << ret_val << std::endl;
                }
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                // is a valid IP6 Address
                tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                // TODO: maybe support ipv6 in the future...
            }
        }
        if (ifAddrStruct != nullptr) {
            freeifaddrs(ifAddrStruct);
        }
        return ret_val;
    }
}

#endif //IDEA_GET_DEFAULT_HOST_H
