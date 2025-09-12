#include <bits/stdc++.h>
using namespace std;

// if a prefix [1 - i] is a palindrome, then the degree is just degree of the prefix of [i / 2] + 1
// so we just need to see if [1 - i] is a palindrome
// this can be done with hash or manacher
// manacher is a bit overkill so let's just use hash

// TODO(template)
template <int SIZE = 1>
class hash_t {
public:
// slower but harder to hack
#define DYNAMIC_HASH

#ifdef DYNAMIC_HASH
  inline static int64_t mod[SIZE];
  inline static int64_t base[SIZE];
#else
  inline static constexpr int64_t mod[] = {999797999, 999999937, 1000000007};
  inline static constexpr int64_t base[] = {31, 37, 131};
#endif
  static_assert(sizeof(mod) / sizeof(mod[0]) >= SIZE);
  static_assert(sizeof(base) / sizeof(base[0]) >= SIZE);

  class hash_element_t {
  public:
      int64_t value[SIZE];

      hash_element_t() = default;
      hash_element_t(const int64_t& x) {
          for (int i = 0; i < SIZE; i++) value[i] = x % mod[i];
      }
      constexpr hash_element_t(const int64_t* data) : value() {
          for (int i = 0; i < SIZE; i++) value[i] = data[i];
      }

      int64_t& operator[](const int& i) { return value[i]; }
      const int64_t& operator[](const int& i) const { return value[i]; }

      hash_element_t operator+(const hash_element_t& other) const {
          hash_element_t res;
          for (int i = 0; i < SIZE; i++) {
              res[i] = value[i] + other[i];
              if (res[i] >= mod[i]) res[i] -= mod[i];
          }
          return res;
      }

      hash_element_t& operator+=(const hash_element_t& other) {
          for (int i = 0; i < SIZE; i++) {
              value[i] += other[i];
              if (value[i] >= mod[i]) value[i] -= mod[i];
          }
          return *this;
      }

      hash_element_t operator-(const hash_element_t& other) const {
          hash_element_t res;
          for (int i = 0; i < SIZE; i++) {
              res[i] = value[i] - other[i];
              if (res[i] < 0) res[i] += mod[i];
          }
          return res;
      }

      hash_element_t& operator-=(const hash_element_t& other) {
          for (int i = 0; i < SIZE; i++) {
              value[i] -= other[i];
              if (value[i] < 0) value[i] += mod[i];
          }
          return *this;
      }

      hash_element_t operator*(const hash_element_t& other) const {
          hash_element_t res;
          for (int i = 0; i < SIZE; i++) {
              res[i] = value[i] * other[i];
              res[i] %= mod[i];
          }
          return res;
      }

      hash_element_t& operator*=(const hash_element_t& other) {
          for (int i = 0; i < SIZE; i++) {
              value[i] *= other[i];
              value[i] %= mod[i];
          }
          return *this;
      }

      bool operator!=(const hash_element_t& other) const {
          for (int i = 0; i < SIZE; i++)
              if (value[i] != other[i]) return true;
          return false;
      }

      bool operator==(const hash_element_t& other) const {
          for (int i = 0; i < SIZE; i++)
              if (value[i] != other[i]) return false;
          return true;
      }
  };

#ifdef DYNAMIC_HASH
  static bool is_prime(const int64_t& x) {
      if (x % 2 == 0) return false;
      if (x % 3 == 0) return false;
      if (x % 5 == 0) return false;
      if (x % 7 == 0) return false;
      for (int64_t i = 6; i * i <= x; i += 6) {
          if (x % (i - 1) == 0) return false;
          if (x % (i + 1) == 0) return false;
      }
      return true;
  }

  inline static mt19937_64 rng{chrono::high_resolution_clock::now().time_since_epoch().count()};

  inline static int64_t random_prime(uniform_int_distribution<int64_t>& distribution) {
      int64_t x = distribution(rng);
      while (!is_prime(x)) x++;
      return x;
  }

  inline static uniform_int_distribution<int64_t> random_base{256, 1024};
  inline static uniform_int_distribution<int64_t> random_mod{1'000'000'000, 2'000'000'000};
  inline static hash_element_t base_element;
#else
  inline static constexpr hash_element_t base_element{base};
#endif

  inline static vector<hash_element_t> base_power;
  static void build_common() {
      if (base_power.size()) return;
#ifdef DYNAMIC_HASH
      for (int i = 0; i < SIZE; i++) {
          base[i] = random_prime(random_base);
          mod[i] = random_prime(random_mod);
      }
      base_element = hash_element_t(base);
#endif
      for (int i = 0; i < SIZE; i++) cerr << i << "-th hash: " << base[i] << ' ' << mod[i] << '\n';
      base_power.emplace_back(1);
  }

  vector<hash_element_t> prefix_hash;

  hash_t() : prefix_hash(){};

  template <class S>
  void build(const S& s) {
      build_common();
      while (base_power.size() <= s.size()) base_power.push_back(base_power.back() * base_element);
      prefix_hash.resize(s.size());
      prefix_hash[0] = s[0];
      for (int i = 1; i < s.size(); i++) prefix_hash[i] = (prefix_hash[i - 1] * base_element) + s[i];
  }

  template <class S>
  hash_t(const S& s) : prefix_hash() {
      build(s);
  }

  hash_element_t operator()(const int& l, const int& r) const {
      if (l == 0) return prefix_hash[r];
      return prefix_hash[r] - (prefix_hash[l - 1] * base_power[r - l + 1]);
  }
};

string s;
using h_t=hash_t<>;
h_t hash_s,hash_reversed_s;
int64_t degree[5000001];
int main(){
  cin>>s;
  hash_s.build(s);
  reverse(s.begin(),s.end());
  hash_reversed_s.build(s);
  int64_t ans=0;
  for(int i=0;i<s.size();++i){
    if(hash_s(0,i)==hash_reversed_s(s.size()-i-1,s.size()-1)){
      degree[i+1]=degree[(i+1)/2]+1;
    }
    ans+=degree[i+1];
  }
  cout<<ans<<endl;
}
