//
// dllist.hxx-provided
//
// This file has been provided as a template for you to do the group
// portion of your project. You will find instructions, hints/tips, etc.
// below as comments in the source code.
//
// The objective of the code in this file is to implement an ISO C++
// compliant sequence container (for the operations defined herein)
// that implements an XOR-encoded doubly-linked list.
//
// The xorptr.hxx header file is from the individual portion of the
// project which each group member is required to do. Once such has been
// handed-in, pool together your xorptr.hxx code and take the best of, or,
// improve it (should there be bugs in the individual ones) if needed for
// use in your group project.
//
// There is no "test" input or code for this project. You all are well
// aware of std::list<T> and bidirection iterator use in C++. You are to
// write your own test code in a .cxx file (i.e., don't do it in this
// header file!) to test whether or not your code is correct, etc.
//
// Remember that when we mark your project we will #include "dllist.hxx"
// (one or more times) from various .cxx files of our own making --so
// ensure that what you write is correct, is tested, and works! Do ask
// questions or for help when such is needed!
//
// While some preprocessor items may be missing from this file that you
// need to add to it, the rest of the "prototyped" code is present and
// you must implement/fix all of that code. The content of this file
// was based on real, correct code and the comments that appear in this
// file were written based upon such. This was done to ensure everything
// is doable, etc. (Moreover the teaching assistants are aware of the
// solutions needed for this file as well.)
//
// NOTE: Do read the Project Details & Information Document which
//       explains amongst other things the purpose of the types in this
//       file.
//

#ifndef DLLIST_HXX
#define DLLIST_HXX

// YOU NEED SOMETHING HERE WRT THE PREPROCESSOR

//===========================================================================

// WRITE YOUR #include FILES HERE

#include <algorithm>
#include <iterator>
#include <limits>
#include <initializer_list>
#include <iostream>

// Clearly you will need xorptr.hxx and I've provided it. Normally for
// your own #include files you will use "" instead of <>.
// NOTE:You should always write your own #include files after system ones.
#include "xorptr.hxx"

//===========================================================================

// NOTE: Sometimes you need to use type names before they've been defined.
//       This is okay as long as the compiler doesn't need their
//       definitions, e.g., you only use pointers to the type. You can
//       let a C/C++ compiler know of a type name before it is defined
//       by writing a forward declaration. Here are some forward
//       declarations of class templates in this file.

//
// Forward declare types...
//
template <typename T>
class dllist_node;      // doubly-linked list node type

template <typename T>
class dllist;           // doubly-linked list container type

template <typename T>
class dllist_iter;      // doubly-linked list iterator type

template <typename T>
class dllist_citer;     // doubly-linked list const_iterator type

//===========================================================================

