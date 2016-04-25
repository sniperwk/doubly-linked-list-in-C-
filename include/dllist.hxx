// The code here is to implement an XOR-encoded doubly-linked list.

#ifndef DLLIST_HXX
#define DLLIST_HXX

#include <algorithm>
#include <iterator>
#include <limits>
#include <initializer_list>
#include <iostream>

#include "xorptr.hxx"

template <typename T>
class dllist_node;      // doubly-linked list node type

template <typename T>
class dllist;           // doubly-linked list container type

template <typename T>
class dllist_iter;      // doubly-linked list iterator type

template <typename T>
class dllist_citer;     // doubly-linked list const_iterator type

/* dllist_node_ptr_only<T> is a class template which represents
part of the XOR-encoded doubly-linked list node. This type defines 
the "pointer" part (i.e., xorptr_). The remainder of
 the node is defined in dllist_node<T> and that type inherits
 from dllist_node_ptr_only<T> to be a complete node.
 By defining a node in two parts allows the creation
 of sentinel nodes 
*/
template <typename T>
class dllist_node_ptr_only {
public:
  using xorptr_type = xorptr<dllist_node_ptr_only>;

private:
  xorptr_type xorptr_;

public:
  // Prohibitions...
  dllist_node_ptr_only() = delete;
  dllist_node_ptr_only(dllist_node_ptr_only const&) = delete;
  dllist_node_ptr_only& operator =(dllist_node_ptr_only const&) = delete;

  // Defaults...
  ~dllist_node_ptr_only() = default;
  dllist_node_ptr_only(dllist_node_ptr_only&&) = default;
  dllist_node_ptr_only& operator =(dllist_node_ptr_only&&) = default;


  // constructor that takes prev and next and construct an
  // xorptr_ instance.
  explicit dllist_node_ptr_only(dllist_node_ptr_only* prev, dllist_node_ptr_only* next) :
    xorptr_(prev, next) {}

  // copy constructor
  explicit dllist_node_ptr_only(xorptr<dllist_node_ptr_only> const& xp) :
    xorptr_(xp) {}

  // move constructor
  explicit dllist_node_ptr_only(xorptr<dllist_node_ptr_only>&& xp) :
    xorptr_(std::move(xp)) {}


  void swap(dllist_node_ptr_only& b) {
  //#error Write the code to swap xorptr_ with b.xorptr_ using std::swap here.
    std::swap(xorptr_, b.xorptr_);
  }

  dllist_node<T>& to_node() & {
    return static_cast<dllist_node<T>&>(*this);
  }

  dllist_node<T> const& to_node() const& {
    return static_cast<dllist_node<T> const&>(*this);
  }

  dllist_node<T>&& to_node() && {
    return static_cast<dllist_node<T>&&>(*this);
  }

  dllist_node_ptr_only* nextptr(dllist_node_ptr_only* prev) {
    return xorptr_ ^ prev;
  }

  dllist_node_ptr_only const* nextptr(dllist_node_ptr_only const* prev) const {
    return xorptr_ ^ prev;
  }

  // With an XOR-encoded linked list, the xorptr<T> values are
  // computed using the previous node's address and the next node's address.
  // This means to update an xorptr<T>, one has to "subtract" the old
  // value being updated and replace it with the new value being updated.
  //
  // Because XOR is both commutative and associative, updating an xorptr<T>
  // value is simplified considerably. Given:
  //
  //   xorptr_ = a ^ b;
  //
  // and now one wants to replace 'b' with the new value of c, then:
  //
  //   xorptr_ ^ b
  //   IMPLIES (a ^ b) ^ b
  //   IMPLIES a ^ (b ^ b)
  //   IMPLIES a ^ 0
  //   IMPLIES a
  //
  // which means that one can simply XOR c into that result:
  //
  //   (xorptr_ ^ b) ^ c
  //
  // to obtain the desired result:
  //
  //   a ^ c
  //
  // xorptr_ ^ oldptr, returns a normal pointer.
  void updateptr(dllist_node_ptr_only* oldptr, dllist_node_ptr_only* newptr) {
    // compute (xorptr_ ^ oldptr ^ newptr)
    xorptr_ = xorptr<dllist_node_ptr_only>(xorptr_ ^ oldptr, newptr);
  }


  //  compute the xorptr<T> of ptr1 and ptr2
  void setptr(dllist_node_ptr_only* ptr1, dllist_node_ptr_only* ptr2) {
    xorptr_ = xorptr_type{ ptr1, ptr2 };
  }

