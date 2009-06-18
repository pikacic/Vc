/**
 * The type of the vector used for indexes in gather and scatter operations.
 */
typedef INDEX_TYPE IndexType;

/**
 * The type of the entries in the vector.
 */
typedef ENTRY_TYPE EntryType;

/**
 * The type of the mask used for masked operations and returned from comparisons.
 */
typedef MASK_TYPE Mask;

enum {
    /**
     * The size of the vector. I.e. the number of scalar entries in the vector.
     */
    Size
};

/**
 * Construct an uninitialized vector.
 */
VECTOR_TYPE();

/**
 * Construct a vector with the entries initialized to zero.
 *
 * \see Vc::Zero
 */
VECTOR_TYPE(Vc::Zero);

/**
 * Construct a vector with the entries initialized to one.
 *
 * \see Vc::One
 */
VECTOR_TYPE(Vc::One);

/**
 * Construct a vector with the entries initialized to 0, 1, 2, 3, 4, 5, ...
 *
 * \see Vc::IndexesFromZero
 */
VECTOR_TYPE(Vc::IndexesFromZero);

/**
 * Construct a vector loading its entries from \p alignedMemory.
 *
 * \param alignedMemory A pointer to data. The pointer must be aligned on a
 *                      Vc::VectorAlignment boundary.
 */
VECTOR_TYPE(ENTRY_TYPE *alignedMemory);

/**
 * Convert from another vector type.
 */
template<typename OtherVector> explicit VECTOR_TYPE(const OtherVector &);

/**
 * Construct a vector with all entries of the vector filled with the given value.
 *
 * \note If you want to set it to 0 or 1 use the special initializer constructors above. Calling
 * this constructor with 0 will cause a compilation error because the compiler cannot know which
 * constructor you meant.
 */
VECTOR_TYPE(ENTRY_TYPE x);

/**
 * Construct a vector from an array of vectors with different Size.
 *
 * E.g. convert from two double_v to one float_v.
 *
 * \see expand
 */
VECTOR_TYPE(const OtherVector *array);

/**
 * Expand the values into an array of vectors that have a different Size.
 *
 * E.g. convert from one float_v to two double_v.
 *
 * This is the reverse of the above constructor.
 */
void expand(OtherVector *array) const;

/**
 * Load the vector entries from \p alignedMemory, overwriting the previous values.
 *
 * \param alignedMemory A pointer to data. The pointer must be aligned on a
 *                      Vc::VectorAlignment boundary.
 */
void load(const ENTRY_TYPE *alignedMemory);

/**
 * Load the vector entries from \p unalignedMemory, overwriting the previous values.
 *
 * \param unalignedMemory A pointer to data. The pointer must not be aligned.
 */
void loadUnaligned(const ENTRY_TYPE *unalignedMemory);

/**
 * Set all entries to zero.
 */
void makeZero();

/**
 * Set all entries to zero where the mask is set. I.e. a 4-vector with a mask of 0111 would
 * set the last three entries to 0.
 */
void makeZero(const MASK_TYPE &mask);

/**
 * Store the vector data to \p alignedMemory.
 *
 * \param alignedMemory A pointer to memory, where to store. The pointer must be aligned on a
 *                      Vc::VectorAlignment boundary.
 */
void store(EntryType *alignedMemory) const;

/**
 * Non-temporal store variant. Writes to the memory without polluting the cache.
 *
 * \param alignedMemory A pointer to memory, where to store. The pointer must be aligned on a
 *                      Vc::VectorAlignment boundary.
 */
void storeStreaming(EntryType *alignedMemory) const;

/**
 * Return the vector entry at the given \p index.
 *
 * \param index A value between 0 and Size. This value is not checked internally so you must make/be
 *              sure it is in range.
 */
ENTRY_TYPE operator[](int index) const;