//
// dllist_node_ptr_only<T>
//
// A dllist_node_ptr_only<T> is a class template that can be thought of
// as "half" of the XOR-encoded doubly-linked list node. Specifically,
// this type defines the "pointer" half (i.e., xorptr_). The remainder of
// the node is defined in dllist_node<T> below and that type inherits
// from dllist_node_ptr_only<T> to be a complete node.
//
// The reason for defining a node in two parts is to allow the creation
// of sentinel nodes (i.e., nodes with the pointer-linking ability but
// without any data) that can be stored in the container class (i.e.,
// dllist<T> in this file).
//
// The code in this class is very straight-forward since you've
// already written xorptr<T>. Follow the instructions/comments that
// appear below to finish writing this class.
//
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

  // Items to write/do...

  // The following constructor must take prev and next and construct an
  // xorptr_ instance. You must do this using the C++ constructor
  // initializer list syntax (i.e., using the : ... { syntax). There must
  // be no code within the braces of the constructor body.
  explicit dllist_node_ptr_only(dllist_node_ptr_only* prev, dllist_node_ptr_only* next) :
    xorptr_(prev, next) {}

  // The following two constructors must initialize xorptr_ with xp passing
  // xp appropriately (i.e., an rvalue must be moved). No code is allowed
  // to appear within the braces of the constructor body.
  explicit dllist_node_ptr_only(xorptr<dllist_node_ptr_only> const& xp) :
    xorptr_(xp) {}
  explicit dllist_node_ptr_only(xorptr<dllist_node_ptr_only>&& xp) :
    xorptr_(std::move(xp)) {}


  // It is common in C++ for classes to define a swap function to swap
  // *this and its argument (e.g., b here). Unless there is something
  // special about how something is swapped, usually the code in such
  // swap functions simple invokes std::swap.
  void swap(dllist_node_ptr_only& b) {
  //#error Write the code to swap xorptr_ with b.xorptr_ using std::swap here.
    std::swap(xorptr_, b.xorptr_);
  }

  // Handy casts...
  //
  // Per the Project Details and Information Document and from the
  // lectures, if one can give a 100% guarantee the downcasted type
  // is ALWAYS a specific type, then one does not have to use dynamic_cast
  // to cast a type to those types: one can use a static_cast instead.
  // Unlike dynamic_cast which uses Run-Time-Type-Information (RTTI)
  // at run-time to determine whether or not the cast is safe to do, one
  // can use static_cast to do this at compile-time. That said, know that
  // this is inherently UNSAFE unless one can ABSOLUTELY guarantee without
  // fail that the type casted to is indeed actually what exists.
  //
  // Since this is new to you, some of this code has been provided.
  //
  // Also notice the &, const&, and && after the closing parentheses.
  // Just as with "const", this refers to the type that the this pointer
  // POINTS TO. So & means "*this" object is an lvalue, const& means
  // "this" object is const&, and && means "*this" is an rvalue. :-)
  //
  // IMPORTANT: to_node() ASSUMES that the actual type of *this is
  //            of type dllist_node<T>. If it is invoked on an object
  //            it must be! (If not, then the behaviour is undefined.)
  //
  // Due to the seriousness of these functions, they've been defined
  // for you.
  //
  dllist_node<T>& to_node() & {
    return static_cast<dllist_node<T>&>(*this);
  }

  dllist_node<T> const& to_node() const& {
    return static_cast<dllist_node<T> const&>(*this);
  }

  dllist_node<T>&& to_node() && {
    return static_cast<dllist_node<T>&&>(*this);
  }

  //
  // XOR-encoded pointer manipulations...
  //
  // The nextptr(prev) function computes xorptr_ ^ prev and returns
  // the result. While the single line of code in both functions
  // is identical, notice the difference in type in prev and in terms of
  // *this (i.e., the first *this is non-const and the second *this
  // is const). This will cause the various overloads defined in xorptr.hxx
  // to to be used --so technically the code is not identical in terms
  // of types in the C++ source code. (This is, in part, why those
  // overloads were written.
  //
  // NOTE: This function in terms of a normal linked list would be
  //       conceptually equivalent to BOTH of these functions:
  //
  //       node* nextptr(node* prevNode) { return prevNode->next; }
  //       node* prevptr(node* nextNode) { return nextNode->prev; }
  //
  //       Why & how "both"? Answer: Re-examine how one traverses an
  //       XOR-encoded list.
  //
  dllist_node_ptr_only* nextptr(dllist_node_ptr_only* prev) {
    // write the single line of code to compute the XOR of xorptr_ and prev
    return xorptr_ ^ prev;
  }

  dllist_node_ptr_only const* nextptr(dllist_node_ptr_only const* prev) const {
    // write the single line of code to compute the XOR of xorptr_ and prev
    return xorptr_ ^ prev;
  }

  //
  // When one performs an insertion or removal of a node in a linked
  // list one needs to update the various nodes. Normally one simply
  // assigns the required pointer(s) their new values and that is all.
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
  // Unlike nextptr(prev), updateptr(oldptr, newptr) cannot just be
  // xorptr_ ^ oldptr ^ newptr. The xorptr_ class was designed to be
  // as reasonably type-/use-safe as possible. This means that if one
  // computes the XOR-encoded pointer, xorptr_, then XORing a pointer,
  // e.g., xorptr_ ^ oldptr, returns a normal pointer. (The safety
  // assumption is that one is XORing the correct pointer value with
  // xorptr_ so that the result is the correct pointer.)
  //
  // Since xorptr_ ^ oldptr returns a normal pointer, one cannot merely
  // write xorptr_ ^ oldptr ^ newptr and you are not allowed to add
  // additional functions to xorptr_ or to call xorptr_traits<T>
  // functions to achieve this! Instead, here you are to ONLY use the
  // xorptr<T> type's member functions and operations (e.g., ^) to
  // perform the correct XOR.
  //
  // HINTS:
  //   - Look at the constructors of xorptr<T>. There is nothing wrong
  //     with creating xorptr<T> rvalue(s) in the code written.
  //   - The code is a SINGLE line of code.
  //   - Remember to assign this->xorptr_ the result of the computation.
  //
  void updateptr(dllist_node_ptr_only* oldptr, dllist_node_ptr_only* newptr) {
    /* compute (xorptr_ ^ oldptr ^ newptr) here as detailed above */
    xorptr_ = xorptr<dllist_node_ptr_only>(xorptr_ ^ oldptr, newptr);
  }


  //
  // In dllist<T>::swap(dllist<T>&) you'll notice the code is (i) provided
  // and (ii) is long. While strictly not necessary because
  // updateptr(oldptr, newptr) can be used to update an xorptr<T> value
  // if one wants to update BOTH pointer values stored in an xorptr<T>
  // the it is sometimes easier (and less computation) to provide
  // both pointer values, ptr1 and ptr2, and compute the XOR of them.
  //
  // Write the SINGLE line of code to:
  //   1) compute the xorptr<T> of ptr1 and ptr2 (hint: think rvalue
  //      construction), and,
  //   2) assign the result to xorptr_.
  //
  void setptr(dllist_node_ptr_only* ptr1, dllist_node_ptr_only* ptr2) {
    xorptr_ = xorptr_type{ ptr1, ptr2 };
  }

  //
  // Normally in a traditionally doubly-linked list, one might write
  // both an insert_before() and an insert_after() function for
  // convenience reasons. In an XOR-encoded list, this is unnecessary
  // and the properties of "unlocking" the XOR-encoded pointer values
  // means one has to follow a traversal order to insert() a node
  // into the next position in the list.
  //
  // Although insert(prev, before, new_node) is line-for-line conceptually
  // equivalent to a normal linked list insert, many find this confusing
  // to implement with XOR-encoded pointers. (Why? It is easy to have
  // your head spinning in "circles". I recommend writing down numbers
  // as addresses on paper for a linked list on paper in order to figure
  // things out. Once one understands that, then the code in this function
  // is straight-forward to understand and write.)
  //
  // As this course is about C++ and related concepts and not data
  // structures and algorithms, this code has been provided to you. The
  // comments should suffice to understand it fully.
  //
  // NOTE: insert(prev, before, new_node) has arguments that appear in
  //       the ORDER that they are intended to be in the linked list.
  //       Moreover, insert() returns the node pointer that is after
  //       new_node should such be needed elsewhere.
  //
  // NOTE: insert() is a static function. This was to have its arguments
  //       be in the order they are in the actual list. This helps the
  //       human programmer know their code is correct, i.e., if they
  //       want to insert a node after before, then need the prev node to
  //       get the node after before (i.e., oldnext), in order to add
  //       new_node after the before node. :-)
  //
  // NOTE: DO NOT ALTER THIS FUNCTION IN ANY WAY. IT IS CORRECT!
  //
  static dllist_node_ptr_only* insert(
    dllist_node_ptr_only* prev,
    dllist_node_ptr_only* before,
    dllist_node_ptr_only* new_node
  )
  {
    // Insert newnext after this pointer as determined by prevptr...
    //    START: prev, before, oldnext, ...
    //   RESULT: prev, before, new_node, oldnext, ...

    // Obtain oldnext...
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

  //
  // remove(prev, before) is a static function for the same reason
  // as insert(prev, before, new_node), i.e., to remove the node after
  // before, one needs the node previous to before (i.e., prev) to obtain
  // the node after before --and to avoid confusion for the human
  // programmer one writes the arguments in the order of list traversal. :-)
  //
  // Although one likely wants to delete the old node, that node might be
  // a sentinel node, or, the caller may actually want to insert the node
  // into another position in the list, or, into a completely different
  // list, so it is better to remove the node and let the caller determine
  // what to do with it.
  //
  // ASIDE: For similar reasons, the insert() function does not allocate
  //        the node --the caller provides it.
  //
  // NOTE: DO NOT ALTER THIS FUNCTION IN ANY WAY. IT IS CORRECT!
  //
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

    // Obtain oldnext (which will be deleted)...
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

//
// dllist_node<T>
//
// Having nodes in a list that don't store any data wouldn't be of must
// use. Except for the sentinal nodes defined in dllist<T>, all nodes
// in dllist<T> are actually of type dllist_node<T>.
//
// You need to make the dllist_node<T> class:
//
//   1) final
//   2) inherit publicly (but not virtually) from dllist_node_ptr_only<T>
//   3) declare a private instance of type T called datum_
//   4) declare a public type alias called "xorptr_type" that is of
//      type dllist_node_ptr_only<T>::xorptr_type.
//
// and follow the instructions/comments inside the class and do those
// tasks accordingly.
//
template <typename T>
class dllist_node final : public dllist_node_ptr_only<T> {
private:
  T datum_;
public:
  using xorptr_type = typename dllist_node_ptr_only<T>::xorptr_type;
  // Defaults...
  ~dllist_node() = default;

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing in two nullptr
  //      values since no pointer values are provided.
  //   2) Initialize datum_ with datum using copy construction.
  explicit dllist_node(T const& datum) :
    dllist_node_ptr_only<T>(nullptr, nullptr), datum_(datum) {}

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing in two nullptr
  //      values since no pointer values are provided.
  //   2) Initialize datum_ with datum using move construction.
  explicit dllist_node(T&& datum) :
    dllist_node_ptr_only<T>(nullptr, nullptr), datum_(std::move(datum)) {}

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing prev and next values.
  //   2) Initialize datum_ with datum using copy construction.
  dllist_node(T const& datum, dllist_node* prev, dllist_node* next) :
    dllist_node_ptr_only<T>(prev, next), datum_(datum) {}


  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing prev and next values.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, dllist_node* prev, dllist_node* next) :
    dllist_node_ptr_only<T>(prev, next), datum_(std::move(datum)) {}

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing xp values.
  //   2) Initialize datum_ with datum using copy construction.
  dllist_node(T const& datum, xorptr_type const& xp) :
    dllist_node_ptr_only<T>(xp), datum_(datum) {}

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing xp values.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, xorptr_type const& xp) :
    dllist_node_ptr_only<T>(xp), datum_(std::move(datum)) {}

  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing xp values by moving
  //      it since it is being passed as an rvalue reference.
  //   2) Initialize datum_ with datum using move construction.
  dllist_node(T&& datum, xorptr_type&& xp) :
    dllist_node_ptr_only<T>(std::move(xp)), datum_(std::move(datum)) {}

  // Move constructor...
  // No code can appear inside the next constructor's function body (braces).
  // Do initialize the class as follows before the opening brace:
  //   1) "call" the parent type's constructor passing n values by moving
  //      it since it is being passed as an rvalue reference.
  //   2) Initialize datum_ with n's datum using move construction.
  dllist_node(dllist_node&& n) :
    dllist_node_ptr_only<T>(std::move(n)), datum_(std::move(n.datum_)) {}

  // Move assignment...
  // Implement the move assignment operator as follows:
  //   1) Call the parent's class' move assignment operator by moving n.
  //      HINT: You must call the parent function by writing the full
  //            typename of the parent class, writing "::operator =(", and
  //            then specifying the move of n, followed by ");".
  //   2) Move assign n's datum_ value to this->datum_.
  //   3) As this is an assignment operator, remember to return *this.
  dllist_node& operator =(dllist_node&& n) {
    // I don't know what is happening
    dllist_node_ptr_only<T>::operator =(std::move(n));
    this->datum_ = std::move(n.datum_);
    return *this;
  }

  // swap...
  // Do the following:
  //   1) Call the parent's class' swap.
  //      HINT: In the move assignment above, you wrote the FULL name of the
  //            parent class' function to call it. You must do the same
  //            here. (In Java, you can use "super" to do this. In C++
  //            there is no "super" since C++ supports multiple
  //            inheritance (and what would "super" mean then?). Instead
  //            in C++, you write the full name of the parent class to
  //            "access" something in that class.)
  //   2) Write using std::swap;
  //   3) Now call swap to swap this->datum_ and b's datum_.
  //      NOTE: Step 2 should not be combined with step 3 here. C++ supports
  //            something called ADL (argument dependent lookup). If
  //            type T is in some namespace N and T has a swap(T,T) function
  //            defined in the N namespace, then the C++ compiler will
  //            find and use the N::swap(T,T). If you hard-code std:: in
  //            front of the swap call, then the C++ compiler will ONLY call
  //            std::swap(T,T) and not the more optimal N::swap(T,T).
  //            Moreover, if there is no N::swap(T,T), know that step 2
  //            ensures the compiler will call std::swap(T,T) as it should.
  //
  //            LESSON: Sometimes it is good not to over-specify things.
  //
  void swap(dllist_node& b) {
    dllist_node_ptr_only<T>::swap(b);
    using std::swap;
    swap(this->datum_, b.datum_);
  }

  //
  // "Getter" functions...
  //
  // In Java, you often wrote getXXX() functions. The following two
  // datum() functions are exactly this. Each is a single line, returning
  // this->datum_. Write these.
  //
  T& datum() {
    return this->datum_;
  }
  T const& datum() const {
    return this->datum_;
  }
};

