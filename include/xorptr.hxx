#ifndef XORPTR_HXX
#define XORPTR_HXX


#if defined CXX_XOR_PROJECT_USE_REINTERPRET_CAST && defined CXX_XOR_PROJECT_USE_UINTPTR_T
#error "Only define one CXX_XOR_PROJECT_USE_XXX macro!"
#elif !defined CXX_XOR_PROJECT_USE_REINTERPRET_CAST  && !defined CXX_XOR_PROJECT_USE_UINTPTR_T
#error "Define one of the CXX_XOR_PROJECT_USE_XXX macros!"
#endif

#ifdef CXX_XOR_PROJECT_USE_UINTPTR_T 
#define xorptr_traits_defined true
template <typename T>
struct xorptr_traits final {
  using pointer_type = T*;
  using const_pointer_type = T const*;
  using xorptr_type = std::uintptr_t;

  static xorptr_type create() noexcept {
    return create(static_cast<T*>(nullptr), static_cast<T*>(nullptr));
  }

  static xorptr_type create(pointer_type p1, pointer_type p2) noexcept {
    return reinterpret_cast<std::uintptr_t>(p1) ^ reinterpret_cast<std::uintptr_t>(p2);
  }

  static xorptr_type create(const_pointer_type p1, const_pointer_type p2) noexcept {
    return reinterpret_cast<std::uintptr_t>(p1) ^ reinterpret_cast<std::uintptr_t>(p2);
  }

  static pointer_type extract(xorptr_type const& xp, pointer_type p) noexcept {
    return static_cast<T*>(reinterpret_cast<void*>(xp ^ reinterpret_cast<std::uintptr_t>(p)));
  }

  static const_pointer_type extract(xorptr_type const& xp, const_pointer_type p) noexcept {
    return static_cast<T const*>(reinterpret_cast<void const*>(xp ^ reinterpret_cast<std::uintptr_t>(p)));
  }
};
#endif


#ifdef CXX_XOR_PROJECT_USE_REINTERPRET_CAST
#define xorptr_traits_defined true

template <typename T>
struct xorptr_traits final {
  using pointer_type = T*;
  using const_pointer_type = T const*;
  using xorptr_type = void*;
  // Create an XOR-encoded pointer value of two nullptr values...
  static xorptr_type create() noexcept {
    return create(static_cast<T*>(nullptr), static_cast<T*>(nullptr));
  }
  // Create an XOR-encoded pointer value of two non-const pointer values...
  static xorptr_type create(pointer_type p1, pointer_type p2) noexcept {
    using ucharptr = unsigned char*;
    xorptr_type volatile vp1 = p1;
    xorptr_type volatile vp2 = p2;
    ucharptr ucp1 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp1)));
    ucharptr ucp2 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp2)));
    for (int i = 0; i != sizeof(xorptr_type); ++i) {
      ucp1[i] ^= ucp2[i];
    }
    return vp1;
  }
  // Create an XOR-encoded pointer value of two const pointer values...
  static xorptr_type create(const_pointer_type p1, const_pointer_type p2) noexcept {
    using ucharptr = unsigned char*;
    void const* volatile vp1 = p1;
    void const* volatile vp2 = p2;
    ucharptr ucp1 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp1)));
    ucharptr ucp2 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp2)));
    for (int i = 0; i != sizeof(xorptr_type); ++i) {
      ucp1[i] ^= ucp2[i];
    }
    return const_cast<xorptr_type>(vp1);
  }
  // Compute the XOR of an XOR-encoded pointer with a normal pointer.
  // NOTE: The result is a normal pointer. See cast hints below.
  static pointer_type extract(xorptr_type const& xp, pointer_type p) noexcept {
    using ucharptr = unsigned char*;
    xorptr_type volatile vp1 = xp;
    xorptr_type volatile vp2 = p;
    ucharptr ucp1 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp1)));
    ucharptr ucp2 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp2)));
    for (int i = 0; i != sizeof(xorptr_type); ++i) {
      ucp1[i] ^= ucp2[i];
    }
    return static_cast<pointer_type>(vp1);
  }
  // Compute the XOR of an XOR-encoded pointer with a const pointer..
  // NOTE: The result is a const pointer. See cast hints below.
  static const_pointer_type extract(xorptr_type const& xp, const_pointer_type p) noexcept {
    using ucharptr = unsigned char*;
    void const* volatile vp1 = xp;
    void const* volatile vp2 = p;
    ucharptr ucp1 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp1)));
    ucharptr ucp2 = static_cast<ucharptr>(reinterpret_cast<xorptr_type>(const_cast<void **>(&vp2)));
    for (int i = 0; i != sizeof(xorptr_type); ++i) {
      ucp1[i] ^= ucp2[i];
    }
    return static_cast<const_pointer_type>(vp1);
  }
};
#endif

#ifdef xorptr_traits_defined
template <typename T>
class xorptr final {
public:
  using traits_type = xorptr_traits<T>;
  using pointer_type = typename traits_type::pointer_type;
  using const_pointer_type = typename traits_type::const_pointer_type;
private:
  using xorptr_type = typename traits_type::xorptr_type;
  xorptr_type xorptr_;
public:
  xorptr() noexcept : 
    xorptr_(traits_type::create()) {}
  explicit xorptr(pointer_type ptr1, pointer_type ptr2) noexcept : 
    xorptr_(traits_type::create(ptr1, ptr2)) {}
  explicit xorptr(const_pointer_type ptr1, const_pointer_type ptr2) noexcept : 
    xorptr_(traits_type::create(ptr1, ptr2)) {}
  xorptr(xorptr const& xp) noexcept = default;
  xorptr(xorptr&& xp) noexcept = default;
  xorptr& operator =(xorptr const& xp) noexcept = default;
  xorptr& operator =(xorptr&& xp) noexcept = default;
  ~xorptr() noexcept = default;
  pointer_type operator ^(pointer_type& b) const noexcept {
    return traits_type::extract(xorptr_, b);
  }
  const_pointer_type operator ^(const_pointer_type& b) const noexcept {
    return traits_type::extract(xorptr_, b);
  }
};
#endif

#endif // #ifndef XORPTR_HXX
