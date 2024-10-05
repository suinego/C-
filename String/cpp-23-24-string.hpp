#pragma once
#include <string.h>
#include <concepts>
#include <iostream>
#include <vector>

class String {
 private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  char* str_ = nullptr;

 public:
  String() = default;
  String(size_t size, char symbols);
  String(const char* strc);
  String(const String& str_copy);
  String& operator=(String str_copy);
  String& operator+=(const String& str);
  friend String operator+(const String& fisrt_str, const String& second_str);
  friend bool operator<(const String& first, const String& second);
  friend bool operator>(const String& first, const String& second);
  friend bool operator>=(const String& first, const String& second);
  friend bool operator<=(const String& first, const String& second);
  friend bool operator==(const String& first, const String& second);
  friend bool operator!=(const String& first, const String& second);
  friend std::ostream& operator<<(std::ostream& out, const String& str);
  friend std::istream& operator>>(std::istream& in, String& str);
  String& operator*=(size_t num);
  friend String operator*(const String& other, size_t num);
  void Swap(String& str);
  ~String();
  const char* Data() const;
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char symbols);
  void Reserve(size_t new_capacity);
  size_t Size() const;
  size_t Capacity() const;
  char& operator[](size_t element);
  const char& operator[](size_t element) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;
  bool Empty() const;
  void Clear();
  void PushBack(char symbols);
  void ShrinkToFit();
  std::vector<String> Split(const String& delim = " ");
  template <typename T, template <class> typename Container>
    requires std::is_base_of_v<std::string, T>
  [[nodiscard]] String Join(const Container<T>& strings) const;
};
