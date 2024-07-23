#pragma once

#include <list>
#include <vector>
#include <cstdint>
#include <utility>
#include <functional>

#include "Pulse/Core/Logging.hpp"

namespace Pulse
{

    template <typename Key, typename Value>
    class HashMapIterator 
    {
    public:
        using BucketIterator = typename std::list<std::pair<Key, Value>>::iterator;
        using OuterIterator = typename std::vector<std::list<std::pair<Key, Value>>>::iterator;

    public:
        // Constructors
        HashMapIterator(OuterIterator outerBegin, OuterIterator outerEnd, BucketIterator bucketIterator);
        ~HashMapIterator() = default;

        // Operators
        std::pair<Key, Value>& operator * ();
        std::pair<Key, Value>* operator -> ();

        HashMapIterator& operator ++ ();

        bool operator == (const HashMapIterator& other) const;
        bool operator != (const HashMapIterator& other) const;

    private:
        // Private methods
        void AdvanceToNonEmptyBucket();

    private:
        OuterIterator m_OuterIteratorBegin;
        OuterIterator m_OuterIteratorEnd;

        BucketIterator m_BucketIterator;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    HashMapIterator<Key, Value>::HashMapIterator(OuterIterator outerBegin, OuterIterator outerEnd, BucketIterator bucketIterator)
        : m_OuterIteratorBegin(outerBegin), m_OuterIteratorEnd(outerEnd), m_BucketIterator(bucketIterator) 
    {
        AdvanceToNonEmptyBucket();
    }

    ///////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    std::pair<Key, Value>& HashMapIterator<Key, Value>::operator * ()
    {
        return *m_BucketIterator;
    }

    template<typename Key, typename Value>
    std::pair<Key, Value>* HashMapIterator<Key, Value>::operator -> ()
    {
        return &(*m_BucketIterator);
    }

    template<typename Key, typename Value>
    HashMapIterator<Key, Value>& HashMapIterator<Key, Value>::operator ++ ()
    {
        ++m_BucketIterator;
        AdvanceToNonEmptyBucket();

        return *this;
    }

    template<typename Key, typename Value>
    bool HashMapIterator<Key, Value>::operator == (const HashMapIterator& other) const
    {
        return (m_OuterIteratorBegin == other.m_OuterIteratorBegin) && ((m_OuterIteratorBegin == m_OuterIteratorEnd) || (m_BucketIterator == other.m_BucketIterator));
    }

    template<typename Key, typename Value>
    bool HashMapIterator<Key, Value>::operator != (const HashMapIterator& other) const
    {
        return !(*this == other);
    }

    ///////////////////////////////////////////////////////////
    // Private Methods
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    void HashMapIterator<Key, Value>::AdvanceToNonEmptyBucket()
    {
        while (m_OuterIteratorBegin != m_OuterIteratorEnd 
            && m_BucketIterator == m_OuterIteratorBegin->end()) 
        {
            ++m_OuterIteratorBegin;
            if (m_OuterIteratorBegin != m_OuterIteratorEnd)
                m_BucketIterator = m_OuterIteratorBegin->begin();
        }
    }



    // Simple HashMap class
    template <typename Key, typename Value>
    class HashMap
    {
    public:
        // Constructors
        constexpr HashMap(size_t bucketCount = 16);
        constexpr ~HashMap() = default;

        // Operators
        inline constexpr Value& operator [] (const Key& key) { return At(key); }
        inline constexpr const Value& operator [] (const Key& key) const { return At(key); }

        // Methods
        constexpr bool Insert(const Key& key, const Value& value);
        constexpr bool Remove(const Key& key);

        constexpr Value* Find(const Key& key);

        constexpr Value& At(const Key& key);
        constexpr const Value& At(const Key& key) const;

        constexpr bool Contains(const Key& key) const;

        constexpr float LoadFactor() const;

        constexpr HashMapIterator<Key, Value> begin();
        constexpr HashMapIterator<Key, Value> end();

    private:
        // Private Methods
        constexpr size_t GetBucketIndex(const Key& key) const;

        constexpr void Rehash(size_t newBucketCount);

