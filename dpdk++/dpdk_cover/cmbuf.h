#ifndef CMBUF_H
#define CMBUF_H
#include <rte_mbuf.h>
#include <string>
#include <general/base_defines.h>
#include <general/error_handler.h>
#include <rte_ether.h>







struct sMetaData
{

};

class cMbuf: public rte_mbuf
{
public:




//	template < class T > FORCE_INLINE T* getL3Payload() const;
//	FORCE_INLINE uint32_t getL3PlLen() const;
//	template < class T > FORCE_INLINE T* getL4Payload() const;
//	template < class T > FORCE_INLINE const T* getDataByOffset( uint16_t offset = 0 ) const;
//	template < class T > FORCE_INLINE T* getDataByOffset( uint16_t offset = 0 );
//	FORCE_INLINE rte_mbuf* mbuf() const;
//	FORCE_INLINE sMetaData* meta();
//	FORCE_INLINE const sMetaData* metaConst() const;
//	FORCE_INLINE void init( uint16_t sourcePhyPort, const uint64_t& timestamp );
//	FORCE_INLINE void handleL2();
//	FORCE_INLINE void handleL4( uint8_t vrf = DEFAULT_VRF );
//	FORCE_INLINE void setDstEther( const rte_ether_addr& addr );
//	FORCE_INLINE void setSrcEther( const rte_ether_addr& addr );

//	FORCE_INLINE sContrackValue* getContrackValue() const;

//	FORCE_INLINE void setDirection( bool b );
//	FORCE_INLINE ip_conntrack_dir getDirection() const;
//	void fillSuricataMeta();

//	FORCE_INLINE void vlRemove();
//	FORCE_INLINE void vlSet( uint16_t inner, uint16_t outer );
//	FORCE_INLINE void vlAdd();

//	FORCE_INLINE void adj( uint16_t len );
//	FORCE_INLINE void prepend( uint16_t len );

//	template < prefetch::ePrefetchType prefetchType, uint32_t flags > FORCE_INLINE void prefetch() const;

//	FORCE_INLINE void setDestinationIfaceData( const sFpIface& iface );





//	uint32_t doSimpleRssFiveTuple();



//	inline void free();
//	void fakeInit();
//	std::string strEther() const;
//	bool hasL4Handle() const;

//	FORCE_INLINE void applyCtKey();

//	FORCE_INLINE void makeChecksum();
//	FORCE_INLINE bool checkIpChecksum() const;
//	FORCE_INLINE bool checkTcpChecksum() const;
//	FORCE_INLINE uint32_t comparePayload( const cDfMbuf* target, uint32_t len, uint32_t start = 0 ) const;



	// compile time section


	// slow path
	std::string toStr( uint64_t flags  ) const;

private:
	explicit cMbuf();

};

#endif // CMBUF_H
