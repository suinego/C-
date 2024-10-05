#include "cpp-23-24-string.hpp"

template <typename T>
struct RemoveReference {
  using Type = T;
};
template <typename T>
struct RemoveReference<T&> {
  using Type = T;
};
template <typename T>
struct RemoveReference<T&&> {
  using Type = T;
};

template <typename T>
using STDRemoveReferenceT = typename RemoveReference<T>::Type;

template <typename T>
auto STDMove(T&& value) noexcept -> STDRemoveReferenceT<T>&& {
  return static_cast<STDRemoveReferenceT<T>&&>(value);
}

template <typename T>
void STDSwap(T& left, T& right) {
  T temp = STDMove(left);
  left = STDMove(right);
  right = STDMove(temp);
}

void String::Swap(String& str) {
  STDSwap(size_, str.size_);
  STDSwap(capacity_, str.capacity_);
  STDSwap(str_, str.str_);
}

String::String(size_t size, char symbols) : size_(size), capacity_(size_) {
  str_ = new char[size_ + 1];
  memset(str_, symbols, size_);
  str_[size_] = '\0';
}

[[nodiscard]] size_t String::Capacity() const { return capacity_; }

String::String(const char* strc) : size_(strlen(strc)), capacity_(size_) {
  str_ = new char[size_ + 1];
  memcpy(str_, strc, size_);
  str_[size_] = '\0';
}

String::String(const String& str_copy)
    : size_(str_copy.size_), capacity_(str_copy.size_) {
  str_ = new char[size_ + 1];
  memcpy(str_, str_copy.str_, size_);
  str_[size_] = '\0';
}

String::~String() { delete[] str_; }

String& String::operator=(String str_copy) {
  if (this == &str_copy) {
    return *this;
  }
  Swap(str_copy);
  return *this;
}

[[nodiscard]] size_t String::Size() const { return size_; }

String& String::operator+=(const String& str) {
  if (capacity_ < size_ + str.size_) {
    Reserve(2 * (size_ + str.size_));
  }
  if (str_ != nullptr) {
    memcpy(str_ + size_, str.str_, str.size_);
    size_ += str.size_;
    str_[size_] = '\0';
  }
  return *this;
}

String operator+(const String& first_str, const String& second_str) {
  String copy(first_str);
  copy += second_str;
  return copy;
}

String& String::operator*=(size_t num) {
  if (num == 0) {
    Clear();
    return *this;
  }
  size_t new_size = size_ * num;
  Reserve(new_size + 1);
  for (size_t ind = 1; ind < num; ++ind) {
    memcpy(str_ + size_ * ind, str_, size_);
  }
  size_ *= num;
  str_[size_] = '\0';
  return *this;
}

String operator*(const String& other, size_t num) {
  String other_copy = other;
  return other_copy *= num;
}

void String::Clear() {
  if (this->Empty()) {
    return;
  }
  size_ = 0;
  str_[0] = '\0';
}

void String::PushBack(char symbols) {
  if (size_ == capacity_ || str_ == nullptr) {
    Reserve(2 * size_ + 1);
  }
  str_[size_] = symbols;
  ++size_;
  str_[size_] = '\0';
}

void String::PopBack() {
  if (str_ == nullptr) {
    return;
  }
  *(str_ + --size_) = '\0';
}
void String::Resize(size_t new_size) {
  Reserve(new_size + 1);
  if (new_size > size_) {
    memset(str_ + size_, '\0', new_size - size_);
  }
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Resize(size_t new_size, char symbols) {
  Reserve(new_size + 1);
  if (new_size > size_) {
    memset(str_ + size_, symbols, new_size - size_);
  }
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    char* new_str = new char[new_capacity + 1];
    if (str_ != nullptr) {
      memcpy(new_str, str_, size_);
      delete[] str_;
    }
    capacity_ = new_capacity;
    str_ = new_str;
    str_[size_] = '\0';
  }
}
const char* String::Data() const { return str_; }

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    char* new_str = new char[size_ + 1];
    memcpy(new_str, str_, size_);
    new_str[size_] = '\0';
    delete[] str_;
    str_ = new_str;
    capacity_ = size_;
  }
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.str_;
  return out;
}

std::istream& operator>>(std::istream& in, String& str) {
  str.Clear();
  char character;
  while (in.get(character) && !in.eof()) {
    str.PushBack(character);
  }
  return in;
}

char& String::operator[](size_t element) { return str_[element]; }

[[nodiscard]] const char& String::operator[](size_t element) const { return str_[element]; }

char& String::Front() { return str_[0]; }

const char& String::Front() const { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

const char& String::Back() const { return str_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

bool operator<(const String& first, const String& second) {
  for (size_t ind_first = 0, ind_second = 0;
       ind_first < first.Size() && ind_second < second.Size();
       ++ind_first, ++ind_second) {
    if (first[ind_first] < second[ind_second]) {
      return true;
    }
    if (first[ind_first] > second[ind_second]) {
      return false;
    }
  }
  return first.Size() < second.Size();
}

bool operator>(const String& first, const String& second) {
  return second < first;
}
bool operator<=(const String& first, const String& second) {
  return !(second < first);
}
bool operator>=(const String& first, const String& second) {
  return !(second > first);
}
bool operator==(const String& first, const String& second) {
  return (!(first > second) && !(first < second));
}
bool operator!=(const String& first, const String& second) {
  return !(first == second);
}

bool Compare(const char* left, const char* right, size_t size) {
  for (size_t ind = 0; ind < size; ++ind) {
    if (right[ind] != left[ind]) {
      return false;
    }
  }
  return true;
}

size_t GetStringsSize(const auto& strings) {
  size_t result = 0;
  for (const auto& string : strings) {
    result += string.size();
  }
  return result;
}

std::vector<String> String::Split(const String& delim) {
  String tmp;
  std::vector<String> res;
  size_t reserved_space = (size_ / delim.size_ + 1) * (size_ / 2);
  Reserve(reserved_space);
  size_t i = 0;
  for (; i < size_ - delim.size_ + 1; ++i) {
    if (Compare(str_ + i, delim.str_, delim.size_)) {
      res.push_back(tmp);
      tmp.Clear();
      i += delim.size_ - 1;
    } else {
      tmp.PushBack(str_[i]);
    }
  }
  for (size_t index = i; index < size_; ++index) {
    tmp.PushBack(str_[index]);
  }
  res.push_back(tmp);
  return res;
}
template <typename T, template <class> typename Container>
  requires std::is_base_of_v<std::string, T>
[[nodiscard]] String String::Join(const Container<T>& strings) const {
  if (strings.Empty()) {
    return String();  // copy ellision so because of this would not be copy of
                      // String()
  }
  String result;
  result.Reserve(GetStringsSize(strings) + (strings.Size() - 1) * Size() + 1);
  auto iter = (strings + 0);
  result += *iter;
  ++iter;
  while (iter != (strings.str_ + Size())) {
    result += *this;
    result += *iter;
    ++iter;
  }
  return result;
}
