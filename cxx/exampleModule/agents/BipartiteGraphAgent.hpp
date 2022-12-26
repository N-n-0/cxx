#pragma once
#include <algorithm>
#include <vector>
#include <sc-memory/kpm/sc_agent.hpp>
#include <type_traits>
#include "keynodes/keynodes.hpp"
#include "BipartiteGraphAgent.generated.hpp"
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_iterator.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_type.hpp"
#include "sc-memory/utils/sc_log.hpp"

namespace ExampleModule {


	class GraphBipartyleFinder
	{
		public:

    		GraphBipartyleFinder(ScMemoryContext *context, const std::string& graph_name) :
        	context(context)
    		{
        		graph = context->HelperResolveSystemIdtf(graph_name);

        		rrel_arcs = context->HelperResolveSystemIdtf("rrel_arcs");
        		rrel_nodes = context->HelperResolveSystemIdtf("rrel_nodes");

        	ScIterator5Ptr arcs_set_it = context->Iterator5(
            		graph,
            		ScType::EdgeAccessConstPosPerm,
            		ScType::Node,
            		ScType::EdgeAccessConstPosPerm,
            		rrel_arcs
        	);

        	ScIterator5Ptr nodes_set_it = context->Iterator5(
            		graph,
            		ScType::EdgeAccessConstPosPerm,
            	ScType::Node,
            	ScType::EdgeAccessConstPosPerm,
            	rrel_nodes
        );

        if (arcs_set_it->Next())
            graph_arcs = arcs_set_it->Get(2);

        if (nodes_set_it->Next())
            graph_nodes = nodes_set_it->Get(2);
    	}

    	bool Find()
    	{
        	first_partyle = context->CreateNode(ScType::Node);
        	second_partyle = context->CreateNode(ScType::Node);

        	ScIterator3Ptr nodes_it = context->Iterator3(
            		graph_nodes,
            		ScType::EdgeAccessConstPosPerm,
            		ScType::Node
        	);

        	if (nodes_it->Next())
        	{
            		ScAddr current_node = nodes_it->Get(2);
            		ScAddr visited_nodes = context->CreateNode(ScType::Node);

            	if (!CheckInDepth(current_node, visited_nodes, true))
            	{
                	SetGraphStatus(false);
                	return false;
            	}

            	while (nodes_it->Next())
            	{
                	ScAddr node = nodes_it->Get(2);
                	if (!ExistsInSet(visited_nodes, node))
                	{
                    		SetGraphStatus(false);
                    		return false;
                	}
            	}
        }

        	SetGraphStatus(true);
        	return true;
    }

    	void Print()
    	{
        	std::cout << "First patryle: " << std::endl;
        	PrintNodesSet(first_partyle);
        	std::cout << std::endl;

        	std::cout << "Second partyle: " << std::endl;
        	PrintNodesSet(second_partyle);
        	std::cout << std::endl;
    	}

	private:

    	void SetGraphStatus(bool isBipartyle)
    	{
        	ScAddr node = context->HelperResolveSystemIdtf("concept_bipartite_graph");

        	if (!node.IsValid())
        	{
            	node = context->CreateNode(ScType::NodeClass);
            	context->HelperSetSystemIdtf("concept_bipartite_graph", node);
        	}

        	if (isBipartyle)
        	{
            		context->CreateArc(ScType::EdgeAccessConstPosPerm, node, graph);
        	}
        	else
        	{
            		context->CreateArc(ScType::EdgeAccessConstNegPerm, node, graph);
        	}
    	}

    	bool CheckInDepth(const ScAddr& node, ScAddr& visited_nodes, bool is_first_partyle)
    	{
        	if (is_first_partyle && ExistsInSet(second_partyle, node))
        	{
            		return false;
        	}

        	if (!is_first_partyle && ExistsInSet(first_partyle, node))
        	{
            		return false;
        	}	

        	if (ExistsInSet(visited_nodes, node))
        	{
            		return true;
        	}

        	AddToSet(visited_nodes, node);
        	AddToSet(is_first_partyle ? first_partyle : second_partyle, node);

        	ScIterator5Ptr incident_nodes_it_forward = context->Iterator5(
            	node,
            	ScType(0),
            	ScType::Node,
            	ScType::EdgeAccessConstPosPerm,
            	graph_arcs
        	);

        	while (incident_nodes_it_forward->Next())
        	{
            	ScAddr incident_node = incident_nodes_it_forward->Get(2);

            	if (!CheckInDepth(incident_node, visited_nodes, !is_first_partyle))
            	{
                	return false;
            	}
        	}

        	ScIterator5Ptr incident_nodes_it_backward = context->Iterator5(
            	ScType::Node,
            	ScType(0),
            	node,
            	ScType::EdgeAccessConstPosPerm,
            	graph_arcs
        	);

        	while (incident_nodes_it_backward->Next())
        	{
            	ScAddr incident_node = incident_nodes_it_backward->Get(0);

            	if (!CheckInDepth(incident_node, visited_nodes, !is_first_partyle))
            	{
                	return false;
            	}
        	}

        	return true;
    	}

    	void PrintNodesSet(const ScAddr& set)
    	{
        	ScIterator3Ptr nodes_it = context->Iterator3(
            	set,
            	ScType::EdgeAccessConstPosPerm,
            	ScType::Node
        	);

        	while (nodes_it->Next())
        	{
            	ScAddr node = nodes_it->Get(2);

            	std::string idtf = context->HelperGetSystemIdtf(node);
            	std::cout << "  " << idtf << std::endl;
        	}
    	}

    	bool ExistsInSet(const ScAddr& set, const ScAddr& element)
    	{
        	return context->HelperCheckEdge(
            	set,
            	element,
            	ScType::EdgeAccessConstPosPerm
        	);
    	}

   	 void AddToSet(const ScAddr& set, const ScAddr& element)
    	{
	        context->CreateEdge(ScType::EdgeAccessConstPosPerm, set, element);
    	}

    	std::unique_ptr<ScMemoryContext> context;

    	ScAddr rrel_arcs;
    	ScAddr rrel_nodes;

    	ScAddr graph;

    	ScAddr graph_nodes;
    	ScAddr graph_arcs;

   	ScAddr first_partyle, second_partyle;

	};
}
