#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

template <typename T>
class SparseSet {
public:
    SparseSet() = default;

    bool contains(T item) const {
        return dense_to_sparse_.count(item) > 0;
    }

    void insert(T item) {
        if (!contains(item)) {
            dense_to_sparse_[item] = size_;
            sparse_to_dense_.push_back(item);
            size_++;
        }
    }

    void erase(T item) {
        if (contains(item)) {
            size_--;
            T last_item = sparse_to_dense_.back();
            T removed_item = sparse_to_dense_[dense_to_sparse_[item]];

            std::iter_swap(sparse_to_dense_.begin() + dense_to_sparse_[item], sparse_to_dense_.end() - 1);
            sparse_to_dense_.pop_back();
            dense_to_sparse_[last_item] = dense_to_sparse_[item];
            dense_to_sparse_.erase(item);
        }
    }

    T* get(T item) {
        if (contains(item)) {
            return &sparse_to_dense_[dense_to_sparse_[item]];
        }
        return nullptr;
    }

    const std::vector<T>& getEntities() const {
        return sparse_to_dense_;
    }

    size_t size() const {
        return size_;
    }

    // Iterator type for SparseSet
    class Iterator {
    public:
        Iterator(typename std::vector<T>::iterator it) : it_(it) {}

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++it_;
            return temp;
        }

        T& operator*() {
            return *it_;
        }

        bool operator==(const Iterator& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        typename std::vector<T>::iterator it_;
    };

    Iterator begin() {
        return Iterator(sparse_to_dense_.begin());
    }

    Iterator end() {
        return Iterator(sparse_to_dense_.end());
    }

private:
    std::vector<T> sparse_to_dense_;
    std::unordered_map<T, size_t> dense_to_sparse_;
    size_t size_ = 0;
};
