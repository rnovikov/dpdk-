#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H
#include <vector>
#include <string>
#include <map>


namespace dc_tools
{

using TRow = std::vector< std::string >;

class cTableRow
{
public:
    cTableRow();
    cTableRow( const TRow& from );

    cTableRow& operator<<( const std::string& val );
    cTableRow& operator<<( uint32_t val );
    cTableRow& operator<<( uint64_t val );
    cTableRow& operator<<( uint16_t val );
    cTableRow& operator<<( uint8_t val );
    cTableRow& operator<<( int val );
    cTableRow& operator<<( double val );
    cTableRow& operator<<( const cTableRow& row );

protected:
    const TRow& getElements() const;
    friend class cTableView;

private:
    TRow elements_;
};

class cTableView
{
public:
    cTableView( const std::string& name, std::string sep = "|" );

    cTableRow& addRow();
    std::string toStr() const;
    std::string name() const;
    std::map< std::string, std::vector< std::string > > getTable( const std::string& val = "-//-" ) const;

private:
    std::string separator_;
    std::vector< cTableRow > rows_;

    std::vector< uint32_t > getWidth() const;

    std::string name_;
};

}
#endif // CTABLEVIEW_H
