/**
 * User: Houliang Qi
 * Date: 1/24/18
 * Time: 3:37 PM
 */

#ifndef _GOPHERWOOD_COMMON_LRUCACHE_H_
#define _GOPHERWOOD_COMMON_LRUCACHE_H_

#include <unordered_map>
#include <list>
#include <vector>
#include "Logger.h"

namespace Gopherwood {

    template<typename key_t, typename value_t>
    class LRUCache {
    public:
        typedef typename std::pair<key_t, value_t> key_value_pair_t;
        typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

        LRUCache(size_t
                 max_size) :
                _max_size(max_size) {
        }

        std::vector<key_t> put(const key_t &key, const value_t &value) {
            auto it = _cache_items_map.find(key);
            _cache_items_list.push_front(key_value_pair_t(key, value));
            if (it != _cache_items_map.end()) {
                _cache_items_list.erase(it->second);
                _cache_items_map.erase(it);
            }
            _cache_items_map[key] = _cache_items_list.begin();

            std::vector<key_t> deleteVector;

            if (_cache_items_map.size() > _max_size) {
                auto last = _cache_items_list.end();
                last--;
                _cache_items_map.erase(last->first);
                _cache_items_list.pop_back();
                deleteVector.push_back(last->first);
            }
            return deleteVector;
        }

        void deleteObject(const key_t &key) {
            auto it = _cache_items_map.find(key);
            if (it != _cache_items_map.end()) {
                _cache_items_list.erase(it->second);
                _cache_items_map.erase(it);
            }
        }

        std::vector<key_t> getAllKeyObject() {
            std::vector<key_t> allKeyVector;
            for (key_value_pair_t tmpKV: _cache_items_list) {
                allKeyVector.push_back(tmpKV.first);
            }
            return allKeyVector;
        }


        const bool get(const key_t &key) {
            auto it = _cache_items_map.find(key);
            if (it == _cache_items_map.end()) {
                LOG(Gopherwood::Internal::INFO, "There is no such key in cache");
                return false;
            } else {
                _cache_items_list.splice(_cache_items_list.begin(), _cache_items_list, it->second);
                return true;
//                return it->second->second;
            }
        }

        bool exists(const key_t &key) const {
            return _cache_items_map.find(key) != _cache_items_map.end();
        }

        size_t size() const {
            return _cache_items_map.size();
        }

        void printLruCache() {
            LOG(Gopherwood::Internal::INFO, "start to print the lru cache status");
            for (key_value_pair_t tmpKV: _cache_items_list) {
                LOG(Gopherwood::Internal::INFO, "the key = %d, the value =%d", tmpKV.first, tmpKV.second);
            }
            LOG(Gopherwood::Internal::INFO, "the end of print the lru cache status");
        }

    private:
        std::list<key_value_pair_t> _cache_items_list;
        std::unordered_map<key_t, list_iterator_t> _cache_items_map;
        size_t _max_size;
    };

} // namespace Gopherwood

#endif    /* _GOPHERWOOD_COMMON_LRUCACHE_H_ */
