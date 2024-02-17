#ifndef CRINGCOVER_H
#define CRINGCOVER_H
#include <base/base_types.h>
#include <rte_ring.h>

// USE ONLY SC SP RING!!!
template <class TItem> class cRingCover
{
public:
    cRingCover() = default;
    cRingCover( const cRingCover& obj ) = delete;
    ~cRingCover()
    {
        if( ring_ )
        {
            rte_ring_free( ring_ );
            ring_ = nullptr;
        }
    }
    DC_ALWAYS_INLINE bool init( std::string name, uint32_t size, bool isSingle = true );

    uint32_t aval() const
    {
        return rte_ring_free_count( ring_ );
    }
    uint32_t eval() const
    {
        return rte_ring_count( ring_ );
    }

    uint32_t capacity() const
    {
        TA_LOGIC_ERROR( capacity_ );
        return capacity_;
    }
    DC_ALWAYS_INLINE bool put( TItem* obj );
    DC_ALWAYS_INLINE TItem* get();

    void setSNOwner( void* owner )
    {
        snOwner_ = owner;
    }
    void* snOwner() const
    {
        return snOwner_;
    }
    void setRCOwner( void* owner )
    {
        rcOwner_ = owner;
    }
    void* rcOwner() const
    {
        return rcOwner_;
    }

    bool isInited() const
    {
        return ring_ != nullptr;
    }

private:
    rte_ring* ring_ = nullptr;
    uint32_t capacity_ = 0;
    void* rcOwner_ = nullptr;
    void* snOwner_ = nullptr;
    std::string name_;
};

template <class TItem> bool cRingCover<TItem>::init( std::string name, uint32_t size, bool isSingle )
{
    name_ = "lz_" + name;
    uint32_t flags = RING_F_SP_ENQ | RING_F_SC_DEQ;
    if( !isSingle )
    {
        flags = RING_F_MC_RTS_DEQ | RING_F_MP_RTS_ENQ;
    }

    capacity_ = size - 1;
    L_DEBUG << "CREATING RNG WITH NAME: " << name_;
    ring_ = rte_ring_create( name_.c_str(), size, 0, flags );
    TA_LOGIC_ERROR( ring_ != nullptr );

    return true;
}

template <class TItem> inline bool cRingCover<TItem>::put( TItem* obj )
{
    TA_BAD_POINTER( ring_ );
    TA_BAD_POINTER( obj );

    int ret = rte_ring_enqueue_bulk( ring_, (void**)&obj, 1, nullptr );
    TA_LOGIC_ERROR( ret == 1 );
    return ret == 1;
}
template <class TItem> inline TItem* cRingCover<TItem>::get()
{
    TA_BAD_POINTER( ring_ );
    TItem* answer = nullptr;

    // rte_ring_sc_dequeue
    int ret = rte_ring_dequeue_bulk( ring_, (void**)&answer, 1, nullptr );
    TA_LOGIC_ERROR( ( ret == 1 ) || ( ret == 0 ) && ( !answer ) );
    return answer;
}

using FrameRing = cRingCover<cFrameBuffer>;

#endif // CRINGCOVER_H
