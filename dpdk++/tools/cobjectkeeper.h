#ifndef COBJECTKEEPER_H
#define COBJECTKEEPER_H
#include <base/base_types.h>
#include <map>
namespace dc_tools
{

template <class TClass, uint32_t maxCapacity> class cObjKeeper
{
public:
    cObjKeeper( const std::string& name );

    DC_ALWAYS_INLINE uint32_t capacity() const;
    DC_ALWAYS_INLINE uint32_t maxIndex() const;
    DC_ALWAYS_INLINE uint32_t freeIndexesCount() const;
    DC_ALWAYS_INLINE TClass* getObj( uint32_t index ) const;
    DC_ALWAYS_INLINE bool hasFreeIndexs() const;
    DC_ALWAYS_INLINE std::map<uint32_t, TClass*> getObjectsWithIdexes() const;

    DC_ALWAYS_INLINE std::vector<TClass*> getAllAvaliableObjects() const
    {
        std::vector<TClass*> answer;
        for( const sInternalItem& item : indexArray_ )
        {
            if( item.value_ != nullptr )
            {
                if( item.isInited_ )
                {
                    answer.push_back( item.value_ );
                }
            }
        }
        return answer;
    }

    DC_ALWAYS_INLINE uint32_t addNewObj( TClass* obj );
    DC_ALWAYS_INLINE bool insertObj( TClass* obj, uint32_t index );
    DC_ALWAYS_INLINE void setInited( uint32_t index );
    TClass* removeObjectById( uint32_t index );

    inline std::string name() const;
    inline std::string toStr() const;

    DC_ALWAYS_INLINE uint16_t size() const;

    struct sInternalItem
    {
        TClass* value_ = nullptr;
        bool isInited_ = false;
    };

private:
    std::array<sInternalItem, maxCapacity> indexArray_ = { 0 };
    uint32_t currentCount_ = 0;
    std::string name_ = "";
};

template <class TClass, uint32_t maxCapacity> DC_ALWAYS_INLINE uint16_t cObjKeeper<TClass, maxCapacity>::size() const
{
    return currentCount_;
}

template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE TClass* cObjKeeper<TClass, maxCapacity>::getObj( uint32_t index ) const
{
    return indexArray_[index].value_;
}

template <class TClass, uint32_t maxCapacity> DC_ALWAYS_INLINE bool cObjKeeper<TClass, maxCapacity>::hasFreeIndexs() const
{
    return freeIndexesCount() > 0;
}

template <class TClass, uint32_t maxCapacity>
cObjKeeper<TClass, maxCapacity>::cObjKeeper( const std::string& name ) : name_( name )
{
    for( int i = 0; i < maxCapacity; ++i )
    {
        if( indexArray_[i].value_ != nullptr )
        {
            THROW_ERROR( "BAD" );
        }
    }
}

template <class TClass, uint32_t maxCapacity> DC_ALWAYS_INLINE uint32_t cObjKeeper<TClass, maxCapacity>::capacity() const
{
    return maxCapacity;
}
template <class TClass, uint32_t maxCapacity> DC_ALWAYS_INLINE uint32_t cObjKeeper<TClass, maxCapacity>::maxIndex() const
{
    return maxCapacity - 1;
}
template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE uint32_t cObjKeeper<TClass, maxCapacity>::freeIndexesCount() const
{
    return maxCapacity - currentCount_;
}

template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE bool cObjKeeper<TClass, maxCapacity>::insertObj( TClass* obj, uint32_t index )
{
    THROW_ASSERT( freeIndexesCount() > 0, "BAD VALUE" );
    L_DEBUG << name() << " INDESTING " << index;
    if( indexArray_[index].value_ != nullptr )
    {
        return false;
    }
    indexArray_[index].value_ = obj;
    indexArray_[index].isInited_ = false;
    currentCount_++;
    setInited( index );
    return true;
}

template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE uint32_t cObjKeeper<TClass, maxCapacity>::addNewObj( TClass* obj )
{
    THROW_ASSERT( freeIndexesCount() > 0, "BAD VALUE" );
    for( uint32_t i = 0; i < maxCapacity; ++i )
    {
        if( indexArray_[i].value_ == nullptr )
        {
            indexArray_[i].value_ = obj;
            indexArray_[i].isInited_ = false;
            currentCount_++;
            setInited( i );
            L_DEBUG << name() << " ADDING " << i;
            return i;
        }
    }
    return BAD_UINT32_VALUE;
}

template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE std::map<uint32_t, TClass*> cObjKeeper<TClass, maxCapacity>::getObjectsWithIdexes() const
{
    std::map<uint32_t, TClass*> answer;
    for( int i = 0; i < maxCapacity; ++i )
    {
        if( indexArray_[i].value_ != nullptr )
        {
            answer[i] = indexArray_[i].value_;
        }
    }
    return answer;
}

template <class TClass, uint32_t maxCapacity>
DC_ALWAYS_INLINE void cObjKeeper<TClass, maxCapacity>::setInited( uint32_t index )
{
    THROW_ASSERT( indexArray_[index].value_ != nullptr, "BAD VAL" );
    THROW_ASSERT( indexArray_[index].isInited_ == false, "ALREADY INITED" );
    indexArray_[index].isInited_ = true;
}
template <class TClass, uint32_t maxCapacity>
TClass* cObjKeeper<TClass, maxCapacity>::removeObjectById( uint32_t index )
{
    THROW_ASSERT( indexArray_[index].value_, "BAD INDEX" );
    TClass* answer = indexArray_[index].value_;
    indexArray_[index].value_ = nullptr;
    indexArray_[index].isInited_ = false;
    currentCount_--;
    return answer;
}
template <class TClass, uint32_t maxCapacity> inline std::string cObjKeeper<TClass, maxCapacity>::name() const
{
    return name_;
}
template <class TClass, uint32_t maxCapacity> inline std::string cObjKeeper<TClass, maxCapacity>::toStr() const
{
    std::stringstream ss;
    ss << "name:\"" << name() << "\"" << std::endl;
    ss << "capacity:" << capacity() << std::endl;
    ss << "free ind:" << freeIndexesCount() << std::endl;
    return ss.str();
}

} // namespace tools
#endif // COBJECTKEEPER_H
