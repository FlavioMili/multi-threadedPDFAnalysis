#include <iostream>
#include <string>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-document.h>
#include <sstream>
#include <cctype>
#include <algorithm>

struct Statistics {
  std::string longest_word;
  double average_word_length = 0.0;
  size_t total_words = 0;
};

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

std::string extract_pdf_text(const std::string& pdf_path) {
  std::string full_text;
  auto doc = poppler::document::load_from_file(pdf_path);

  for (int i = 0; i < doc->pages(); i++) {
    auto page = doc->create_page(i);
    if (!page) continue;

    full_text += page->text().to_latin1();
  }

  return full_text;
}

int main() {
  std::string pdf_path = "text.pdf";
  std::cout << "Analyzing PDF: " << pdf_path << std::endl;

  std::string text = extract_pdf_text(pdf_path);

  Statistics stats = analyze_text(text);

  std::cout << "Total Words: " << stats.total_words << std::endl;
  std::cout << "Longest Word: " << stats.longest_word << std::endl;
  std::cout << "Average Word Length: " << stats.average_word_length << std::endl;

  return 0;
}