//===========================================================================

// For convenience, define two global swap() overloads for the two
// node types. Both of these have a single line of code in them which
// invokes a's swap() method passing in b to that call.
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
// IMPORTANT: The constructors and functions in this class are the same
//            as the constructors and functions in the ISO C++ 14 standard
//            for a sequence container, e.g., std::list. So if there is
//            any doubt as to what the function should do, besides asking
//            for help/clarification, you can also consult the appropriate
//            C++ document (e.g., for std::list).
//
// Follow the instructions / comments below to complete this class.
//
template <typename T>
class dllist
{
private:
  std::size_t size_;                // length of list
  dllist_node_ptr_only<T> front_;   // sentinel "front" node
  dllist_node_ptr_only<T> back_;    // sentinel "back" node

public:
  // All C++ containers define these type aliases. (They have been provided
  // to you so you can see how this is done. Notice the actual iterator
  // class names are "hidden" being the iterator and const_iterator type
  // aliases --which makes using C++ containers MUCH easier. Also notice
  // the special "reverse_iterator" type.)
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

  //
  // Default constructor...
  // No code can appear inside the next constructor's function body (braces).
  // Initialize the class as follows before the opening brace:
  //   1) Explictly default initialize size_ (which sets its value to zero).
  //   2) Initialize front_ by passing in &back_ twice.
  //   3) Initialize back_ by passing in &front_ twice.
  //
  // NOTE: front_'s initial value is the XOR of &back_ with &back_ whose
  //       result is zero. back_'s initial value is the XOR of &front_
  //       whose result is also zero. Even though they are initially zero,
  //       you are required to explicitly pass this in here.
  //
  // NOTE: The reason you can use &back_ before front_ is initialized is
  //       because the compiler has already seen (above!) the variable
  //       declaration of back_ so it can pass the appropriate address
  //       in to initialize front_.
  //
  dllist() :
    size_(0), front_(&back_, &back_), back_(&front_, &front_) {}