  /* insert(prev, before, new_node) has arguments that appear in
       the ORDER that they are intended to be in the linked list.
       Moreover, insert() returns the node pointer that is after
       new_node should such be needed elsewhere.
  */
  static dllist_node_ptr_only* insert(
    dllist_node_ptr_only* prev,
    dllist_node_ptr_only* before,
    dllist_node_ptr_only* new_node
  )
  {
    // Insert newnext after this pointer as determined by prevptr...
    //    START: prev, before, oldnext, ...
    //   RESULT: prev, before, new_node, oldnext, ...

    auto oldnext = before->xorptr_ ^ prev;

    // Remove this from oldnext's ptr and add new_node...
    oldnext->updateptr(before, new_node);

    // Remove oldnext from before's ptr and add new_node...
    before->updateptr(oldnext, new_node);

    // Set new_node to before ^ oldnext...
    new_node->xorptr_ = xorptr_type{ before, oldnext };

    // Return oldnext since the remaining values are known to the caller...
    return oldnext;
  }

  
  // remove(prev, before) removes the node after before
  static dllist_node_ptr_only* remove(
    dllist_node_ptr_only* prev,
    dllist_node_ptr_only* before
  )
  {
    // Remove nextptr after this pointer as determined by prevptr.
    // For safety reasons, return the removed pointer so called
    // can delete it (since it can be a sentinel pointer).
    //    START: prev, before, oldnext, newnext
    //   RESULT: prev, before, newnext

    auto oldnext = before->xorptr_ ^ prev;

    // Obtain newnext (which will become the new next node)...
    auto newnext = oldnext->nextptr(before);

    // Remove oldnext from newnext's ptr and add this...
    newnext->updateptr(oldnext, before);

    // Remove oldnext from this' ptr and add newnext...
    before->updateptr(oldnext, newnext);

    // Clear oldnext's ptr links...
    oldnext->xorptr_ = xorptr_type{};

    // Return removed node for processing (e.g., delete)...
    return oldnext;
  }
};

//===========================================================================
// dllist_node<T>
// a private instance of type T called datum_
template <typename T>
class dllist_node final : public dllist_node_ptr_only<T> {
private:
  T datum_;
public:
  using xorptr_type = typename dllist_node_ptr_only<T>::xorptr_type;
  // Defaults...
  ~dllist_node() = default;

  //  1) "call" the parent type's constructor passing in two nullptr
  //      values since no pointer values are provided.
  //  2)  Initialize datum_ with datum using copy construction.
  explicit dllist_node(T const& datum) :
    dllist_node_ptr_only<T>(nullptr, nullptr), datum_(datum) {}

  //   1) "call" the parent type's constructor passing in two nullptr
  //      values since no pointer values are provided.
  //   2) Initialize datum_ with datum using move construction.
  explicit dllist_node(T&& datum) :
    dllist_node_ptr_only<T>(nullptr, nullptr), datum_(std::move(datum)) {}

  //   1) "call" the parent type's constructor passing prev and next values.
  //   2) Initialize datum_ with datum using copy construction.
  dllist_node(T const& datum, dllist_node* prev, dllist_node* next) :
    dllist_node_ptr_only<T>(prev, next), datum_(datum) {}


  //   1) "call" the parent type's constructor passing prev and next values.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, dllist_node* prev, dllist_node* next) :
    dllist_node_ptr_only<T>(prev, next), datum_(std::move(datum)) {}

  //   1) "call" the parent type's constructor passing xp values.
  //   2) Initialize datum_ with datum using copy construction.
  dllist_node(T const& datum, xorptr_type const& xp) :
    dllist_node_ptr_only<T>(xp), datum_(datum) {}

  //   1) "call" the parent type's constructor passing xp values.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, xorptr_type const& xp) :
    dllist_node_ptr_only<T>(xp), datum_(std::move(datum)) {}

  //   1) "call" the parent type's constructor passing xp values by moving
  //      it since it is being passed as an rvalue reference.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, xorptr_type&& xp) :
    dllist_node_ptr_only<T>(std::move(xp)), datum_(std::move(datum)) {}

  // Move constructor...
  //   1) "call" the parent type's constructor passing n values by moving
  //      it since it is being passed as an rvalue reference.
  //   2) Initialize datum_ with n's datum using move construction.
  dllist_node(dllist_node&& n) :
    dllist_node_ptr_only<T>(std::move(n)), datum_(std::move(n.datum_)) {}

