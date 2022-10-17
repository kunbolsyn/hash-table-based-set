#ifndef SET_
#define SET_

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>

namespace case_insensitive
{
   size_t hash(const std::string &s);
   bool equal(const std::string &s1, const std::string &s2);
}

class set
{
   size_t set_size;

   double max_load_factor;

   using buckettype = std::list<std::string>;

   std::vector<buckettype> buckets;

   bool insert_norehash(const std::string &s);

public:
   set(double max_load_factor = 3.0)
       : set_size(0),
         max_load_factor(max_load_factor),
         buckets(std::vector<buckettype>(8))
   {
      if (max_load_factor <= 0.0)
         throw std::runtime_error("max load factor must be > 0");
   }

   set(const set &s) = default;
   set &operator=(const set &s) = default;
   ~set() = default;
   set(std::initializer_list<std::string> init, double max_load_factor = 3);

   bool contains(const std::string &s) const;
   bool insert(const std::string &s);

   bool remove(const std::string &s);

   size_t size() const;

   bool empty() const;

   void clear();

   void consider_rehash();

   void print(std::ostream &out) const;

   double loadfactor() const
   {
      return static_cast<double>(set_size) / buckets.size();
   }

   double standarddev() const
   {
      double sum = 0.0;
      double lf = loadfactor();

      for (const auto &l : buckets)
      {
         double dif = l.size() - lf;
         sum += dif * dif;
      }

      return sqrt(sum / buckets.size());
   }

   std::ostream &printstatistics(std::ostream &out) const
   {
      out << "set size =            " << size() << "\n";
      out << "load factor =         " << loadfactor() << " ( max = " << max_load_factor << " )\n";
      out << "standard deviation =  " << standarddev() << "\n";
      return out;
   }
};

inline std::ostream &operator<<(std::ostream &out, const set &s)
{
   s.print(out);
   return out;
}

#endif
