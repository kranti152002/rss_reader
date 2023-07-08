#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "rapidxml/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

// Callback function for writing received data
size_t write_callback(char* ptr, size_t size, size_t nmemb, string* data) {
    size_t totalSize = size * nmemb;
    data->append(ptr, totalSize);
    return totalSize;
}

// Function to fetch the XML data from the specified URL
string fetch_rss_data(const string& url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            cerr << "Error occurred while fetching RSS data: " << curl_easy_strerror(result) << endl;
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

// Function to parse the XML data and extract headlines
vector<string> parse_rss_data(const string& xmlData) {
    vector<string> headlines;
    xml_document<> doc;
    doc.parse<0>(const_cast<char*>(xmlData.c_str()));

    xml_node<>* root = doc.first_node();
    xml_node<>* channelNode = root->first_node("channel");

    for (xml_node<>* itemNode = channelNode->first_node("item"); itemNode; itemNode = itemNode->next_sibling()) {
        xml_node<>* titleNode = itemNode->first_node("title");
        if (titleNode) {
            headlines.push_back(titleNode->value());
        }
    }

    return headlines;
}

int main() {
    string rssUrl = "https://example.com/rss"; // Replace with the desired RSS feed URL

    // Fetch RSS data
    string rssData = fetch_rss_data(rssUrl);

    // Parse RSS data and extract headlines
    vector<string> headlines = parse_rss_data(rssData);

    // Display headlines
    cout << "Latest Headlines:" << endl;
    for (const auto& headline : headlines) {
        cout << "- " << headline << endl;
    }

    return 0;
}