  // Move assignment...
  //   1) Call the parent's class' move assignment operator by moving n.
  //   2) Move assign n's datum_ value to this->datum_.
  //   3) As this is an assignment operator, remember to return *this.
  dllist_node& operator =(dllist_node&& n) {
    dllist_node_ptr_only<T>::operator =(std::move(n));
    this->datum_ = std::move(n.datum_);
    return *this;
  }

  //   Call the parent's class' swap.
  void swap(dllist_node& b) {
    dllist_node_ptr_only<T>::swap(b);
    using std::swap;
    swap(this->datum_, b.datum_);
  }

  // "Getter" functions...
  T& datum() {
    return this->datum_;
  }
  T const& datum() const {
    return this->datum_;
  }
};

//===========================================================================

// define two global swap() overloads for the two
// node types. 
template <typename T>
inline void swap(dllist_node_ptr_only<T>& a, dllist_node_ptr_only<T>& b) {
  a.swap(b);
}

template <typename T>
inline void swap(dllist_node<T>& a, dllist_node<T>& b) {
  a.swap(b);
}

//===========================================================================

//
// dllist<T>
//
// The dllist<T> type is the xor-encoded doubly-linked list sequence
// container class. This type allows to store, erase, and access
// elements stored within it not like std::vector<T>, std::list<T>, etc.
//
template <typename T>
class dllist
{
private:
  std::size_t size_;                // length of list
  dllist_node_ptr_only<T> front_;   // sentinel "front" node
  dllist_node_ptr_only<T> back_;    // sentinel "back" node

public:
  using value_type = T;

  using reference = value_type&;
  using const_reference = value_type const&;

  using pointer = value_type*;
  using const_pointer = value_type const*;

  using iterator = dllist_iter<T>;
  using const_iterator = dllist_citer<T>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  // Default constructor...
  dllist() :
    size_(0), front_(&back_, &back_), back_(&front_, &front_) {}

  // constructor dllist(n, value)
  // This constructor will create a list of n nodes containing value as
  // their datum.
  dllist(size_type n, T const& value = T{}) : dllist<T>{} {
    std::fill_n(std::back_inserter(*this), n, value);
  }

  // constructor dllist(}{ ... })
  dllist(std::initializer_list<T> il) : dllist{} {
    std::copy(il.begin(), il.end(), std::back_inserter(*this));
  }
  // constructor dllist(first, last)
  template <typename InIter>
  dllist(InIter const& first, InIter const& last) : dllist{} {
    std::copy(first, last, std::back_inserter(*this));
  }

  // copy constructor
  dllist(dllist const& l) : dllist{} {
    std::copy(l.begin(), l.end(), std::back_inserter(*this));
  }

  // move constructor
  dllist(dllist&& l) :
    dllist{}
  {
    swap(l);
  }

  // destructor
  ~dllist() {
    while (!empty()) {
      try {
        clear();
      } catch(...) {
      }
    }
  }

  // dllist's copy assignment operator
  dllist& operator =(dllist const& l) {
    dllist<T> tmp(l);
    this->swap(tmp);
    return *this;
  }

  // move assignment operator
  dllist& operator =(dllist&& l) {
    dllist<T> tmp(std::move(l));
    this->swap(tmp);
    return *this;
  }

  // the next function implements the l.assign() call. 
  //   1) Create a local variable called tmp of type dllist<T> and pass
  //      in il to its constructor. 
  //   2) swap *this and tmp.
  void assign(std::initializer_list<T> il) {
    dllist<T> tmp(il);
    this->swap(tmp);
  }

  //   1) Create a local variable called tmp of type dllist<T> and invoke
  //      the dllist<T> constructor appropriate for the arguments n and
  //      value.
  //   2) Call this->swap() to swap *this and tmp.
  //
  void assign(size_type n, value_type const& value) {
    dllist<T> tmp(n, value);
    this->swap(tmp);
  }

  //   1) Create a local variable called tmp of type dllist<T> and invoke
  //      the dllist<T> constructor appropriate for the arguments first and
  //      last.
  //   2) Call this->swap() to swap *this and tmp.
  //
  template <typename InIter>
  void assign(InIter const& first, InIter const& last) {
    dllist<T> tmp(first, last);
    this->swap(tmp);
  }

  bool empty() const {
    return size_ == 0;
  }

  size_type size() const {
    return size_;
  }

  size_type max_size() const {
    return std::numeric_limits<size_type>::max();
  }

  reference front() {
    return front_.nextptr(&back_)->to_node().datum();
  }