  //
  // constructor dllist(n, value)
  // This constructor will create a list of n nodes containing value as
  // their datum.
  //
  // Initialize the class as follows before the opening brace:
  //   1) Explicitly call the dllist<T> default constructor.
  //
  // Initialize the class as follows after the opening brace:
  //   1) Call <algorithm>'s fill_n() function passing in
  //      a) <iterator>'s back_inserter() function on *this list
  //      b) n
  //      c) value
  //
  // NOTE: The reason you can use back_inserter like this is because *this
  //       is already fully constructed by first calling the dllist<T>
  //       default constructor!
  //
  // Notice that if value has a default value --a default constructed T!
  // This is handy if you want a list of n default values. :-)
  //
  dllist(size_type n, T const& value = T{}) : dllist<T>{} {
    std::fill_n(std::back_inserter(*this), n, value);
  }

  //
  // constructor dllist(}{ ... })
  //
  // You've been using code like this all semester:
  //
  //   std::list<int> mylist( { 1, -5, 4 } );
  //
  // The { 1, -5, 4 } is of type std::initializer_list<int> and that type
  // is defined in <initializer_list> (i.e., you will need to add this
  // #include at the top of this file!).
  //
  // This constructor will provide the ability for one to do the same
  // with dllist<T>:
  //
  //   dllist<int> mylist( { 1, -5, 4 } );
  //
  // Initialize the class as follows before the opening brace:
  //   1) Explicitly call the dllist<T> default constructor.
  //
  // Initialize the class as follows after the opening brace:
  //   1) Call <algorithm>'s copy() function passing in
  //      a) il's begin and end values
  //      b) <iterator>'s back_inserter on *this.
  //
  dllist(std::initializer_list<T> il) : dllist{} {
    std::copy(il.begin(), il.end(), std::back_inserter(*this));
  }
  //
  // constructor dllist(first, last)
  //
  // You can copy the values of one container/range into another:
  //
  //   std::list<int> mylist( { 1, -5, 4 } );
  //   std::vector<int> myvector( begin(mylist), end(mylist) );
  //
  // This is the purpose of this constructor.
  //
  // Initialize the class as follows before the opening brace:
  //   1) Explicitly call the dllist<T> default constructor.
  //
  // Initialize the class as follows after the opening brace:
  //   1) Call <algorithm>'s copy() function passing in
  //      a) first and last, and,
  //      b) <iterator>'s back_inserter on *this.
  //
  template <typename InIter>
  dllist(InIter const& first, InIter const& last) : dllist{} {
    std::copy(first, last, std::back_inserter(*this));
  }

  //
  // copy constructor
  //
  // Initialize the class as follows before the opening brace:
  //   1) Explicitly call the dllist<T> default constructor.
  //
  // Initialize the class as follows after the opening brace:
  //   1) Call <algorithm>'s copy() function passing in
  //      a) l's begin and end, and,
  //      b) <iterator>'s back_inserter on *this.
  //
  dllist(dllist const& l) : dllist{} {
    std::copy(l.begin(), l.end(), std::back_inserter(*this));
  }

  //
  // move constructor
  //
  // Initialize the class as follows before the opening brace:
  //   1) Explicitly call the dllist<T> default constructor.
  //
  // Initialize the class as follows after the opening brace:
  //   2) Call swap(l) to swap l with *this. (swap() is defined below.)
  //
  dllist(dllist&& l) :
    dllist{}
  {
    swap(l);
  }

