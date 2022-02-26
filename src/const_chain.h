#pragma once

#include <algorithm>
#include <iterator>

/// переменная этого класса
/// может принимать только значения из values
/// если присвоить к значению не из values, оно не изменится
/// операторы ++ и -- изменяют на следующее/предыдущее значение в цепочке
/// инкремент/декремент крайних значений присвает значение противоволожного конца
template<class T, T ... values>
class ConstChain
{
public:
   ConstChain();
   operator T() const;
   T operator++();
   T operator--();
   /// может присваивать только занчения из values...
   T operator= (T val);
private:
   static constexpr T arr[sizeof...(values)] = {values...};
   // оптимизировал зараза, потому volatile
   volatile std::size_t index;
};






template<class T, T ... values>
constexpr T ConstChain<T,values...>::arr[sizeof...(values)];



template<class T, T ... values>
ConstChain<T,values...>::ConstChain() : index{0} {}

template<class T, T ... values>
ConstChain<T,values...>::operator T() const
{
   return arr[index];
}



template<class T, T ... values>
T ConstChain<T,values...>::operator++ ()
{
   ++index;
   if (index == sizeof...(values))
      index = 0;
   return arr[index];
}



template<class T, T ... values>
T ConstChain<T,values...>::operator-- ()
{
   if (index != 0)
      --index;
   else
      index = sizeof...(values) - 1;
   return arr[index];
}



template<class T, T ... values>
T ConstChain<T,values...>::operator= (T val)
{
   auto it = std::find (std::begin(arr), std::end(arr), val);
   if (it != std::end(arr))
      index = it - std::begin(arr);
   return arr[index];
}