  const_reference front() const {
    return front_.nextptr(&back_)->to_node().datum();
  }

  reference back() {
    return back_.nextptr(&front_)->to_node().datum();
  }

  const_reference back() const {
    return back_.nextptr(&front_)->to_node().datum();
  }

  // The constructor values to pass to iterator:
  //   1) &front_ (i.e., this is the "previous [sentinel] node",)
  //   2) front_.nextptr(&back_) (i.e., this is the "current node")
  //
  iterator begin() {
    return dllist_iter<T>(&front_, front_.nextptr(&back_));
  }

  // The constructor values to pass to iterator:
  //   1) back_.nextptr(&front_) (i.e., this is the "last" node)
  //   2) &back_ (i.e., this is the "one-past-the-end" sentinel node)
  //
  iterator end() {
    return dllist_iter<T>(back_.nextptr(&front_), &back_);
  }

  const_iterator begin() const{ 
    return dllist_citer<T>(&front_, front_.nextptr(&back_));
  }

  const_iterator end() const {
    return dllist_citer<T>(back_.nextptr(&front_), &back_);
  }

  const_iterator cbegin() const
  {
    return begin();
  }

  const_iterator cend() const
  {
    return end();
  }

  // rbegin() returns an instance of the reverse iterator adapter type
  // defined in <iterator> as std::reverse_iterator<ITERATOR_TYPE>.
  // std::reverse_iterator's constructor is passed the appropriate
  // "normal" iterator value.
  reverse_iterator rbegin() {
    return reverse_iterator(this->end());
  }

