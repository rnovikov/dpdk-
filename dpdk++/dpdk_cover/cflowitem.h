#ifndef CFLOWITEM_H
#define CFLOWITEM_H
#include <base/base_types.h>
#include <rte_flow.h>
#define FLOW_MAX_PATTERN_NUMBER 16
#define FLOW_MAX_ACTION_NUMBER 8
class cFlowItem
{

public:
    rte_flow_attr attr_;

private:
    rte_flow_item pattern_[FLOW_MAX_PATTERN_NUMBER];

public:
    rte_flow_action action_[FLOW_MAX_ACTION_NUMBER];
    rte_flow_action_port_id tmpNamedAction;

    uint32_t currentPatternId_ = 0;

    void addEthPattern();
    void addStopPattern();
    void addIpPattern();

    rte_flow_item* getPatternPointer()
    {
        TA_LOGIC_ERROR( currentPatternId_ == BAD_UINT32_VALUE );
        return &pattern_[0];
    }

    rte_flow_item& getPatternAndIncrementIndex();

    void setQueueAction( const rte_flow_action_queue& q )
    {
        action_[0].type = RTE_FLOW_ACTION_TYPE_QUEUE;
        action_[0].conf = &q;
        action_[1].type = RTE_FLOW_ACTION_TYPE_END;
    }
    void setPaththrough()
    {
        NOT_IMPLEMENTED;
        tmpNamedAction.id = 0;
        tmpNamedAction.original = 0;
        action_[0].type = RTE_FLOW_ACTION_TYPE_PORT_ID;
        action_[0].conf = &tmpNamedAction;
        action_[1].type = RTE_FLOW_ACTION_TYPE_END;
    }
    sFlowItem( uint32_t id )
    {
        zero_struct( attr_ );
        zero_struct( pattern_ );
        zero_struct( action_ );
        zero_struct( tmpNamedAction );
        attr_.priority = id;
    }
};

#endif // CFLOWITEM_H