  //
  // destructor
  //
  // A destructor cleans things up but also must normally not be allowed to
  // throw an exception. To clean up the nodes in the container, one
  // simply calls the clear() member function (defined below). That said,
  // the clear() function might throw exceptions if T's destructor is
  // allowed to throw exceptions. To ensure that clear() doesn't throw
  // an exception, you'll need to call clear() from within a try block
  // and ignore any exception thrown (HINT: use catch(...) { }).
  // Your code should be something like this:
  //
  //   while (!empty())
  //   {
  //     // write try block here
  //     // do call clear() from within the try block
  //     // do write the catch clause here
  //   }
  //
  ~dllist() {
    while (!empty()) {
      try {
        clear();
      } catch(...) {
      }
    }
  }

  //
  // Write dllist's copy assignment operator here by:
  //   1) Declaring a local variable called tmp of type dllist<T>.
  //      Ensure this tmp variable's constructor is passed l to copy
  //      construct tmp with l's contents.
  //   2) Now call this->swap() on tmp.
  //   3) As this is an assignment operator, remember to return *this.
  //
  // This is an elegant, exception-safe way to make a copy of l! If an
  // exception occurs while making the copy of l, then *this remains
  // unmodified! (If the swap() operation will never throw --and it won't
  // for dllist<T>, then one is guaranteed that the state of *this will be
  // valid even if an exception is thrown.)
  //
  dllist& operator =(dllist const& l) {
    dllist<T> tmp(l);
    this->swap(tmp);
    return *this;
  }

  // Implement the move assignment operator per the copy assignment operator
  // except you want to move construct tmp passing in l.
  dllist& operator =(dllist&& l) {
    dllist<T> tmp(std::move(l));
    this->swap(tmp);
    return *this;
  }

  //
  // If you wrote:
  //
  //   dllist<int> l;
  //   // ...
  //   l.assign( { 1, 2, -5 } );
  //
  // the next function implements the l.assign() call. To implement this
  // function write:
  //   1) Create a local variable called tmp of type dllist<T> and pass
  //      in il to its constructor. (Don't move it --this initializer_list
  //      type is special to the compiler... just pass it in.)
  //   2) Call this->swap() to swap *this and tmp.
  //
  void assign(std::initializer_list<T> il) {
    dllist<T> tmp(il);
    this->swap(tmp);
  }

  //
  // To implement the next function write:
  //   1) Create a local variable called tmp of type dllist<T> and invoke
  //      the dllist<T> constructor appropriate for the arguments n and
  //      value.
  //   2) Call this->swap() to swap *this and tmp.
  //
  void assign(size_type n, value_type const& value) {
    dllist<T> tmp(n, value);
    this->swap(tmp);
  }

  //
  // To implement the next function write:
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

  //
  // To implement empty(), use the class member variable (look at the top
  // of this class!) to check whether or not there are elements.
  //
  bool empty() const {
    return size_ == 0;
  }

  //
  // To implement size(), use the class member variable (look at the top
  // of this class!) to determine how many elements there are in this
  // container.
  //
  size_type size() const {
    return size_;
  }

  // The next function has been provided for you.
  size_type max_size() const {
    return std::numeric_limits<size_type>::max();
  }

  // The next function has been provided for you.
  // NOTE: The precondition to this function's use is empty() == false.
  reference front() {
    return front_.nextptr(&back_)->to_node().datum();
  }

  // The next function has been provided for you.
  // NOTE: The precondition to this function's use is empty() == false.
  const_reference front() const {
    return front_.nextptr(&back_)->to_node().datum();
  }

  // The next function has been provided for you.
  // NOTE: The precondition to this function's use is empty() == false.
  reference back() {
    return back_.nextptr(&front_)->to_node().datum();
  }

  // The next function has been provided for you.
  // NOTE: The precondition to this function's use is empty() == false.
  const_reference back() const {
    return back_.nextptr(&front_)->to_node().datum();
  }

  //
  // begin()
  //
  // This function is a SINGLE line of code! Simply construct an rvalue
  // and return it! The rvalue is of type iterator (i.e., dllist_iter<T>)
  // since this is the non-const begin() member function definition.
  //
  // The constructor values you want to pass to iterator, are as follows:
  //   1) &front_ (i.e., this is the "previous [sentinel] node",)
  //   2) front_.nextptr(&back_) (i.e., this is the "current node")
  //
  iterator begin() {
    return dllist_iter<T>(&front_, front_.nextptr(&back_));
  }

  //
  // end()
  //
  // This function is a SINGLE line of code! Simply construct an rvalue
  // and return it! The rvalue is of type iterator (i.e., dllist_iter<T>)
  // since this is the non-const begin() member function definition.
  //
  // The constructor values you want to pass to iterator, are as follows:
  //   1) back_.nextptr(&front_) (i.e., this is the "last" node)
  //   2) &back_ (i.e., this is the "one-past-the-end" sentinel node)
  //
  iterator end() {
    return dllist_iter<T>(back_.nextptr(&front_), &back_);
  }

  //
  // begin() const
  //
  // This code is identical to the non-const begin() except it returns
  // const_iterator instead (i.e., dllist_citer<T>).
  //
  const_iterator begin() const{ 
    return dllist_citer<T>(&front_, front_.nextptr(&back_));
  }

  //
  // begin() const
  //
  // This code is identical to the non-const begin() except it returns
  // const_iterator instead (i.e., dllist_citer<T>).
  //
  const_iterator end() const {
    return dllist_citer<T>(back_.nextptr(&front_), &back_);
  }

  // The next function has been provided for you.
  // NOTE: Why risk error-prone typing? Simply call the appropriate
  //       function! :-)
  const_iterator cbegin() const
  {
    return begin();
  }

