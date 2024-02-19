#include "cflowitem.h"

static inline rte_flow* make_port_rule( uint32_t port, uint32_t rxQ, const sFiveTypleSession& key )
{
    /* Declaring structs being used. 8< */
    sFlowItem flowItem( rxQ + 1 );
    rte_flow_action_queue qStruct;
    qStruct.index = rxQ;

    rte_flow* flow = NULL;

    rte_flow_item_ipv4 ip_spec;
    zero_struct( ip_spec );
    rte_flow_item_ipv4 ip_mask;
    zero_struct( ip_mask );
    rte_flow_item_tcp tcp_spec;
    zero_struct( tcp_spec );
    rte_flow_item_tcp tcp_mask;
    zero_struct( tcp_mask );
    rte_flow_item_udp udp_spec;
    zero_struct( udp_spec );
    rte_flow_item_udp udp_mask;
    zero_struct( udp_mask );

    flowItem.addEthPattern();

    if( key.dstIp_ || key.srcIp_ )
    {
        makeIpSpcMskFromSession( key, ip_spec, ip_mask );
    }

    rte_flow_item& ipPattern = flowItem.getPatternAndIncrementIndex();

    ipPattern.type = RTE_FLOW_ITEM_TYPE_IPV4;
    ipPattern.spec = &ip_spec;
    ipPattern.mask = &ip_mask;

    if( key.dstL4Port_ || key.srcL4Port_ )
    {
        TA_LOGIC_ERROR( key.proto_ );

        if( key.proto_ == IP_NP_TCP )
        {
            rte_flow_item& tcpPattern = flowItem.getPatternAndIncrementIndex();
            makePortSpcMskFromSession( key, tcp_spec, tcp_mask );
            tcpPattern.type = RTE_FLOW_ITEM_TYPE_TCP;
            tcpPattern.spec = &tcp_spec;
            tcpPattern.mask = &tcp_mask;
        }
        else if( key.proto_ == IP_NP_UDP )
        {
            makePortSpcMskFromSession( key, udp_spec, udp_mask );
            rte_flow_item& udpPattern = flowItem.getPatternAndIncrementIndex();
            udpPattern.type = RTE_FLOW_ITEM_TYPE_UDP;
            udpPattern.spec = &udp_spec;
            udpPattern.mask = &udp_mask;
        }
        else
        {
            NOT_IMPLEMENTED;
        }
    }
    flowItem.addStopPattern();

    /* >8 End of declaring structs being used. */
    int res;
    /* Set the rule attribute, only ingress packets will be checked. 8< */
    flowItem.attr_.ingress = 1;
    flowItem.setQueueAction( qStruct );

    rte_flow_error error;
    /* >8 End of final level must be always type end. */

    /* Validate the rule and create it. 8< */
    res = rte_flow_validate( port, &flowItem.attr_, flowItem.getPatternPointer(), flowItem.action_, &error );
    if( res )
    {
        L_EMERG << res;
        L_EMERG << error.message;
        if( flowItem.attr_.egress )
        {
            L_EMERG << "EGRESS !";
        }

        NOT_IMPLEMENTED;
    }
    flow = rte_flow_create( port, &flowItem.attr_, flowItem.getPatternPointer(), flowItem.action_, &error );
    /* >8 End of validation the rule and create it. */
    TA_BAD_POINTER( flow );
    return flow;
}


void cFlowItem::addEthPattern()
{
    TA_LOGIC_ERROR( currentPatternId_ < FLOW_MAX_PATTERN_NUMBER );
    rte_flow_item& pt = pattern_[currentPatternId_];
    pt.type = RTE_FLOW_ITEM_TYPE_ETH;
    currentPatternId_++;
}

void cFlowItem::addStopPattern()
{
    rte_flow_item& pt = pattern_[currentPatternId_];
    pt.type = RTE_FLOW_ITEM_TYPE_END;
    currentPatternId_ = BAD_UINT32_VALUE;
}

rte_flow_item& cFlowItem::getPatternAndIncrementIndex()
{
    rte_flow_item& answer = pattern_[currentPatternId_];
    currentPatternId_++;
    return answer;
}
