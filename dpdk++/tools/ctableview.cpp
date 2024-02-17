#include "ctableview.h"
#include <iostream>
#include <iomanip>

using namespace std;
namespace dc_tools
{
cTableView::cTableView( const string& name, std::string sep ) : name_( name ), separator_( sep )
{
}

cTableRow& cTableView::addRow()
{
    rows_.push_back( cTableRow() );
    return rows_[rows_.size() - 1];
}

string cTableView::name() const
{
    return name_;
}

std::map< std::string, std::vector< std::string > > cTableView::getTable( const std::string& val ) const
{
    std::map< std::string, std::vector< std::string > > answer;
    std::vector< uint32_t > width = getWidth();
    uint32_t counter = 0;

    for( const auto& item : rows_ )
    {
        stringstream ss;
        ss << setw( 3 ) << setfill( '0' ) << counter;
        counter++;
        std::vector< string > values;
        values.resize( width.size(), val );
        const TRow& row = item.getElements();
        for( int i = 0; i < row.size(); ++i )
        {
            values[i] = row[i];
        }
        answer[ss.str()] = values;
    }

    return answer;
}

std::string cTableView::toStr() const
{
    stringstream ss;
    ss << "TABLE:\"" << name_ << "\" SIZE:" << rows_.size() << endl;
    std::vector< uint32_t > wdt = getWidth();
    for( const cTableRow& row : rows_ )
    {
        const TRow& elem = row.getElements();
        for( uint i = 0; i < elem.size(); ++i )
        {
            ss << setw( wdt[i] ) << elem[i] << separator_;
        }
        ss << endl;
    }
    return ss.str();
}

std::vector< uint32_t > cTableView::getWidth() const
{
    std::vector< uint32_t > answer;
    uint32_t maxColumn = 0;

    for( const cTableRow& row : rows_ )
    {
        uint32_t cur = row.getElements().size();
        if( cur > maxColumn )
        {
            maxColumn = cur;
        }
    }

    for( uint i = 0; i < maxColumn; ++i )
    {
        uint32_t maxWidth = 0;
        for( const cTableRow& row : rows_ )
        {
            uint32_t sz = row.getElements().size();

            if( i < sz )
            {
                uint32_t elSize = row.getElements()[i].size();

                if( elSize > maxWidth )
                {
                    maxWidth = elSize;
                }
            }
        }
        answer.push_back( maxWidth );
    }

    return answer;
}

cTableRow::cTableRow()
{
}

cTableRow::cTableRow( const TRow& from )
{
    elements_ = from;
}

cTableRow& cTableRow::operator<<( const std::string& val )
{
    elements_.push_back( val );
    return *this;
}

cTableRow& cTableRow::operator<<( int val )
{
    return operator<<( to_string( val ) );
}

cTableRow& cTableRow::operator<<( uint64_t val )
{
    return operator<<( to_string( val ) );
}

cTableRow& cTableRow::operator<<( uint32_t val )
{
    return operator<<( to_string( val ) );
}

cTableRow& cTableRow::operator<<( uint16_t val )
{
    return operator<<( to_string( (uint32_t)val ) );
}

cTableRow& cTableRow::operator<<( uint8_t val )
{
    return operator<<( to_string( (uint32_t)val ) );
}

cTableRow& cTableRow::operator<<( double val )
{
    return operator<<( to_string( val ) );
}

cTableRow& cTableRow::operator<<( const cTableRow& row )
{
    for( std::string str : row.getElements() )
    {
        elements_.push_back( str );
    }
    return *this;
}

const TRow& cTableRow::getElements() const
{
    return elements_;
}

}
