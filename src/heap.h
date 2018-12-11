// необходимо понимать, что в микромире работать с кучей надо особенно аккуратно
// постоянные выделения/освобождения памяти могут привести к дефрагментации
// от этого легко обезопаситься, если применять new к объектам
// , которые никогда не удаляются, а это практически все объекты
// , которые работают с переферией, инициализируются на старте
// и перестают работать только, когда пропадает питание

// Для собственного оператора нужна собственная куча
// самый простой способ, выделить память в самом начале в виде массива
// и выдавать указатель оператору, контролируя переполнение этого массива
// Способ посложнее, который не рассматриваю пока, прописать кучу в линкере
template<size_t heap_size>
class Heap
{
   uint8_t memory[heap_size];
   size_t free_index {0};
public:
   void* allocate (size_t size)
   {
      void* p = reinterpret_cast<void*>(memory + free_index);
      free_index += size;
      // если нам необходимо памяти больше, чем выделено, то тут будет HardFault
      // что определиться при первой же отладке
      return (free_index < heap_size) ? p : nullptr;
   }
};

// выделяем память под динамические объекты
constexpr size_t heap_size {256};
Heap<heap_size> heap {};

// собственный глобальный оператор new, который работает с нашей кучей
void* operator new (size_t size)
{
   return heap.allocate(size);
}