    private:
        std::vector<std::list<std::pair<Key, Value>>> m_Buckets;
        std::hash<Key> m_HashFn = {};
        size_t m_ElementCount = 0ull;
        
        inline static constinit const float s_MaxLoadFactor = 0.75f;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    constexpr HashMap<Key, Value>::HashMap(size_t bucketCount)
        : m_Buckets(bucketCount) {}

    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    constexpr bool HashMap<Key, Value>::Insert(const Key& key, const Value& value)
    {
        if (LoadFactor() > s_MaxLoadFactor)
            Rehash(m_Buckets.size() * 2);
        
        size_t index = GetBucketIndex(key);
        for (auto& [k, v] : m_Buckets[index])
        {
            if (k == key)
                return false; // Key already exists, do not insert
        }

        m_Buckets[index].emplace_back(key, value);
        ++m_ElementCount;

        return true;
    }

    template<typename Key, typename Value>
    constexpr bool HashMap<Key, Value>::Remove(const Key& key)
    {
        size_t index = GetBucketIndex(key);

        auto& bucket = m_Buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) 
        {
            if (it->first == key) 
            {
                bucket.erase(it);
                --m_ElementCount;

                return true;
            }
        }

        return false;
    }

    template<typename Key, typename Value>
    constexpr Value* HashMap<Key, Value>::Find(const Key& key)
    {
        size_t index = GetBucketIndex(key);

        for (auto& [k, v] : m_Buckets[index]) 
        {
            if (k == key)
                return &v;
        }

        return nullptr;
    }

    template<typename Key, typename Value>
    constexpr Value& HashMap<Key, Value>::At(const Key& key)
    {
        size_t index = GetBucketIndex(key);

        for (auto& [k, v] : m_Buckets[index])
        {
            if (k == key)
                return v;
        }

        Logger::Assert(false, "Key not found");
    }

    template<typename Key, typename Value>
    constexpr const Value& HashMap<Key, Value>::At(const Key& key) const
    {
        size_t index = GetBucketIndex(key);

        for (const auto& [k, v] : m_Buckets[index]) 
        {
            if (k == key)
                return v;
        }

        Logger::Assert(false, "Key not found");
    }

    template<typename Key, typename Value>
    constexpr bool HashMap<Key, Value>::Contains(const Key& key) const
    {
        size_t index = GetBucketIndex(key);

        for (const auto& [k, v] : m_Buckets[index]) 
        {
            if (k == key)
                return true;
        }

        return false;
    }

    template<typename Key, typename Value>
    constexpr float HashMap<Key, Value>::LoadFactor() const
    {
        return static_cast<float>(m_ElementCount) / m_Buckets.size();
    }

    template<typename Key, typename Value>
    constexpr HashMapIterator<Key, Value> HashMap<Key, Value>::begin()
    {
        return HashMapIterator<Key, Value>(m_Buckets.begin(), m_Buckets.end(), m_Buckets.empty() ? typename std::list<std::pair<Key, Value>>::iterator() : m_Buckets.front().begin());
    }

    template<typename Key, typename Value>
    constexpr HashMapIterator<Key, Value> HashMap<Key, Value>::end()
    {
        return HashMapIterator<Key, Value>(m_Buckets.end(), m_Buckets.end(), typename std::list<std::pair<Key, Value>>::iterator());
    }

    ///////////////////////////////////////////////////////////
    // Private Methods
    ///////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    constexpr size_t HashMap<Key, Value>::GetBucketIndex(const Key& key) const
    {
        return m_HashFn(key) % m_Buckets.size();
    }

    template<typename Key, typename Value>
    constexpr void HashMap<Key, Value>::Rehash(size_t newBucketCount)
    {
        std::vector<std::list<std::pair<Key, Value>>> newBuckets(newBucketCount);

        for (const auto& bucket : m_Buckets)
        {
            for (const auto& [k, v] : bucket) 
            {
                size_t newIndex = std::hash<Key>()(k) % newBucketCount;
                newBuckets[newIndex].emplace_back(k, v);
            }
        }

        m_Buckets.swap(newBuckets);
    }

}