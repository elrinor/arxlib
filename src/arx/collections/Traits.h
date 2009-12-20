#ifndef __ARX_COLLECTIONS_TRAITS_H__
#define __ARX_COLLECTIONS_TRAITS_H__

#include "config.h"
#include "../HasXxx.h"

namespace arx {
  namespace detail {
    ARX_HAS_TYPE_XXX_TRAIT_DEF(mapped_type);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(key_type);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(find);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(push_front);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(push_back);

    ARX_HAS_FUNC_XXX_TRAIT_DEF(empty);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(size_type);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(size);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(reverse_iterator);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(const_reverse_iterator);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(rbegin);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(rend);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(allocator_type);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(get_allocator);

    ARX_HAS_FUNC_XXX_TRAIT_DEF(back);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(front);

    ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF(has_operator_brack, operator[], (int));

    ARX_HAS_FUNC_XXX_TRAIT_DEF(reserve);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(capacity);

    ARX_HAS_FUNC_XXX_TRAIT_DEF(assign);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(clear);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(insert);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(erase);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(key_compare);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(value_compare);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(hasher);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(key_equal);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(local_iterator);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(const_local_iterator);

  } // namespace detail
} // namespace arx


#endif // __ARX_COLLECTIONS_TRAITS_H__