  reverse_iterator rend() {
    return reverse_iterator(this->begin());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(this->end());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(this->begin());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(this->cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(this->cbegin());
  }

  // clear() destroys all elements in the container
  void clear() {
    while (!this->empty()) {
      this->pop_back();
    }
  }

  void swap(dllist& l)
  {

    if (size_ == 0)
    {
      // Fix the front_ and back_ values...
      front_.setptr(&l.back_, &l.back_);
      back_.setptr(&l.front_, &l.front_);
    }
    else if (size_ == 1)
    {
      auto element = front_.nextptr(&back_);

      // Fix the front_ and back_ values...
      front_.setptr(&l.back_, element);
      back_.setptr(element, &l.front_);

      // Fix the element value...
      element->setptr(&l.back_, &l.front_);
    }
    else
    {
      // Obtain some values before they are changed...
      auto first = front_.nextptr(&back_);
      auto second = first->nextptr(&front_);
      auto last = back_.nextptr(&front_);
      auto before_last = last->nextptr(&back_);

      // Fix the front_ and back_ values...
      front_.setptr(&l.back_, first);
      back_.setptr(&l.front_, last);

      // Fix the first and last values...
      first->setptr(&l.front_, second);
      last->setptr(&l.back_, before_last);
    }

    if (l.size_ == 0)
    {
      l.front_.setptr(&back_, &back_);
      l.back_.setptr(&front_, &front_);
    }
    else if (l.size_ == 1)
    {
      auto l_element = l.front_.nextptr(&l.back_);

      // Fix the front_ and back_ values...
      l.front_.setptr(&back_, l_element);
      l.back_.setptr(l_element, &front_);

      // Fix the element value...
      l_element->setptr(&back_, &front_);
    }
    else
    {
      // Obtain some values before they are changed...
      auto l_first = l.front_.nextptr(&l.back_);
      auto l_second = l_first->nextptr(&l.front_);
      auto l_last = l.back_.nextptr(&l.front_);
      auto l_before_last = l_last->nextptr(&l.back_);

      // Fix the front_ and back_ values...
      l.front_.setptr(&back_, l_first);
      l.back_.setptr(&front_, l_last);

      // Fix the first and last values...
      l_first->setptr(&front_, l_second);
      l_last->setptr(&back_, l_before_last);
    }

    // swap the values...
    front_.swap(l.front_);
    back_.swap(l.back_);

    // swap sizes...
    std::swap(size_, l.size_);
  }

  void push_front(value_type const& v) {
    dllist_node<T>::insert(&back_, &front_, new dllist_node<value_type>(v));
    ++size_;
  }

  void push_front(value_type&& v) {
    dllist_node<T>::insert(&back_, &front_, new dllist_node<value_type>(std::move(v)));
    ++size_;
  }

  void pop_front() {
    auto old = dllist_node<T>::remove(&back_, &front_);
    delete old;
    --size_;
  }

  void push_back(value_type const& v) {
    dllist_node<T>::insert(&front_, &back_, new dllist_node<value_type>(v));
    ++size_;
  }
  void push_back(value_type&& v) {
    dllist_node<T>::insert(&front_, &back_, new dllist_node<value_type>(std::move(v)));
    ++size_;
  }
  void pop_back() {
    auto old = dllist_node<T>::remove(&front_, &back_);
    delete old;
    --size_;
  }

  template <typename... Args>
  iterator emplace(iterator pos, Args&&... args) {
    --pos;
    auto newnode = new dllist_node<T>{T(std::forward<Args>(args)...)};
    auto nextnode = dllist_node<T>::insert(pos.prevptr_, pos.nodeptr_, newnode);
    ++size_;
    return iterator(newnode, nextnode);
  }

  template <typename... Args>
  void emplace_front(Args&&... args) {
    this->emplace(begin(), std::forward<Args>(args)...);
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    this->emplace(end(), std::forward<Args>(args)...);
  }

  iterator insert(iterator pos, value_type const& value) {
    --pos;
    auto newnode = new dllist_node<T>(value);
    auto nextnode = dllist_node<T>::insert(pos.prevptr_, pos.nodeptr_, newnode);
    ++size_;
    return pos;
  }

  template <typename InIter>
  void insert(iterator pos, InIter first, InIter const& last) {
    for (; first != last; ++first) {
      pos = insert(pos, *first);
    }
  }

  iterator erase(iterator pos) {
    --pos;
    auto oldnode = dllist_node<T>::remove(pos.prevptr_, pos.nodeptr_);
    delete oldnode;
    ++pos;
    --size_;
    return pos;
  }

  iterator erase(iterator first, iterator const& last) {
    while (first != last)  {
      first = erase(first);
    }
    return first;
  }
};

//===========================================================================

template <typename T>
inline void swap(dllist<T>& a, dllist<T>& b)
{
  a.swap(b);
}

template <typename T>
inline bool operator ==(dllist<T> const& a, dllist<T> const& b) {
  return std::equal(a, b);
}

template <typename T>
inline bool operator !=(dllist<T> const& a, dllist<T> const& b)
{
  return !(operator ==(a,b));
}

template <typename T>
inline bool operator <(dllist<T> const& a, dllist<T> const& b) {
  return std::lexicographical_compare(a, b);
}

template <typename T>
inline bool operator <=(dllist<T> const& a, dllist<T> const& b)
{
  return !(a > b);
}

template <typename T>
inline bool operator >=(dllist<T> const& a, dllist<T> const& b)
{
  return !(a < b);
}

template <typename T>
inline bool operator >(dllist<T> const& a, dllist<T> const& b)
{
  return b < a;
}

template <typename T>
inline auto begin(dllist<T>& a)
{
  return a.begin();
}

template <typename T>
inline auto begin(dllist<T> const& a)
{
  return a.begin();
}

template <typename T>
inline auto cbegin(dllist<T> const& a)
{
  return a.begin();
}

template <typename T>
inline auto rbegin(dllist<T>& a)
{
  return a.rbegin();
}

template <typename T>
inline auto rbegin(dllist<T> const& a)
{
  return a.rbegin();
}

template <typename T>
inline auto crbegin(dllist<T> const& a)
{
  return a.crbegin();
}

template <typename T>
inline auto end(dllist<T>& a)
{
  return a.end();
}

template <typename T>
inline auto end(dllist<T> const& a)
{
  return a.end();
}

template <typename T>
inline auto cend(dllist<T> const& a)
{
  return a.end();
}

template <typename T>
inline auto rend(dllist<T>& a)
{
  return a.rend();
}

template <typename T>
inline auto rend(dllist<T> const& a)
{
  return a.rend();
}

template <typename T>
inline auto crend(dllist<T> const& a)
{
  return a.crend();
}

//===========================================================================

//
// dllist_iter<T>
//
// The dllist_iter<T> defines the non-const T iterator type 
template <typename T>
class dllist_iter : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T&> {
private:
  friend class dllist<T>;
  friend class dllist_citer<T>;

  dllist_node_ptr_only<T>* prevptr_; // Used to compute next node address
  dllist_node_ptr_only<T>* nodeptr_; // Cur node; for xorptr_ value

public:
  // The default constructor simply initializes prevptr_ and nodeptr_
  // to nullptr. 
  dllist_iter() :
    prevptr_{nullptr}, nodeptr_{nullptr} {}

  dllist_iter(dllist_iter const&) = default;
  dllist_iter& operator =(dllist_iter const&) = default;