  // The next function has been provided for you.
  // NOTE: Why risk error-prone typing? Simply call the appropriate
  //       function! :-)
  const_iterator cend() const
  {
    return end();
  }

  //
  // rbegin() returns an instance of the reverse iterator adapter type
  // defined in <iterator> as std::reverse_iterator<ITERATOR_TYPE>.
  // std::reverse_iterator's constructor is passed the appropriate
  // "normal" iterator value. This means "rbegin()" will be passed
  // the return iterator from calling this->end().
  //
  // Write the SINGLE line of code to return the rvalue constructed
  // std::reverse_iterator.
  //
  reverse_iterator rbegin() {
    return reverse_iterator(this->end());
  }

  //
  // rend() is just like rbegin() except that this->begin() is passed
  // to the std::reverse_iterator constructor instead. :-)
  //
  reverse_iterator rend() {
    return reverse_iterator(this->begin());
  }

  //
  // rbegin() const is the same as rbegin() except the iterator
  // type is now const_reverse_iterator (i.e., dllist_citer<T>).
  //
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(this->end());
  }

  //
  // rend() const is the same as rend() except the iterator
  // type is now const_reverse_iterator (i.e., dllist_citer<T>).
  //
  const_reverse_iterator rend() const {
    return const_reverse_iterator(this->begin());
  }

  // crbegin() const returns what begin() const returns.
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(this->cend());
  }

  // crend() const returns what end() const returns.
  const_reverse_iterator crend() const {
    return const_reverse_iterator(this->cbegin());
  }

  //
  // clear() destroys all elements in the container
  //
  // Implement it as follows:
  //   1) while *this is not empty()
  //      a) call pop_back()
  //
  void clear() {
    while (!this->empty()) {
      this->pop_back();
    }
  }

  // The this->swap(l) function has been provided for you.
  // To understand why all of this work is required to swap, read its
  // comments.
  void swap(dllist& l)
  {
    //
    // Since &front_ and &back_ are hard-coded into the dllist nodes,
    // one must replace all &front_ with &l.front_ and all &back_
    // with &l.back_ and vice versa. There are at most four nodes
    // with these values in each list:
    //
    //   1) front_ and l.front_
    //   2) back_ and l.back_
    //   3) the first node of each list, if it exists
    //   4) the last node of each list, if it exists
    //
    // Once these "fixed" pointer values have been changed to be
    // the corresponding entry in the other list, then everything
    // can be safely swapped.
    //

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

    // FINALLY we can first swap the values...
    front_.swap(l.front_);
    back_.swap(l.back_);

    // AND then swap sizes...
    std::swap(size_, l.size_);
  }

  //
  // push_front(v)
  //
  // Implement this TWO-STATEMENT function as follows:
  //   1) Call dllist_node<T>::insert() passing in these three arguments:
  //      a) &back_ [sentinel node]
  //      b) &front_ [sentinel node]
  //      c) dynamically allocate a dllist_node<T> with operator new
  //         passing in v
  //   2) Increment size_.
  //
  // NOTE: Remember that insert() is static so you must prefix it with
  //       the full class name. See the insert() function above for a
  //       description of the arguments.
  //
  // NOTE: Now you should see why TWO sentinel nodes are needed with an
  //       XOR-encoded list.
  //
  void push_front(value_type const& v) {
    dllist_node<T>::insert(&back_, &front_, new dllist_node<value_type>(v));
    ++size_;
  }

  // The next function is identical to the previous push_front() function
  // except that v is moved into the dllist_node<T> constructor.
  void push_front(value_type&& v) {
    dllist_node<T>::insert(&back_, &front_, new dllist_node<value_type>(std::move(v)));
    ++size_;
  }

  //
  // pop_front()
  //
  // Implement this THREE-STATEMENT function as follows:
  //   1) Call dllist_node<T>::remove passing in these three arguments:
  //      a) &back_ [sentinel node]
  //      b) &front_ [sentinel node]
  //      AND assign its return value to a local variable called old.
  //   2) Decrement size_.
  //   3) Deallocate the dynamic memory associated with old.
  //      ASIDE: If you are paranoid, you can if (old != &back_) but this
  //             is unneeded as the precondition for pop_front() is
  //             empty() == false.
  void pop_front() {
    auto old = dllist_node<T>::remove(&back_, &front_);
    delete old;
    --size_;
  }

  //
  // push_back(v)
  // pop_back()
  //
  // These functions are IDENTICAL to their corresponding push_front(v)
  // and pop_back() functions except as follows:
  //
  //   1) push_back's insert call's first two arguments will be &front_ and
  //      &back_
  //   2) pop_back's remove call's arguments will be &front_ and &back_.
  //
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

  //
  // emplace(pos, args...)
  //
  // Implement this function as follows:
  //   1) pos is passed by value so you can modify it! Simply --pos;.
  //      Why? Because you want to ADD a new node AFTER pos.
  //   2) Write:
  //      auto newnode = new dllist_node<T>{T(std::forward<Args>(args)...)};
  //   3) Write:
  //      auto nextnode = dllist_node<T>::insert(
  //        pos.prevptr_, pos.nodeptr_, newnode);
  //   4) Increment size_.
  //   5) Return the appropriate constructed iterator type passing in
  //        newnode, nextnode as the constructor values.
  //
  template <typename... Args>
  iterator emplace(iterator pos, Args&&... args) {
    --pos;
    auto newnode = new dllist_node<T>{T(std::forward<Args>(args)...)};
    auto nextnode = dllist_node<T>::insert(pos.prevptr_, pos.nodeptr_, newnode);
    ++size_;
    return iterator(newnode, nextnode);
  }

  // Implement the next SINGLE statement function as follows:
  //   1) Call this->emplace() with these arguments:
  //      a) begin() [i.e., add the new node at the start of the list]
  //      b) std::forward<Args>(args)...
  template <typename... Args>
  void emplace_front(Args&&... args) {
    this->emplace(begin(), std::forward<Args>(args)...);
  }

  // Implement the next SINGLE statement function as follows:
  //   1) Call this->emplace() with these arguments:
  //      a) end() [i.e., add the new node at the start of the list]
  //      b) std::forward<Args>(args)...
  template <typename... Args>
  void emplace_back(Args&&... args) {
    this->emplace(end(), std::forward<Args>(args)...);
  }

  // This function is identical to emplace() except that args is replaced
  // with value and there is no std::forward or ... stuff.
  iterator insert(iterator pos, value_type const& value) {
    --pos;
    auto newnode = new dllist_node<T>(value);
    auto nextnode = dllist_node<T>::insert(pos.prevptr_, pos.nodeptr_, newnode);
    ++size_;
    return pos;
  }

  // Implement the insert function as follows:
  //   1) Write a for loop to iterate through the range [first,last).
  //   2) Inside the for loop is the single line of code:
  //      a) pos = insert(pos, *first);
  //         where *first is the variable you are iterating with.
  //         (You can use first --you don't need to make another variable.)
  template <typename InIter>
  void insert(iterator pos, InIter first, InIter const& last) {
    for (; first != last; ++first) {
      pos = insert(pos, *first);
    }
  }

  // Implement erase() as follows...
  //   1) You need to --pos; to properly erase the desired node.
  //   2) Remove the node using:
  //      auto oldnode = dllist_node<T>::remove(pos.prevptr_, pos.nodeptr_);
  //   3) Deallocate the dynamically allocated memory in oldnode.
  //   4) Move pos back, i.e., ++pos;
  //   5) Return pos.
  iterator erase(iterator pos) {
    --pos;
    auto oldnode = dllist_node<T>::remove(pos.prevptr_, pos.nodeptr_);
    delete oldnode;
    ++pos;
    --size_;
    return pos;
  }

  // Implement this erase() function as follows...
  //   1) Write a while loop with the test first != last.
  //      a) Inside the while loop, write "first = erase(first);"
  //   2) return first
  iterator erase(iterator first, iterator const& last) {
    while (first != last)  {
      first = erase(first);
    }
    return first;
  }
};

