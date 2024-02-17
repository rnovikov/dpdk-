#ifndef CONNECTION_H
#define CONNECTION_H

#include <base/base_types.h>



class iReceiver
{
public:


    DC_ALWAYS_INLINE uint32_t rcvObjects( cMbuf** buffer, uint64_t size )
    {
        // TOO FAST
        // const uint64_t rnTime=get_cycles();
        TA_LOGIC_ERROR( size );
        const uint32_t answer = internal_receive_objects( buffer, size );
        for( uint i = 0; i < answer; ++i )
        {
            cMbuf* mbuf = buffer[i];

            //            hndlTime_.addMultiple(answer,get_cycles()-rnTime);
        }
        return answer;
    }

private:
    virtual uint32_t internal_receive_objects( cMbuf** buffer, uint64_t size ) = 0;
};

class iSender
{
public:


    virtual uint32_t getAval()const{NOT_IMPLEMENTED; return 0;}

    DC_ALWAYS_INLINE bool sndSingleObject( cMbuf* obj )
    {
        TA_BAD_POINTER( obj );

        const uint32_t answer = internal_send_objects( &obj, 1 );
        TA_LOGIC_ERROR( answer );
        return answer == 1;
    }
    DC_ALWAYS_INLINE uint32_t sndObjects( cMbuf** buffer, uint64_t size )
    {
        TA_LOGIC_ERROR( size );


        //        const uint64_t rnTime=get_cycles();
        const uint32_t answer = internal_send_objects( buffer, size );
        TA_LOGIC_ERROR( answer == size );

        //        hndlTime_.addMultiple(size,get_cycles()-rnTime);
        return answer;
    }

protected:
    virtual uint32_t internal_send_objects( cMbuf** buffer, uint64_t size ) = 0;
};

#endif // CONNECTION_H
