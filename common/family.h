#pragma once

#include <atomic>
#include <type_traits>

namespace mkr {
    /**
     * Utility class to generate a unique id for a group of classes.
     * Each child class of B will have a type id generated starting from 0.
     * @tparam B The base class of the category.
     */
    template<typename B>
    class family {
    private:
        /// Current type id.
        static std::atomic<uint64_t> current_id_;

        /**
         * Generate the type id of a class. This exists so that get_id() can remove the cv of the template class.
         * @tparam D The derived class of B. D may also be of type B. This type is cv removed.
         * @return The type id of class D.
         */
        template<typename D>
        static uint64_t generate_id() {
            static uint64_t id = current_id_++;
            return id;
        }

    public:
        static constexpr uint64_t INVALID_ID = 0;

        family() = delete;

        /**
         * Get the type id of a class.
         * @tparam D The class type. D may also be of type B.
         * @return The type if of D.
         */
        template<typename D>
        static uint64_t get_id() requires std::is_base_of_v<B, D> {
            return generate_id<std::remove_cvref_t<D>>();
        }
    };

    /// Type id starts from 1.
    template<typename B>
    std::atomic<uint64_t> family<B>::current_id_ = 1;

#define FAMILY_ID(__BASE__, __DERIVED__) family<__BASE__>::get_id< __DERIVED__>()
}