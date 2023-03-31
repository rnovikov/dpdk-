#ifndef IMIRRORPOINT_H
#define IMIRRORPOINT_H
#include <general/forward_declaration.h>
#include <general/base_types.h>


enum class eMirrorPointType
{
	PCAP_MIRROR
};

class iMirrorPoint
{
public:

	virtual ~iMirrorPoint() {}
	virtual uint32_t saveObjects( cMbuf** buffer, uint32_t size ) = 0;
	virtual void init() = 0;
	std::string name() const;
	eMirrorPointType type() const;
	void setInfo( const std::string& info );
	virtual std::string toStr() const;
	void exportToRow( tools::cTableRow& row, bool addFilePath = false ) const;
	static void exportHdrsToRow( tools::cTableRow& row, bool addFilePath = false );

	static std::string strMirrorPointType( eMirrorPointType type );
};

#endif // IMIRRORPOINT_H
