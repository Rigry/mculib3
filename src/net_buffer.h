#pragma once

#include <array>
#include <type_traits>


template<size_t size_>
class Net_buffer : public std::array<uint8_t,size_> {
protected:
   using Container = std::array<uint8_t,size_>;
   size_t begin_i {0};
   size_t end_i   {0};
   auto& base() { return *static_cast<Container*>(this); }
public:
   void clear() { begin_i = 0; end_i = 0; }
   auto begin() { return base().begin() + begin_i; }
   auto end()   { return base().begin() + end_i; }
   auto size()  { return end_i - begin_i; }
   void set_size (size_t v) { begin_i = 0; end_i = std::min(v,size_); }

   Net_buffer& operator<< (const uint8_t& v)
   {
      if (end_i < size_) {
         *end() = v;
         end_i++;
      }
      return *this;
   }

   auto to_bytes (uint16_t v)
   {
      union {
         std::array<uint8_t,2> d8;
         uint16_t d16;
      } u;
      u.d16 = v;
      return std::tuple {u.d8[0], u.d8[1]};
   }

   auto from_bytes (uint8_t v1, uint8_t v2)
   {
      union {
         std::array<uint8_t,2> d8;
         uint16_t d16;
      } u;
      u.d8 = {v2, v1};
      return u.d16;
   }

   Net_buffer& operator<< (uint16_t v)
   {
      auto [low, hi] = to_bytes(v);
      *this << hi << low;
      return *this;
   }

   template<class U>
   std::enable_if_t<std::is_same_v<typename U::value_type, uint8_t>, Net_buffer&>
   operator<< (const U& v)
   {
      if (end_i + v.size() < size_) {
         std::copy (v.begin(), v.end(), end());
         end_i += v.size();
      }
      return *this;
   }

   Net_buffer& operator>> (uint8_t& v)
   {
      if (end_i > begin_i)
         v = base()[begin_i++];

      return *this;
   }

   Net_buffer& operator>> (uint16_t& v)
   {
      if (end_i > begin_i + 1) {
         union {
            std::array<uint8_t,2> d8;
            uint16_t d16;
         } u;
         auto v1 = pop_front();
         auto v2 = pop_front();
         u.d8 = {v2, v1};
         v = u.d16;
      }
         // v = from_bytes (pop_front(), pop_front());
      return *this;
   }

   uint8_t pop_back()
   {
      if (end_i > begin_i)
         return base()[--end_i];
      return 0;
   }

   uint8_t pop_front()
   {
      if (end_i > begin_i)
         return base()[begin_i++];
      return 0;
   }

   uint8_t& front()
   {
      return base()[begin_i];
   }
   
};