//===========================================================================

// The next function is provided for you.
template <typename T>
inline void swap(dllist<T>& a, dllist<T>& b)
{
  a.swap(b);
}

// Implement the next function as follows...
//   1) Write "using std::equal" (equal is in <algorithm>).
//   2) return the result of calling equal().
template <typename T>
inline bool operator ==(dllist<T> const& a, dllist<T> const& b) {
  return std::equal(a, b);
}

// The next function is provided for you. Notice that it is wise to
// simply call operator ==() here instead of writing "return !equal(..."...
template <typename T>
inline bool operator !=(dllist<T> const& a, dllist<T> const& b)
{
  return !(operator ==(a,b));
}

// Relational operators per C++14 Table 98 in Clause 23.2.1...

// Implement the next function as follows...
//   1) Write "using std::lexicographical_compare;" (which is in <algorithm>)
//   2) return the result of calling lexicographical_compare().
template <typename T>
inline bool operator <(dllist<T> const& a, dllist<T> const& b) {
  return std::lexicographical_compare(a, b);
}

// The next function is provided for you. It is defined per the ISO C++ 14
// standard document...
template <typename T>
inline bool operator <=(dllist<T> const& a, dllist<T> const& b)
{
  return !(a > b);
}

// The next function is provided for you. It is defined per the ISO C++ 14
// standard document...
template <typename T>
inline bool operator >=(dllist<T> const& a, dllist<T> const& b)
{
  return !(a < b);
}