/**
 * \name Gather and Scatter Functions
 * The gather and scatter functions allow you to easily use vectors with structured data and random
 * accesses.
 *
 * There are two variants:
 * \li For random access in arrays.
 * \li For random access of members of structs in an array.
 *
 * All gather and scatter functions optionally take a mask as last argument. In that case only the
 * entries that are selected in the mask are read in memory and copied to the vector. This allows
 * you to have invalid indexes in the \p indexes vector if those are masked off in \p mask.
 *
 * Accessing values of a struct works like this:
 * \code
 * struct MyData {
 *   float a;
 *   int b;
 * };
 *
 * void foo(MyData *data, uint_v indexes) {
 *   const float_v v1(data, &MyData::a, indexes);
 *   const int_v   v2(data, &MyData::b, indexes);
 *   v1.scatter(data, &MyData::a, indexes - float_v::Size);
 *   v2.scatter(data, &MyData::b, indexes - 1);
 * }
 * \endcode
 */
//@{
VECTOR_TYPE(const ENTRY_TYPE *array, const INDEX_TYPE &indexes);
VECTOR_TYPE(const ENTRY_TYPE *array, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

template<typename S1> VECTOR_TYPE(const S1 *array, const ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes);
template<typename S1> VECTOR_TYPE(const S1 *array, const ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

template<typename S1, typename S2> VECTOR_TYPE(const S1 *array, const S2 S1::* member1, const ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes);
template<typename S1, typename S2> VECTOR_TYPE(const S1 *array, const S2 S1::* member1, const ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

/////////////////////////

void gather(const ENTRY_TYPE *array, const INDEX_TYPE &indexes);
void gather(const ENTRY_TYPE *array, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

template<typename S1> void gather(const S1 *array, const ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes);
template<typename S1> void gather(const S1 *array, const ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

template<typename S1, typename S2> void gather(const S1 *array, const S2 S1::* member1, const ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes);
template<typename S1, typename S2> void gather(const S1 *array, const S2 S1::* member1, const ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes, const MASK_TYPE &mask);

/////////////////////////

void scatter(ENTRY_TYPE *array, const INDEX_TYPE &indexes) const ;
void scatter(ENTRY_TYPE *array, const INDEX_TYPE &indexes, const MASK_TYPE &mask) const ;

template<typename S1> void scatter(S1 *array, ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes) const ;
template<typename S1> void scatter(S1 *array, ENTRY_TYPE S1::* member1, const INDEX_TYPE &indexes, const MASK_TYPE &mask) const ;

template<typename S1, typename S2> void scatter(S1 *array, S2 S1::* member1, ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes) const ;
template<typename S1, typename S2> void scatter(S1 *array, S2 S1::* member1, ENTRY_TYPE S2::* member2, const INDEX_TYPE &indexes, const MASK_TYPE &mask) const ;
//@}

/**
 * \name Comparisons
 *
 * All comparison operators return a mask object.
 *
 * \code
 * void foo(const float_v &a, const float_v &b) {
 *   const float_m mask = a < b;
 *   ...
 * }
 * \endcode
 */
//@{
MASK_TYPE operator==(const VECTOR_TYPE &x) const;
MASK_TYPE operator!=(const VECTOR_TYPE &x) const;
MASK_TYPE operator> (const VECTOR_TYPE &x) const;
MASK_TYPE operator>=(const VECTOR_TYPE &x) const;
MASK_TYPE operator< (const VECTOR_TYPE &x) const;
MASK_TYPE operator<=(const VECTOR_TYPE &x) const;
//@}

/**
 * \name Arithmetic Operations
 *
 * The vector classes implement all the arithmetic and (bitwise) logical operations as you know from
 * builtin types.
 *
 * \code
 * void foo(const float_v &a, const float_v &b) {
 *   const float_v product    = a * b;
 *   const float_v difference = a - b;
 * }
 * \endcode
 */
//@{
VECTOR_TYPE operator+(VECTOR_TYPE x) const;
VECTOR_TYPE operator-(VECTOR_TYPE x) const;
VECTOR_TYPE operator*(VECTOR_TYPE x) const;
VECTOR_TYPE operator/(VECTOR_TYPE x) const;
VECTOR_TYPE operator|(VECTOR_TYPE x) const;
VECTOR_TYPE operator&(VECTOR_TYPE x) const;
VECTOR_TYPE operator^(VECTOR_TYPE x) const;
//@}