  dllist_iter(dllist_iter&) = default;
  dllist_iter& operator =(dllist_iter&) = default;

  ~dllist_iter() = default;

  dllist_iter(dllist_node_ptr_only<T>* prev, dllist_node_ptr_only<T>* xornode) :
    prevptr_{prev}, nodeptr_{xornode} {}

  bool operator ==(dllist_iter const& i) const {
    return nodeptr_ == i.nodeptr_;
  }

  bool operator !=(dllist_iter const& i) const {
    return !(operator==(i));
  }

  // unary *, i.e., the indirection (pointer) operator.
  T& operator *() const {
    return nodeptr_->to_node().datum();
  }

  //  -> (indirection) operator which must return a T*.
  T* operator ->() const {
    return &nodeptr_->to_node().datum();
  }

  dllist_iter& operator ++() {
    auto next_nodeptr_ = nodeptr_->nextptr(prevptr_);
    prevptr_ = nodeptr_;
    nodeptr_ = next_nodeptr_;
    return *this;
  }

  dllist_iter operator ++(int) {
    dllist_iter<T> tmp(*this);
    operator++();
    return tmp;
  }

  dllist_iter& operator --() {
    auto prev_prevptr_ = prevptr_->nextptr(nodeptr_);
    nodeptr_ = prevptr_;
    prevptr_ = prev_prevptr_;
    return *this;
  }

  dllist_iter operator --(int) {
    dllist_iter<T> tmp(*this);
    operator--();
    return tmp;
  }
};

//===========================================================================
//
// dllist_citer
//
// T is const and therefore return values, etc. are const.
template <typename T>
class dllist_citer : public std::iterator<std::bidirectional_iterator_tag, T const, std::ptrdiff_t, T const*, T const&> {
private:
  friend class dllist<T>;

  dllist_node_ptr_only<T> const* prevptr_; // Used to compute next node address
  dllist_node_ptr_only<T> const* nodeptr_; // Cur node; for xorptr_ value

public:
  dllist_citer() :
    prevptr_{nullptr}, nodeptr_{nullptr} {}

  dllist_citer(dllist_citer const&) = default;
  dllist_citer& operator =(dllist_citer const&) = default;

  dllist_citer(dllist_citer&) = default;
  dllist_citer& operator =(dllist_citer&) = default;

  ~dllist_citer() = default;

  dllist_citer(dllist_node_ptr_only<T> const* prev, dllist_node_ptr_only<T> const* xornode) :
    prevptr_{prev}, nodeptr_{xornode} {}

  dllist_citer(dllist_iter<T> const& i) :
    nodeptr_{i.nodeptr_}, prevptr_{i.prevptr_} {}

  dllist_citer& operator =(dllist_iter<T> const& i) {
    nodeptr_ = i.nodeptr_;
    prevptr_ = i.prevptr_;
    return *this;
  }

  bool operator ==(dllist_iter<T> const& i) const {
    return nodeptr_ == i.nodeptr_;
  }

  bool operator !=(dllist_iter<T> const& i) const {
    return !operator ==(i);
  }

  bool operator ==(dllist_citer const& i) const {
    return nodeptr_ == i.nodeptr_;
  }

  bool operator !=(dllist_citer const& i) const {
    return !operator ==(i);
  }

  T const& operator *() const {
    return nodeptr_->to_node().datum();
  }

  T const* operator ->() const {
    return &nodeptr_->to_node().datum();
  }

  dllist_citer& operator ++() {
    auto next_nodeptr_ = nodeptr_->nextptr(prevptr_);
    prevptr_ = nodeptr_;
    nodeptr_ = next_nodeptr_;
    return *this;
  }

  dllist_citer operator ++(int) {
    dllist_citer<T> tmp(*this);
    operator++();
    return tmp;
  }

  dllist_citer& operator --() {
    auto prev_prevptr_ = prevptr_->nextptr(nodeptr_);
    nodeptr_ = prevptr_;
    prevptr_ = prev_prevptr_;
    return *this;
  }

  dllist_citer operator --(int) {
    dllist_citer<T> tmp(*this);
    operator--();
    return tmp;
  }
};

//===========================================================================

template <typename T>
inline bool operator ==(dllist_iter<T> const& i, dllist_citer<T> const& j)
{
  return j == i;
}

template <typename T>
inline bool operator !=(dllist_iter<T> const& i, dllist_citer<T> const& j)
{
  return j != i;
}


#endif // #ifndef DLLIST_HXX