// The next function is provided for you. It is defined per the ISO C++ 14
// standard document...
template <typename T>
inline bool operator >(dllist<T> const& a, dllist<T> const& b)
{
  return b < a;
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto begin(dllist<T>& a)
{
  return a.begin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto begin(dllist<T> const& a)
{
  return a.begin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto cbegin(dllist<T> const& a)
{
  return a.begin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto rbegin(dllist<T>& a)
{
  return a.rbegin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto rbegin(dllist<T> const& a)
{
  return a.rbegin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto crbegin(dllist<T> const& a)
{
  return a.crbegin();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto end(dllist<T>& a)
{
  return a.end();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto end(dllist<T> const& a)
{
  return a.end();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto cend(dllist<T> const& a)
{
  return a.end();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto rend(dllist<T>& a)
{
  return a.rend();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto rend(dllist<T> const& a)
{
  return a.rend();
}

// The next function is provided for you. It simply invokes the class
// member function...
template <typename T>
inline auto crend(dllist<T> const& a)
{
  return a.crend();
}

//===========================================================================

//
// dllist_iter<T>
//
// Every container in C++ has two iterator classes: one for non-const T
// values and one for const T values. The dllist_iter<T> defines the
// non-const T iterator type (i.e., notice the returned T values are not
// constant).
//
// This class does inherit from std::iterator which defines some type
// aliases needed to use this iterator with <algortihm> and other operations
// in the ISO C++ standard.
//
// Key here is to understand that "prevptr_" is the pointer to the node
// previous to the current node. This is needed to "unlock" the XOR-encoded
// pointer value in "nodeptr_" to get the NEXT node to go to (e.g.,
// see operator ++()).
//
// Similarly, one can use "nodeptr_" as the previous pointer to "prevptr_"
// to "unlock" the XOR-encoded pointer value in "prevptr_" to get to the
// PREVIOUS node of such (e.g., see operator --()).
//
template <typename T>
class dllist_iter : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T&> {
private:
  friend class dllist<T>;
  friend class dllist_citer<T>;

  dllist_node_ptr_only<T>* prevptr_; // Used to compute next node address
  dllist_node_ptr_only<T>* nodeptr_; // Cur node; for xorptr_ value

public:
  // The default constructor simply initializes prevptr_ and nodeptr_
  // to nullptr. Clearly the iterator type should never be used if
  // default constructed until it is properly initialized by assignment.
  //
  // NOTE: Iterators are created by begin() and end() functions so this
  //       really is a non-issue. Having a default constructor makes it
  //       possible to declare an iterator variable that is later assigned
  //       and used though.
  dllist_iter() :
    prevptr_{nullptr}, nodeptr_{nullptr} {}

  dllist_iter(dllist_iter const&) = default;
  dllist_iter& operator =(dllist_iter const&) = default;

  dllist_iter(dllist_iter&) = default;
  dllist_iter& operator =(dllist_iter&) = default;

  ~dllist_iter() = default;

  // This is the constructor that the dllist<T> class calls. It has been
  // provided for you.
  dllist_iter(dllist_node_ptr_only<T>* prev, dllist_node_ptr_only<T>* xornode) :
    prevptr_{prev}, nodeptr_{xornode} {}

  // Implement the next function computing:
  //   If nodeptr_ is equal to i.nodeptr_ then the nodes are the same.
  bool operator ==(dllist_iter const& i) const {
    return nodeptr_ == i.nodeptr_;
  }

  // Implement the next function by calling operator ==(i) and
  // appropriately manipulating its result.
  bool operator !=(dllist_iter const& i) const {
    return !(operator==(i));
  }

  // The next function has been implemented for you.
  // This is unary *, i.e., the indirection (pointer) operator.
  // The value returned must be a T&, i.e., one accesses nodeptr_, casts
  // it to dllist_node<T> (via to_node()) and then returns its datum member
  // using datum().
  T& operator *() const {
    return nodeptr_->to_node().datum();
  }

  // The next function has been implemented for you.
  // This is the -> (indirection) operator which must return a T*.
  // It does the same thing as unary * except it returns the address using
  // the address-of operator. (Recall that datum() always returns an lvalue
  // reference.)
  T* operator ->() const {
    return &nodeptr_->to_node().datum();
  }

  // Implement the prefix operator ++ as follows:
  //   1) Execute: auto next_nodeptr_ = nodeptr_->nextptr(prevptr_);
  //      NOTE: This will "unlock" the next pointer using prevptr_.
  //   2) Assign prevptr_ to nodeptr_.
  //   3) Assign nodeptr_ to next_nodeptr_.
  //   4) return *this  [prefix ++ needs to return an lvalue reference]
  dllist_iter& operator ++() {
    auto next_nodeptr_ = nodeptr_->nextptr(prevptr_);
    prevptr_ = nodeptr_;
    nodeptr_ = next_nodeptr_;
    return *this;
  }

  // Implement postfix operator ++ in terms of prefix ++ as follows:
  //   1) Declare a local variable of type dllist_iter<T> passing *this
  //      to its constructor (i.e., copy construct).
  //   2) Call the PREFIX operator ++(). [Notice there is no int arg.]
  //   3) Return the local variable tmp.
  dllist_iter operator ++(int) {
    dllist_iter<T> tmp(*this);
    operator++();
    return tmp;
  }

  // Implement the prefix operator -- as follows:
  //   1) Execute: auto prev_prevptr_ = prevptr_->nextptr(nodeptr_);
  //      NOTE: This will "unlock" the prev pointer using nodeptr_.
  //   2) Assign nodeptr_ to prevptr_.
  //   3) Assign prevptr_ to prev_prevptr_.
  //   4) return *this  [prefix -- needs to return an lvalue reference]
  dllist_iter& operator --() {
    auto prev_prevptr_ = prevptr_->nextptr(nodeptr_);
    nodeptr_ = prevptr_;
    prevptr_ = prev_prevptr_;
    return *this;
  }

  // The postfix -- operator is implemented like postfix ++ operator
  // except you call the prefix operator --() instead.
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
// This class is 100% identical to dllist_iter<T> except that T is const
// and therefore return values, etc. are const.
//
// Implement the code in this function per the descriptions given in
// dllist_iter<T>.
//
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

  // Provided for you... for dllist_iter<T> interoperability...
  // (This is based on a requirement by the standard.)
  dllist_citer(dllist_iter<T> const& i) :
    nodeptr_{i.nodeptr_}, prevptr_{i.prevptr_} {}

  // Provided for you... for dllist_iter<T> interoperability...
  // (This is based on a requirement by the standard.)
  dllist_citer& operator =(dllist_iter<T> const& i) {
    nodeptr_ = i.nodeptr_;
    prevptr_ = i.prevptr_;
    return *this;
  }

  // Provided for you... for dllist_iter<T> interoperability...
  // (This is based on a requirement by the standard.)
  bool operator ==(dllist_iter<T> const& i) const {
    return nodeptr_ == i.nodeptr_;
  }

  // Provided for you... for dllist_iter<T> interoperability...
  // (This is based on a requirement by the standard.)
  bool operator !=(dllist_iter<T> const& i) const {
    return !operator ==(i);
  }

  // Write the remaining functions...
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

// Provided for you... for dllist_iter<T> interoperability...
// (This is based on a requirement by the standard.)
template <typename T>
inline bool operator ==(dllist_iter<T> const& i, dllist_citer<T> const& j)
{
  return j == i;
}

// Provided for you... for dllist_iter<T> interoperability...
// (This is based on a requirement by the standard.)
template <typename T>
inline bool operator !=(dllist_iter<T> const& i, dllist_citer<T> const& j)
{
  return j != i;
}

//===========================================================================

// WRITE THE NEEDED PREPROCESSOR STATEMENT HERE!
#endif
