#include <iostream>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-document.h>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

struct Statistics {
  std::string longest_word;
  double average_word_length = 0.0;
  size_t total_words = 0;
};

Statistics merge_stats(const Statistics& a, const Statistics& b){
  Statistics result;
  result.total_words = a.total_words + b.total_words;
  result.average_word_length = (a.average_word_length * a.total_words + b.average_word_length * b.total_words) / result.total_words;
  result.longest_word = (a.longest_word.length() > b.longest_word.length()) ? a.longest_word : b.longest_word;
  return result;
}

Statistics analyze_text(const std::string& text) {
  std::stringstream ss(text);
  std::string word;
  std::string longest_word;
  size_t total_length = 0;
  size_t word_count = 0;

  while (ss >> word) {
    word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !std::isalpha(c); }), word.end());
    if (word.empty()) continue;

    word_count++;
    total_length += word.length();
    if (word.length() > longest_word.length()) longest_word = word;
  }

  Statistics stats;
  stats.longest_word = longest_word;
  stats.total_words = word_count;
  stats.average_word_length = word_count > 0 ? static_cast<double>(total_length) / word_count : 0.0;
  return stats;
}

void work(int thread_id, int start_page, int end_page, const poppler::document* doc, Statistics& threaded_stats) {
  std::ostringstream local_text;
  for (int page = start_page; page < end_page; page++) {
      auto pdf_page = doc->create_page(page);
      if (!pdf_page) continue;
      local_text << pdf_page->text().to_latin1();
  }
  threaded_stats = analyze_text(local_text.str());
}

int main() {
  std::string pdf_path = "text.pdf";
  std::cout << "Analyzing PDF: " << pdf_path << std::endl;
  auto doc = poppler::document::load_from_file(pdf_path);
  int num_pages = doc->pages();

  int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  std::vector<Statistics> threaded_stats(num_threads);
  std::mutex mtx;
  std::vector<std::string> threads_strings;

  int pages_per_thread = num_pages / num_threads;
  int pages_remainder = num_pages % num_threads;

  // Dividi et impera, we divide the whole pdf in different chunks 
  // both for extracting data and for analysing it.
 
  int start_page = 0;
  for (int i = 0; i < num_threads; i++){
    int local_pages = pages_per_thread + (i < pages_remainder ? 1 : 0);
    int end_page = start_page + local_pages;
    threads.emplace_back(work, i, start_page, end_page, doc, std::ref(threaded_stats[i]));
    start_page = end_page;
  }

  for (auto& t : threads) t.join();
  
  Statistics stats;
  for(const auto& s : threaded_stats) stats = merge_stats(stats, s);

  std::cout << "Total Words: " << stats.total_words << std::endl;
  std::cout << "Longest Word: " << stats.longest_word << std::endl;
  std::cout << "Average Word Length: " << stats.average_word_length << std::endl;

  return 0;
}

